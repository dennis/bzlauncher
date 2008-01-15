#include <wx/wx.h>

class MainFrameImpl;

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*	mainFrame;

public:
	virtual bool OnInit();

	void SetStatusText(const wxString&);
	void RefreshServerList();
};


DECLARE_APP(BZLauncherApp);

