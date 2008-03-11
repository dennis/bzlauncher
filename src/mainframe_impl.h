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
	void EventViewServer(wxCommandEvent&);
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
};

#endif 
