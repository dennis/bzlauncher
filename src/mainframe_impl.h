#ifndef __mainframe_impl__
#define __mainframe_impl__

#include "gui.h"

class MainFrameImpl : public MainFrame {
protected:
	void EventQuit(wxCommandEvent&);
	void EventRefresh(wxCommandEvent&);
	void EventLaunch(wxCommandEvent&);
	void EventLeftDClick(wxGridEvent&);
	void EventRightClick(wxGridEvent&);
	void EventSelectServer(wxGridEvent&);
	void EventShowAbout(wxCommandEvent&);
	void EventViewServer(wxCommandEvent&);
	void EventKeyUp(wxKeyEvent&);

public:
	MainFrameImpl(wxWindow*);

	void SetStatusText(const wxString&);
	void RefreshServerGrid();

	void LaunchGame();
};

#endif 
