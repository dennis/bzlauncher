#include <wx/wx.h>

#include "listserver.h"

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*	mainFrame;
	Server* selectedServer;

public:
	ListServer	list;

	virtual bool OnInit();

	void SetStatusText(const wxString&);
	void RefreshServerList();

	Server* GetSelectedServer();
	void SetSelectedServer(Server*);
};


DECLARE_APP(BZLauncherApp);

