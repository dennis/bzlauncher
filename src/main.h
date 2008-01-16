#include <wx/wx.h>

#include "listserver.h"

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*	mainFrame;

public:
	ListServer	list;

	virtual bool OnInit();

	void SetStatusText(const wxString&);
	void RefreshServerList();
};


DECLARE_APP(BZLauncherApp);

