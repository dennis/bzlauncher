#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>
#include <wx/listimpl.cpp>
#include <wx/utils.h>

#include "main.h"
#include "listserver.h"

WX_DEFINE_LIST(ListServerList);

void ListServer::GetServerList() {
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
	app.SetStatusText(wxString::Format(_("Found %d server(s)"), this->list.GetCount()));
}

bool ListServer::ParseLine(const wxString& line) {
	Server* s = new Server;
	s->serverPort = 5154;

	wxStringTokenizer tok(line, _T(" "));

	int i = 0;
	while(tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();
		switch(i) {
		case 0:	{ // hostname:port 
				int pos = token.Find(_T(":"));
				if( pos == wxNOT_FOUND ) {
					s->serverHost = token;
				}
				else {
					s->serverHost = token.Mid(0,pos);
					token.Mid(pos+1).ToLong(reinterpret_cast<long int*>(&s->serverPort));
				}
			}
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

	this->list.Append(s);

	return true;
}

bool ListServer::GetListServerResponse() {
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

void ListServer::ClearList() {
	ListServerList::iterator i;
	for(i = this->list.begin(); i != this->list.end(); ++i) {
		Server*	current = *i;

		delete current;
	}

	this->list.Clear();
}
