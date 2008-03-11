#ifndef __serverdlg_impl__
#define __serverdlg_impl__

#include "server.h"
#include "gui.h"

/// Server details - view the details of a single server
/// This is the implementation of ServerDlg (which is generated)
/// this allows us to change how ServerDlg is implemented without
/// needing to reimplement everything the ServerDlg class is updated.
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
