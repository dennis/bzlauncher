/*
The MIT License

Copyright (c) 2008 Dennis Møllegaard Pedersen <dennis@moellegaard.dk>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef __mainframe_impl__
#define __mainframe_impl__

#include <wx/timer.h>
#include <wx/imaglist.h>
#include <wx/arrstr.h>
#include "gui.h"

#include "serverping.h"

/// Main Window - presenting the servers online.
/// This is the implementation of MainFrame (which is generated)
/// this allows us to change how MainFrame is implemented without
/// needing to reimplement everything the MainFrame class is updated.
class MainFrameImpl : public MainFrame {
private:

	long currentSortMode; // = colnum+1. If pos then asending, if neg then decending sort

	wxArrayString	favoriteServers;

	void SetupColumns();
	wxRect DetermineFrameSize() const;

	void UpdateServer(int idx, Server*);

	wxTimer			initialLoadTimer;
	ServerPingTrackerTimer	pingTimer;

protected:
	wxImageList*	imageList;
	int				imgFavIdx;

	void EventQuit(wxCommandEvent&);
	void EventRefresh(wxCommandEvent&);
	void EventLaunch(wxCommandEvent&);
	void EventActivated(wxListEvent&);
	void EventRightClick(wxListEvent&);
	void EventSelectServer(wxListEvent&);
	void EventColClick(wxListEvent&);
	void EventShowAbout(wxCommandEvent&);
	void EventFavoriteToggle(wxCommandEvent&);
	void EventPingServer(wxCommandEvent&);
	void EventPingChanged(wxCommandEvent&);
	void EventTimer(wxTimerEvent&);

public:
	MainFrameImpl(wxWindow*);
	~MainFrameImpl();

	void SetStatusText(const wxString&);
	void RefreshServerGrid();

	void LaunchGame();
	void ShowDetails();
};

#endif 
