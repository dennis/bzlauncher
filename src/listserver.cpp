#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/tokenzr.h>

#include "listserver.h"

void ListServer::GetServerList() {
	if( this->GetListServerResponse() ) {
		wxStringTokenizer tok(this->rawResponse, _T("\r\n"));

		while(tok.HasMoreTokens()) {
			wxString token = tok.GetNextToken();

			wxLogMessage(token);
		}
	}
	else {
		wxLogError(_("Can't connect to listserver!"));
	}
}

bool ListServer::GetListServerResponse() {
	wxURL listserv(wxT("http://my.bzflag.org/db?action=LIST"));

	if(listserv.IsOk()) {
		wxInputStream *in_stream;
		in_stream = listserv.GetInputStream();
		char		buffer[1024];

		this->rawResponse.Clear();

		// Read listserver response
		while(!in_stream->Read(buffer,1024).Eof()) 
			this->rawResponse << wxString::From8BitData(buffer,in_stream->LastRead());

		return true;
	}

	return false;
}
