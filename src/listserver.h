#ifndef __listserver__
#define __listserver__

#include <wx/string.h>

class ListServer {
private:
	bool GetListServerResponse();

	wxString	rawResponse; 
	
public:
	ListServer() {};

	void GetServerList();
};

#endif
