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

#include <map>
#include <deque>
#include <algorithm>

#include <wx/string.h>

#include "server.h"
#include "query.h"

class DataController;
class Label;

class QueryResult {
private:
	DataController* ctrl;

public:
	class QueryItem {
	public:
		Server	server;
		int		idx;

		QueryItem() : idx(-1) {
		}

	};

	// Contains a full list of all known servers
	typedef std::map<wxString,QueryItem>	serverlist_t;
	serverlist_t	serverlist;

	// Contains a list of server-names, we need to add to the list
	typedef std::deque<wxString> servernamequeue_t;
	servernamequeue_t	updatedServers;

	Query	query;

	QueryResult(DataController*, const Query&);
	~QueryResult();

	void add(const wxString& name, Server s) {
		servernamequeue_t::iterator j = find(this->updatedServers.begin(), this->updatedServers.end(), name);
		if(j == this->updatedServers.end() )	// only add it once
			this->updatedServers.push_back(name);

		this->serverlist[name].server = s;
	}

	ssize_t size() {
		return this->serverlist.size();
	}
};

#endif
