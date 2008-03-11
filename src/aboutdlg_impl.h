#ifndef __aboutdlg_impl__
#define __aboutdlg_impl__

#include "gui.h"

/// Show author and some info
/// This is the implementation of AboutDlg (which is generated)
/// this allows us to change how AboutDlg is implemented without
/// needing to reimplement everything the AboutDlg class is updated.
class AboutDlgImpl : public AboutDlg
{
protected:
	void EventClose(wxCommandEvent&);

public:
	AboutDlgImpl(wxWindow*);

};

#endif 
