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
#ifndef __listserver_handler__
#define __listserver_handler__

#include <wx/string.h>
#include <wx/list.h>
#include <wx/filesys.h>
#include <wx/arrstr.h>
#include <vector>

#include "ping.h"
#include "query.h"
#include "server.h"

WX_DECLARE_LIST(Server, ServerList);
typedef std::vector<Server*> ServerResultSet;

/// Performs the listserver interaction - mainly parsing the data
/// received from i mainly parsing the data
class ListServerHandler {
private:

	bool GetListServerResponse();
	bool ParseLine(const wxString&, wxArrayString&, wxArrayString&);

	void ClearList();

	wxString	rawResponse;

protected:
	int version;

	PingList	pingList;

public:
	ServerList		serverList;

	ListServerHandler();
	~ListServerHandler();

	int getVersion() {
		return version;
	}

	ServerResultSet Search(const Query&);

	void GetServerList(wxArrayString&,wxArrayString&);
	Server* FindByName(const wxString&);
};



#endif
