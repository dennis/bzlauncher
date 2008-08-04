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
#ifndef __datactrl_h__
#define __datactrl_h__

#include <wx/string.h>
#include <wx/utils.h>
#include <vector>
#include <map>

#include "label.h"
#include "server.h"
#include "attribute.h"
#include "datasrc.h"
#include "queryresult.h"

class Query;

// This class controls/owns the datalist (serverlist) 
class DataController {
private:
	unsigned long	pid;
protected:
	typedef std::vector<DataSource*>	sourcelist_t;
	typedef std::map<wxString,Server>	entitymap_t;
	typedef std::vector<QueryResult*>	qreslist_t;

	sourcelist_t	sourceList;
	entitymap_t		serverList;
	qreslist_t		queryResultList;

	wxMutex	lock;

public:
	typedef std::map<wxString,Label*>		labelmap_t;
	labelmap_t		labelMap;

	DataController() {
		this->pid = wxGetProcessId();
	}

	~DataController();

	template<typename T>
	void updateAttribute(const wxString& name, const Label* l, const Attribute<T>& val) {
		wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
		this->lock.Lock();
		this->serverList[name].update(l,val);
		for(qreslist_t::iterator i = this->queryResultList.begin(); i != this->queryResultList.end(); ++i )
		/*
			if( (*i)->query == this->serverList[name] )
				wxLogDebug(_T(".."));
		*/
		this->lock.Unlock();
	}

	void addLabel(Label* l) {
		wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
		this->labelMap[l->getTag()] = l;
	}

	void add(DataSource*);
	void run();
	void stop();

	AttributeBase* getAttribute(const wxString& name, const Label* l) {
		wxASSERT_MSG(this->pid == wxGetProcessId(), _T("not invoked from main thread"));
		this->lock.Lock();
		if( this->serverList.find(name) == this->serverList.end() )
			return NULL;
		else
			return this->serverList[name].get(l);
		this->lock.Unlock();
	}

	void addQueryResult(QueryResult*);
	void removeQueryResult(QueryResult*);

	QueryResult* search(const Query&);
};

#endif 
