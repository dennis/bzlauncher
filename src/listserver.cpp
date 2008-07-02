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
#include <wx/tokenzr.h>
#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/filesys.h>

#include "listserver.h"

ListServer::ListServer(DataController* _ctrl, const wxArrayString& _s) : DataSource(_ctrl), listServers(_s) {
	this->server           = new Label(_("Server"));
	this->protocolVersion  = new Label(_("Protocol Version"));
	this->text             = new Label(_("Text"));

	// Ownership is transferred to ctrl, so we dont need to free them
	this->ctrl->addLabel(_T("server"),           this->server );
	this->ctrl->addLabel(_T("protocol_version"), this->protocolVersion );
	this->ctrl->addLabel(_T("text"),             this->text );
}

ListServer::~ListServer() {
}

ListServer::ExitCode ListServer::Entry() {
	this->GetServerList();
	while(!this->TestDestroy()) {
		wxSleep(1);
	}
	return 0;
}

void ListServer::GetServerList() {
	int count = 0;
	if( this->GetListServerResponse() ) {
		wxLogDebug(_T("[%lx] Parsing data from list-server..."), this->GetId());
		wxStringTokenizer tok(this->rawResponse, _T("\r\n"));

		while(tok.HasMoreTokens()) {	
			count++;
			wxString token = tok.GetNextToken();
			this->ParseLine(token);
		}
	}
	else {
		wxLogError(_("Can't connect to listserver!"));
	}
	wxLogDebug(_T("Found %d server(s)"), count);
}

bool ListServer::GetListServerResponse() {
	wxFileSystem	fs;
	wxFSFile*		listserv = NULL;
	bool			r = false;

	for(unsigned int c = 0; c < this->listServers.GetCount() && listserv == NULL; ++c)
		listserv = fs.OpenFile(this->listServers[c]);

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

bool ListServer::ParseLine(const wxString& line) {
	wxString name;
	wxStringTokenizer tok(line, _T(" "));

	int i = 0;
	while(tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();
		switch(i) {
		case 0:	{ // servername:port
			name = token;
			this->ctrl->updateAttribute(name, this->server, Attribute<wxString>(name));
			}
			break;
		case 1:  // version 
			this->ctrl->updateAttribute(name, this->protocolVersion, Attribute<wxString>(token));
			break;
		case 2:  // Hex info
			/*
			{
				ServerHexParser* p = ServerHexParser::GetParser(s->protocolVersion);
				if(p) {
					p->parse(token,*s);
					delete p;
				}
			}
			*/
			break;
		case 3: 
			/*
			{
				wxIPV4address ip;
				ip.Hostname(token);
				ip.Service(port);
				s->setIP(ip);
				s->longName.value += tok.GetString();
			}
			*/
			this->ctrl->updateAttribute(name, this->text, Attribute<wxString>(tok.GetString()));
			break;
		}
		i++;
	}

	return true;
}
