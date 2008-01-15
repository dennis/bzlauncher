#ifndef __mainframe_impl__
#define __mainframe_impl__

#include "gui.h"

class MainFrameImpl : public MainFrame {
protected:
	void EventRefresh(wxCommandEvent&);
	void EventShowAbout(wxCommandEvent&);


public:
	MainFrameImpl(wxWindow*);

	void SetStatusText(const wxString&);
};

#endif 
