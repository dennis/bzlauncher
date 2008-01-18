#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/listimpl.cpp>
#include <wx/utils.h>

#include "main.h"
#include "listserverhandler.h"

WX_DEFINE_LIST(ServerList);

void ListServerHandler::GetServerList() {
	wxBusyCursor wait;
	BZLauncherApp& app = wxGetApp();

	app.SetStatusText(_("Fetching data from list-server..."));

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
			// We only parse BZFS0026 for now
			if( token.Cmp(_T("BZFS0026")) != 0 )
				return false;
			break;
		case 2:
			s->ParseServerInfo(token);
			break;
		case 3:
			s->ip.Hostname(token);

			// Get remaining stuff
			s->name += tok.GetString();
			break;
		}
		i++;
	}

	this->serverList.Append(s);

	return true;
}

bool ListServerHandler::GetListServerResponse() {
	wxURL listserv(wxT("http://my.bzflag.org/db?action=LIST"));

	if(listserv.IsOk()) {
		wxInputStream *in_stream;
		in_stream = listserv.GetInputStream();
		char		buffer[1024];

		this->rawResponse.Clear();

		while(!in_stream->Read(buffer,1024).Eof()) 
			this->rawResponse << wxString::From8BitData(buffer,in_stream->LastRead());

		return true;
	}

	return false;
}

void ListServerHandler::ClearList() {
	ServerList::iterator i;
	for(i = this->serverList.begin(); i != this->serverList.end(); ++i) {
		Server*	current = *i;

		delete current;
	}

	this->serverList.Clear();
}
