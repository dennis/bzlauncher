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
#include "view.h"
#include "query.h"

/// Main Window - presenting the servers online.
/// This is the implementation of MainFrame (which is generated)
/// this allows us to change how MainFrame is implemented without
/// needing to reimplement everything the MainFrame class is updated.
class MainFrameImpl : public MainFrame {
private:
	wxArrayString	favoriteServers;

	void SetupColumns(ServerListView*);
	wxRect DetermineFrameSize() const;

	void UpdateServer(int idx, Server*);

	wxTimer			initialLoadTimer;
	ServerPingTrackerTimer	pingTimer;

	bool			filterEnabled;

	viewlist_t		viewList;

	ServerListView*	activeView;

	void SetupViews();

	void AddView(ServerListView*);
	void AddViewAsTab(ServerListView*);
	void AddViewAsFull(ServerListView*);

	void RemoveView(ServerListView*);

	void ViewConnect(ServerListView*);
	void ViewDisconnect(ServerListView*);

protected:
	wxImageList*	imageList;
	int				imgFavIdx;

	void EventActivated(wxListEvent&);
	void EventColClick(wxListEvent&);
	void EventFavoriteToggle(wxCommandEvent&);
	void EventLaunch(wxCommandEvent&);
	void EventPingChanged(wxCommandEvent&);
	void EventPingServer(wxCommandEvent&);
	void EventQuit(wxCommandEvent&);
	void EventRefresh(wxCommandEvent&);
	void EventRightClick(wxListEvent&);
	void EventSearch(wxCommandEvent&);
	void EventSearchText(wxCommandEvent&);

	void EventSelectServer(wxListEvent&);
	void EventShowAbout(wxCommandEvent&);
	void EventTimer(wxTimerEvent&);
	void OnViewChangeEvent(wxNotebookEvent&);

	void RefreshActiveView();

public:
	MainFrameImpl(wxWindow*);
	~MainFrameImpl();

	void SetStatusText(const wxString&);

	void LaunchGame();
	void ShowDetails();
};

#endif 
