#ifndef __MainFrameImpl__
#define __MainFrameImpl__

#include "gui.h"

class MainFrameImpl : public MainFrame
{
public:
	MainFrameImpl( wxWindow* parent );

	void SetStatusText(const wxString&);
};

#endif 
