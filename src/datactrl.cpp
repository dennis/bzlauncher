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
	this->sourceList.push_back(ds);
}

void DataController::run() {
	wxLogDebug(_T("DataController::run()"));
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i )
		(*i)->Run();
}

void DataController::stop() {
	wxLogDebug(_T("DataController::stop()"));
	for(sourcelist_t::iterator i = this->sourceList.begin(); i != this->sourceList.end(); ++i ) {
		(*i)->Kill();
		(*i)->Wait();
	}
}

QueryResult DataController::search(const Query& q) {
	QueryResult res(this);
	this->lock.Lock();
	for(entitymap_t::iterator i = this->serverList.begin(); i != this->serverList.end(); ++i ) {
		res.add(i->second.dupe());	// Make sure we're giving a copy (that by-passes ref-counting)
	}
	this->lock.Unlock();
	return res;
}
