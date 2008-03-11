#include <wx/wx.h>

#include "listserverhandler.h"

class MainFrameImpl;

/// Starting point of BZLauncher 
class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*		mainFrame;
	wxString 			selectedServerHostPort;

public:
	ListServerHandler	listServerHandler;

	virtual bool OnInit();

	~BZLauncherApp();

	void SetStatusText(const wxString&);
	void RefreshServerList();

	const wxString GetSelectedServer();
	void SetSelectedServer(const wxString&);

	void LaunchSelectedServer(wxWindow*);
};


DECLARE_APP(BZLauncherApp);

