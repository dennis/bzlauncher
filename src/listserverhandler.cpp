#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/listimpl.cpp>
#include <wx/utils.h>

#include "main.h"
#include "config.h"
#include "listserverhandler.h"

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

ListServerHandler::ListServerHandler() {
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
	while(tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();
		switch(i) {
		case 0:	
			s->serverHostPort = token;
			break;
		case 1:
			s->protocolVersion = token;
			break;
		case 2:
			if(s->protocolVersion.Cmp(_T("BZFS0026")) == 0) {
				s->ParseServerInfoBZFS0026(token);
				s->fullyParsed = true;
			}
			else {
				s->fullyParsed = false;
			}
			break;
		case 3: {
			long port = 5154;
			// Get Port from serverHostPort
			int pos;
			if((pos = s->serverHostPort.Find(':',true)) != wxNOT_FOUND) {
				if(!s->serverHostPort.Mid(pos+1).ToLong(&port))
					port = 5154;
			}

			wxIPV4address ip;
			ip.Hostname(token);
			ip.Service(port);
			s->setIP(ip);

			// Get remaining stuff
			s->name += tok.GetString();
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

		if(n.Cmp(current->serverHostPort) == 0)
			return current;
	}

return NULL;
}
