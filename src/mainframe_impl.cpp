#include "main.h"
#include "config.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"
#include "updatingdlg_impl.h"

#include "listserverhandler.h"

#include "serverping.h"

static int SortHelper(int res, bool reverse=false) {
	if(res==0) return res;
	if(reverse) return res*(-1);
	return res;
}

static int wxCALLBACK ServerSortCallback(long item1, long item2, long col) {
	Server* s1 = reinterpret_cast<Server*>(item1);
	Server* s2 = reinterpret_cast<Server*>(item2);
	
	// Make sure fav's are placed at the top, no matter what
	if(s1->favorite && !s2->favorite) return -1;
	if(s2->favorite && !s1->favorite) return 1;

	bool ascending = (col<0);

	col = abs(col);

	switch(col) {
		case 1: // ServerHostPort
			return SortHelper(s1->serverHostPort.CmpNoCase(s2->serverHostPort),ascending);
			break;
		case 2: // Name
			return SortHelper(s1->name.CmpNoCase(s2->name),ascending);
			break;
		case 3:	// Type
			return SortHelper(s1->GetType().CmpNoCase(s2->GetType()),ascending);
			break;
		case 4: // Players
			{
			int r=0;
			if(s1->GetPlayerCount() < s2->GetPlayerCount())
				r = -1;
			else if(s1->GetPlayerCount() > s2->GetPlayerCount())
				r = 1;
			return SortHelper(r,ascending);
			}
			break;
		case 5: // Ping
			{
			int r = 0;
			if(s1->ping.getDuration() < s2->ping.getDuration())
				r = -1;
			else if(s1->ping.getDuration() > s2->ping.getDuration())
				r = -1;
			return SortHelper(r,ascending);
			}
			break;
		default:
			return 0;
			break;
	}
	return -1;
}

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ), initialLoadTimer(this)  { 
	ServerPingTracker::receiver = this;
	this->toolBar->SetToolBitmapSize(wxSize(32,32));
	this->toolBar->Realize();
	this->SetSize(this->DetermineFrameSize());
	this->SetupColumns();
	this->m_currentSortMode = appConfig.getSortMode();
	this->favoriteServers   = appConfig.getFavorites();
	this->serverList->SetFocus();

	this->Connect( this->initialLoadTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrameImpl::EventTimer));
	this->Connect(  wxID_ANY, wxEVT_PING_CHANGED, wxCommandEventHandler(MainFrameImpl::EventPingChanged));
	this->initialLoadTimer.Start(300,true);

	this->pingTimer.Start(10);
}

MainFrameImpl::~MainFrameImpl() {
	for(int col = 0; col < Config::COL_COUNT; col++)
		appConfig.setColumnWidth(Config::ColType(col), this->serverList->GetColumnWidth(col));
	appConfig.setFavorites(this->favoriteServers);
	appConfig.setWindowDimensions(GetRect());
	appConfig.setSortMode(this->m_currentSortMode);
}

void MainFrameImpl::SetupColumns() {
	wxString names[] = {
		_("Server"), _("Name"), _("Type"),
		_("#"), _("Ping"), _("Fav")
	};

	for(int col = 0; col < Config::COL_COUNT; col++) {
		this->serverList->InsertColumn(col,names[col]);
		this->serverList->SetColumnWidth(col,appConfig.getColumnWidth(Config::ColType(col)));
	}
}

wxRect MainFrameImpl::DetermineFrameSize() const {
	const int minFrameW = 300;
	const int minFrameH = 300;

	wxSize scr = wxGetDisplaySize();
	wxRect wanted = appConfig.getWindowDimensions();

	// Check values
	wanted.x = wxMin(abs(wanted.x), scr.x - minFrameW);
	wanted.y = wxMin(abs(wanted.y), scr.y - minFrameH);
	wanted.height = wxMax(wanted.height, minFrameH);
	wanted.height = wxMin(wanted.height, scr.x - wanted.x);
	wanted.width = wxMax(wanted.width, minFrameW);
	wanted.width = wxMin(wanted.width, scr.y - wanted.y);
	return wanted;
}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}

void MainFrameImpl::EventRefresh(wxCommandEvent&) {
	this->RefreshServerGrid();
}

void MainFrameImpl::RefreshServerGrid() {
	UpdatingDlgImpl dlg(this);
	dlg.Show();

	BZLauncherApp& app = wxGetApp();

	app.RefreshServerList();

	wxListCtrl* list = this->serverList;

	list->DeleteAllItems();

	// Content
	ServerList::iterator i;

	int idx = 0;
	for(i = app.listServerHandler.serverList.begin(); i != app.listServerHandler.serverList.end(); ++i) {
		dlg.Pulse();
		Server*	current = *i;

		// Check if its a favorite
		current->favorite = (this->favoriteServers.Index(current->serverHostPort) != wxNOT_FOUND);
	
		// Server
		list->InsertItem(idx, current->serverHostPort);
		list->SetItemPtrData(idx,reinterpret_cast<wxUIntPtr>(current));

		// Name
		list->SetItem(idx, 1, current->name);

		// Type
		list->SetItem(idx, 2, current->GetType());

		// Player Count
		list->SetItem(idx, 3, wxString::Format(_T("%d"), current->GetPlayerCount()));
		
		// Ping
		list->SetItem(idx, 4, _("n/a"));

		// Favorite
		this->UpdateServer(idx,current);

		idx++;
	}
	
	dlg.Pulse();
	this->serverList->SortItems(ServerSortCallback, this->m_currentSortMode);
	dlg.Pulse();
}

void MainFrameImpl::EventShowAbout(wxCommandEvent&) {
	AboutDlgImpl dlg(this);
	dlg.ShowModal();
}

void MainFrameImpl::EventQuit(wxCommandEvent&) {
	this->Close();
}

void MainFrameImpl::EventViewServer(wxCommandEvent&) {
	BZLauncherApp& app = wxGetApp();
	Server* s = app.listServerHandler.FindByName(app.GetSelectedServer());
	if(s) {
		ServerDlgImpl dlg(this, s);
		dlg.ShowModal();
	}
	else {
		app.SetStatusText(_("No server selected"));
	}
}

void MainFrameImpl::EventSelectServer(wxListEvent& event) {
	Server* srv = reinterpret_cast<Server*>(event.GetData());
	const wxString s = srv->serverHostPort;
	wxGetApp().SetSelectedServer(s);
}


void MainFrameImpl::EventRightClick(wxListEvent& WXUNUSED(event)) {
	this->PopupMenu(this->serverMenu);
}

void MainFrameImpl::EventActivated(wxListEvent& WXUNUSED(event)) {
	this->LaunchGame();
}

void MainFrameImpl::EventLaunch(wxCommandEvent& WXUNUSED(event)) {
	this->LaunchGame();
}

void MainFrameImpl::LaunchGame() {
	BZLauncherApp& app = wxGetApp();
	app.LaunchSelectedServer(this);
}

void MainFrameImpl::EventColClick(wxListEvent& event) {
	if(abs(this->m_currentSortMode) == (event.GetColumn()+1))
		this->m_currentSortMode *= -1;
	else
		this->m_currentSortMode = event.GetColumn()+1;
	this->serverList->SortItems(ServerSortCallback, this->m_currentSortMode);
}

void MainFrameImpl::EventFavoriteToggle(wxCommandEvent& WXUNUSED(event)) {
	BZLauncherApp& app = wxGetApp();
	Server* s = app.listServerHandler.FindByName(app.GetSelectedServer());
	if(s) {
		if(s->favorite) {
			// Remove from favoriteServers
			s->favorite = false;
			this->favoriteServers.Remove(s->serverHostPort);
		}
		else {
			// Add to favoriteServers
			s->favorite = true;
			this->favoriteServers.Add(s->serverHostPort);
		}
		
		int idx = this->serverList->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if(idx==-1) {
			app.SetStatusText(_("Couldnt find row! :("));
		}
		else {
			this->UpdateServer(idx,s);
			this->serverList->SortItems(ServerSortCallback, this->m_currentSortMode);
		}
	}
	else {
		app.SetStatusText(_("No server selected"));
	}
}

void MainFrameImpl::UpdateServer(int idx, Server* s) {
	if(s->favorite) {
		this->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ));
		this->serverList->SetItem(idx, 5, _("Yes"));
	}
	else {
		this->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ));
		this->serverList->SetItem(idx, 5, _("No"));
	}

	if(s->ping.isOK()) {
		this->serverList->SetItem(idx, 4, wxString::Format(_T("%d"), s->ping.getDuration()));
	}
	else {
		this->serverList->SetItem(idx, 4, _("n/a"));
	}
}

void MainFrameImpl::EventPingServer(wxCommandEvent& WXUNUSED(event)) {
	wxGetApp().SetStatusText(_("Not implemented yet"));
}

void MainFrameImpl::EventTimer(wxTimerEvent& WXUNUSED(event)) {
	this->RefreshServerGrid();
}

void MainFrameImpl::EventPingChanged(wxCommandEvent& event) {
	wxIPV4address	ip;

	long port = 5154;
	int pos;
	if((pos = event.GetString().Find(':',true)) != wxNOT_FOUND) {
		if(!event.GetString().Mid(pos+1).ToLong(&port))
			port = 5154;
		ip.Hostname(event.GetString().Mid(0,pos));
	}
	else {
		ip.Hostname(event.GetString());
	}
	ip.Service(port);

	long item = -1;
	for ( ;; ) {
		item = this->serverList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);

		if(item == -1)
			break;

		Server* current = reinterpret_cast<Server*>(this->serverList->GetItemData(item));

		if(current->ip.IPAddress() == ip.IPAddress() && current->ip.Service() && ip.Service()) {
			this->UpdateServer(item, current);
		}
	}
}
