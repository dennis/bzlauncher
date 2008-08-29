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
#ifndef __view_h__
#define __view_h__

#include <wx/listctrl.h>
#include <vector>
#include "query.h"
#include "queryresult.h"


class ServerListView {
private:
public:
	wxListCtrl*	serverList;
	long		currentSortMode; // = colnum+1. If pos then asending, if neg then decending sort
	Query			query;
	QueryResult*	result;
	int				version;

	ServerListView(const Query&, long);

	~ServerListView() {
		if( this->serverList ) 
			delete this->serverList;
		if( this->result )
			delete this->result;
	}

	const wxString GetName() const {
		return this->query.getName();
	}
};

typedef std::vector<ServerListView*>	viewlist_t;

#endif
