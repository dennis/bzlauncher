#ifndef __mainframe_impl__
#define __mainframe_impl__

#include <wx/imaglist.h>
#include <wx/arrstr.h>
#include "gui.h"

class MainFrameImpl : public MainFrame {
private:

	long m_currentSortMode; // = colnum+1. If pos then asending, if neg then decending sort

	wxArrayString	favoriteServers;

	void SetupColumns();
	wxRect DetermineFrameSize() const;

	void StoreColumnSizes() const;

	void UpdateServer(int idx, Server*);

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

public:
	static Server* GetServerByIdx(int);

	MainFrameImpl(wxWindow*);
	~MainFrameImpl();

	void SetStatusText(const wxString&);
	void RefreshServerGrid();

	void LaunchGame();
};

#endif 
