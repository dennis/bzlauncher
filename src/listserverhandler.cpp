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
#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/listimpl.cpp>
#include <wx/utils.h>

#include "main.h"
#include "config.h"
#include "listserverhandler.h"
#include "serverparser.h"

/*
 * Serverlist is a text/plain response from bzflag.org servers. Each line describes a single
 * bzflag server (bzfs). The format is:
 * hostname:port version hexcoded-info ip name
 * where:
 *   - hostname:port   is the connection info
 *   - version         is the protcol version. currently BZFS0026 is used
 *   - hexcoded info   contains some server details (ctf,ffa.., max allowed players) 
 *                      etc.  version-specific
 *   - ip              ip-address of the hostname
 *   - name            The remaining chars of the line, is the servername as
 *                      shown in "Find server" option in bzfs
 */
WX_DEFINE_LIST(ServerList);

ListServerHandler::ListServerHandler() : version(0) {
}

ListServerHandler::~ListServerHandler() {
	this->ClearList();
}


void ListServerHandler::GetServerList() {
	wxBusyCursor wait;
	BZLauncherApp& app = wxGetApp();

	app.SetStatusText(_("Fetching data from list-server..."));

	// TODO: Dont clear the list just yet. keep it while building the 
	// newer version and after doing this, delete it. This will allow
	// the ServerPing-classes to reuse ping-measurements without needing
	// to reping them.
	this->ClearList();
	this->version++;

	if( this->GetListServerResponse() ) {
		app.SetStatusText(_("Parsing data from list-server..."));
		wxStringTokenizer tok(this->rawResponse, _T("\r\n"));

		while(tok.HasMoreTokens()) {
			wxString token = tok.GetNextToken();
			
			this->ParseLine(token);
		}
	}
	else {
		wxLogError(_("Can't connect to listserver!"));
	}
	app.SetStatusText(wxString::Format(_("Found %d server(s)"), this->serverList.GetCount()));
}

bool ListServerHandler::ParseLine(const wxString& line) {
	Server* s = new Server;

	s->fullyParsed  = false;
	s->favorite     = false;

	wxStringTokenizer tok(line, _T(" "));

	int i = 0;
	long port;
	wxString host;
	while(tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();
		switch(i) {
		case 0:	{ // servername:port
			s->setName(token);

			port = 5154;
			host = token;
			int pos;

			if((pos = token.Find(':',true)) != wxNOT_FOUND) {
				if(!token.Mid(pos+1).ToLong(&port))
					port = 5154;
				host = token.Mid(0,pos-1);
			}

			}
			break;
		case 1:  // version 
			s->protocolVersion = token;
			break;
		case 2:  // Hex info
			{
				ServerHexParser* p = ServerHexParser::GetParser(s->protocolVersion);
				if(p) {
					p->parse(token,*s);
					delete p;
				}
			}
			break;
		case 3: 
			{
				wxIPV4address ip;
				ip.Hostname(token);
				ip.Service(port);
				s->setIP(ip);
				s->longName += tok.GetString();
			}
			break;
		}
		i++;
	}

	this->serverList.Append(s);

	return true;
}

bool ListServerHandler::GetListServerResponse() {
	wxFileSystem	fs;
	wxFSFile*		listserv = NULL;
	wxString		url;
	int				c = 0;
	bool			r = false;

	while( listserv == NULL && (url = appConfig.getListServerURL(c++)) && !url.IsEmpty())
		listserv = fs.OpenFile(url);

	if(listserv) {
		wxInputStream *in_stream;
		in_stream = listserv->GetStream();

		if(in_stream) {
			char		buffer[1024];

			this->rawResponse.Clear();

			while(in_stream->Read(buffer,1024).LastRead()) {
				this->rawResponse << wxString::From8BitData(buffer,in_stream->LastRead());
			}

			r = true;
		}
		else {
			wxLogMessage(_("in_stream == null"));
		}

		delete listserv;
	}

	return r;
}

void ListServerHandler::ClearList() {
	ServerList::iterator i;
	for(i = this->serverList.begin(); i != this->serverList.end(); ++i) {
		Server*	current = *i;
		delete current;
	}

	this->serverList.Clear();
}

Server* ListServerHandler::FindByName(const wxString& n) {
	ServerList::iterator i;
	for(i = this->serverList.begin(); i != this->serverList.end(); ++i) {
		Server*	current = *i;

		if(n.Cmp(current->getName()) == 0)
			return current;
	}

	return NULL;
}

// TODO: This will perform a search of the this->serverList. For now, it dosn't really
// do anything, except return a ServerResultSet (like searching that matches everything)
ServerResultSet ListServerHandler::Query() {
	ServerList		serverList;
	ServerResultSet	resultSet;

	for(ServerList::iterator i = this->serverList.begin(); i != this->serverList.end(); ++i) {
		Server*	current = *i;

		resultSet.push_back(current);
	}

	return resultSet;
}
