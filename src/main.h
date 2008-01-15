#include <wx/wx.h>

#include "mainframe_impl.h"

class BZLauncherApp : public wxApp {
private:
	MainFrameImpl*	mainFrame;

public:
	virtual bool OnInit();

	void SetStatusText(const wxString&);
};


DECLARE_APP(BZLauncherApp);

