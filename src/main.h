#include <wx/wx.h>

#include "gui.h"

class BZLauncherApp : public wxApp {
private:
	MainFrame*	mainFrame;

public:
	virtual bool OnInit();

	void SetStatusText(const wxString&);
};


DECLARE_APP(BZLauncherApp);

