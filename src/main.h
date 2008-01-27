#include <wx/wx.h>
#include <wx/config.h>
#include "listserverhandler.h"

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*		mainFrame;
	wxString 			selectedServerHostPort;

public:
	ListServerHandler	listServerHandler;

	virtual bool OnInit();

	void SetStatusText(const wxString&);
	void RefreshServerList();

	const wxString GetSelectedServer();
	void SetSelectedServer(const wxString&);

	void LaunchSelectedServer();
};


DECLARE_APP(BZLauncherApp);

