#ifndef __aboutdlg_impl__
#define __aboutdlg_impl__

#include "gui.h"

class AboutDlgImpl : public AboutDlg
{
protected:
	void EventClose(wxCommandEvent&);

public:
	AboutDlgImpl(wxWindow*);

};

#endif 
