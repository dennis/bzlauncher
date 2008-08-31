/*
The MIT License

Copyright (c) 2008 Dennis Møllegaard Pedersen <dennis@moellegaard.dk>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include <wx/log.h>
#include "datactrl.h"
#include "datasrc.h"
#include "query.h"

DataController::~DataController() {
	wxLogDebug(_T("DataController::~DataController"));
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i )
		delete *i;
	this->sourceList.clear();

	for(labelmap_t::iterator i = this->labelMap.begin(); i != this->labelMap.end(); ++i )
		delete i->second;
	this->labelMap.clear();
}

void DataController::add(DataSource* ds) {
	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	wxLogDebug(_T("DataController::add(%p)"), ds);
	ds->initializeLabels(this);
	this->sourceList.push_back(ds);
}

void DataController::run() {
	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	wxLogDebug(_T("DataController::run()"));
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i )
		(*i)->Run();
}

void DataController::stop() {
	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	wxLogDebug(_T("DataController::stop()"));
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i ) {
		(*i)->Kill();
		(*i)->Wait();
	}
}

void DataController::addQueryResult(QueryResult* res) {
	this->lock.Lock();
	this->queryResultList.push_back(res);
	this->lock.Unlock();

	wxLogDebug(_T("DataController: Got %d Query-Result"), this->queryResultList.size());
}

void DataController::removeQueryResult(QueryResult* res) {
	this->lock.Lock();

	bool exit = false;

	for(qreslist_t::iterator i = this->queryResultList.begin(); !exit && (i != this->queryResultList.end()); ++i ) {
		if( *i == res )  {
			this->queryResultList.erase(i);
			exit = true;
		}
	}
	this->lock.Unlock();

	wxLogDebug(_T("DataController: Got %d Query-Result"), this->queryResultList.size());
}

QueryResult* DataController::search(const Query& q) {
	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	QueryResult* res = new QueryResult(this, q);
	wxLogDebug(_T("DataController - new QueryResult %p"), res);
	this->lock.Lock();
	for(entitymap_t::iterator i = this->serverList.begin(); i != this->serverList.end(); ++i ) {
		if( q == i->second )
			res->add(i->first, i->second.dupe());
	}
	this->lock.Unlock();
	this->addQueryResult(res);
	return res;
}

void DataController::work() {
	typedef std::map<wxString,bool>	dirtylist_t;
	dirtylist_t dirty;
	dirtylist_t newservers;

	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	wxLogDebug(_T("DataController::work(). Serverlist got %d entries"), this->serverList.size());

	this->lock.Lock();
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i ) {
		int size = (*i)->out_queue.size();
		wxLogDebug(_T("DataSource %p got %d in queue"), (*i), size);
		if(size > 4096) {
			size = 4096;
			wxLogDebug(_T(" ..clamped to %d"), size);
		}
		while(size) {
			// Update our serverlist
			FullAttributeInfo info = (*i)->out_queue.pop();	// Throws it away!
			if(this->serverList.find(info.server) == this->serverList.end()) {
				newservers[info.server] = 1;
			}
			this->serverList[info.server].update(info);
			size--;

			// Mark it as dirty
			dirty[info.server] = 1;
		}
	}

	// Update the result-lists
	for(dirtylist_t::iterator d = dirty.begin(); d != dirty.end(); ++d) {
		for(qreslist_t::iterator q = this->queryResultList.begin(); q != this->queryResultList.end(); ++q ) {
			if( (*q)->query == this->serverList[d->first] )
				(*q)->add(d->first, this->serverList[d->first]);
		}
	}

	// Announce new servers
	for(dirtylist_t::iterator d = dirty.begin(); d != dirty.end(); ++d) {
		this->eventNewServer(d->first);
	}

	this->lock.Unlock();
}

void DataController::eventNewServer(const wxString& name) {
	wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
	Server& server = this->serverList[name];
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i ) {
		(*i)->eventNewServer(name, server);	
	}
}
