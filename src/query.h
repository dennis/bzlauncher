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
#ifndef __query_h__
#define __query_h__

#include <wx/string.h>
#include <wx/arrstr.h>
#include <datactrl.h>

class Server;

class Query {
private:
	wxString		query;
	wxArrayString	tokens;
	wxString		name;

public:
	Query(const wxString& q = _T("")) {
		this->set(q);
	}

	wxString get() const {
		if( this->query.IsEmpty() )
			return wxString(_T("All"));
		else
			return this->query;
	}

	wxString getName() const {
		if( this->name.IsEmpty() )
			return this->get();
		else
			return this->name;
	}

	void set(const wxString &);

	bool operator==(const Server*) const;
	bool operator!=(const Server* s) const {
		return !(*this == s);
	}
};

class QueryResult {
private:
	typedef std::vector<Server>	list_t;
	list_t	list;
	DataController* ctrl;

	list_t::iterator	pointer;
	int		pointer_is;

public:
	QueryResult(DataController* c) : ctrl(c), pointer(this->list.end()), pointer_is(-1) {
	}

	void add(Server s) {
		this->list.push_back(s);
	}

	int size() {
		return this->list.size();
	}

	wxString getAttribute(int row, Label* l) {
		if( row <= 0 ) {
			this->pointer = this->list.begin();
			this->pointer_is = 0;
		}
		if( row == this->pointer_is+1 ) {
			this->pointer++;
			this->pointer_is++;
		}
		if( row >= this->list.size() ) {
			this->pointer = this->list.end();
		}

		if( this->pointer != this->list.end() ) {
			Server s = *(this->pointer);
			AttributeBase*	ab = s.get(l);
			if(ab)
				return ab->aswxString();
		}
		return _T("N/A");
	}
};

#endif
