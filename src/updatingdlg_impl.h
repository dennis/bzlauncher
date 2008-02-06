#ifndef __updatingdlg_impl__
#define __updatingdlg_impl__

#include "gui.h"

class UpdatingDlgImpl : public UpdatingDlg
{
public:
	UpdatingDlgImpl(wxWindow*);
	void Pulse();
};

#endif 
