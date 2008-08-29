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
#ifndef __queryresult_h__
#define __queryresult_h__

#include <vector>
#include <algorithm>

#include <wx/string.h>

#include "server.h"
#include "query.h"

class DataController;
class Label;

class QueryResult {
private:
	typedef std::vector<Server>	serverlist_t;
	serverlist_t	serverlist;

	DataController* ctrl;

	serverlist_t::iterator	pointer;
	int		pointer_is;

public:
	Query	query;

	QueryResult(DataController*, const Query&);
	~QueryResult();

	void add(Server s) {
		serverlist_t::iterator i = find(this->serverlist.begin(), this->serverlist.end(), s);
		if( i != this->serverlist.end() )
			this->serverlist.erase(i);
		this->serverlist.push_back(s);
	}

	int size() {
		return this->serverlist.size();
	}

	wxString getAttribute(int row, Label* l) {
		if( row <= 0 ) {
			this->pointer = this->serverlist.begin();
			this->pointer_is = 0;
		}
		if( row == this->pointer_is+1 ) {
			this->pointer++;
			this->pointer_is++;
		}
		if( row >= this->serverlist.size() ) {
			this->pointer = this->serverlist.end();
		}

		if( this->pointer != this->serverlist.end() ) {
			Server& s = *(this->pointer);
			AttributeBase*	ab = s.get(l);
			if(ab)
				return ab->aswxString();
		}
		return _T("N/A");
	}
};

#endif
