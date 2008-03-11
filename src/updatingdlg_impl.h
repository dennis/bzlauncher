#ifndef __updatingdlg_impl__
#define __updatingdlg_impl__

#include "gui.h"

/// Small window to indicate to the user, that BZLauncher is busy updating
/// This is the implementation of UpdatingDlg (which is generated)
/// this allows us to change how UpdatingDlg is implemented without
/// needing to reimplement everything the UpdatingDlg class is updated.
class UpdatingDlgImpl : public UpdatingDlg
{
public:
	UpdatingDlgImpl(wxWindow*);
	void Pulse();
};

#endif 
