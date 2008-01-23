#ifndef __mainframe_impl__
#define __mainframe_impl__

#include "gui.h"

class MainFrameImpl : public MainFrame {
protected:
	void EventChar(wxKeyEvent&);
	void EventQuit(wxCommandEvent&);
	void EventRefresh(wxCommandEvent&);
	void EventRightClick(wxGridEvent&);
	void EventSelectServer(wxGridEvent&);
	void EventShowAbout(wxCommandEvent&);
	void EventViewServer(wxCommandEvent&);

public:
	MainFrameImpl(wxWindow*);

	void SetStatusText(const wxString&);
	void RefreshServerGrid();
};

#endif 
