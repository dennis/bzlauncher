#ifndef __listserver_handler__
#define __listserver_handler__

#include <wx/string.h>
#include <wx/list.h>

#include "server.h"

WX_DECLARE_LIST(Server, ServerList);

class ListServerHandler {
private:
	bool GetListServerResponse();
	bool ParseLine(const wxString&);

	void ClearList();

	wxString	rawResponse; 

public:
	ServerList	serverList;

	ListServerHandler() {};

	void GetServerList();
};



#endif
