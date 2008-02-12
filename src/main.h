#include <wx/wx.h>
#include <wx/config.h>
#include <wx/fs_inet.h>

#include "listserverhandler.h"

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*		mainFrame;
	wxString 			selectedServerHostPort;
	wxString			iconsDirectory;

public:
	ListServerHandler	listServerHandler;

	virtual bool OnInit();

	~BZLauncherApp();

	void SetStatusText(const wxString&);
	void RefreshServerList();

	const wxString GetSelectedServer();
	void SetSelectedServer(const wxString&);

	void LaunchSelectedServer();

	const wxString getIconsDirectory() {
		return this->iconsDirectory;
	}
};


DECLARE_APP(BZLauncherApp);

