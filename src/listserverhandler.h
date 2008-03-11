#ifndef __listserver_handler__
#define __listserver_handler__

#include <wx/string.h>
#include <wx/list.h>
#include <wx/filesys.h>

#include "server.h"
#include "serverping.h"

WX_DECLARE_LIST(Server, ServerList);
/// Performs the listserver interaction - mainly parsing the data
/// received from i mainly parsing the data
class ListServerHandler {
private:
	bool GetListServerResponse();
	bool ParseLine(const wxString&);

	void ClearList();

	wxString	rawResponse;

	ServerPingList	pingList;

public:
	ServerList	serverList;

	ListServerHandler();
	~ListServerHandler();

	void GetServerList();
	Server* FindByName(const wxString&);
};



#endif
