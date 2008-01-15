#ifndef __listserver__
#define __listserver__

#include <wx/string.h>
#include <wx/list.h>

#include "server.h"

WX_DECLARE_LIST(Server, ListServerList);

class ListServer {
private:
	bool GetListServerResponse();
	bool ParseLine(const wxString&);

	wxString	rawResponse; 

public:
	ListServerList	list;

	ListServer() {};

	void GetServerList();
};



#endif
