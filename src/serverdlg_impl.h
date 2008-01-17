#ifndef __serverdlg_impl__
#define __serverdlg_impl__

#include "server.h"
#include "gui.h"

class ServerDlgImpl : public ServerDlg
{
private:
	Server*	server;
protected:
	void EventClose(wxCommandEvent&);


public:
	ServerDlgImpl(wxWindow*, Server*);
};

#endif 
