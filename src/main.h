#include <wx/wx.h>

#include "listserverhandler.h"

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*		mainFrame;
	Server* 			selectedServer;

public:
	ListServerHandler	listServerHandler;

	virtual bool OnInit();

	void SetStatusText(const wxString&);
	void RefreshServerList();

	Server* GetSelectedServer();
	void SetSelectedServer(Server*);

	void LaunchSelectedServer();
};


DECLARE_APP(BZLauncherApp);

