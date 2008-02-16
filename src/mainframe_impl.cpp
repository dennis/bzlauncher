#include <wx/log.h>
#include "main.h"
#include "config.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"
#include "updatingdlg_impl.h"

#include "listserverhandler.h"

static int SortHelper(int res, bool reverse=false) {
	if(res==0) return res;
	if(reverse) return res*(-1);
	return res;
}

static int wxCALLBACK ServerSortCallback(long item1, long item2, long col) {
	Server* s1 = MainFrameImpl::GetServerByIdx(item1);
	Server* s2 = MainFrameImpl::GetServerByIdx(item2);
	
	// Make sure fav's are placed at the top, no matter what
	if(s1->favorite && s2->favorite) 
		return SortHelper(s1->serverHostPort.CmpNoCase(s2->serverHostPort),false);
	if(s1->favorite) return -1;
	if(s2->favorite) return 1;

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
			break;
		default:
			return 0;
			break;
	}
	return -1;
}

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent )  { 
	this->SetSize(this->DetermineFrameSize());
	this->SetupColumns();
	this->m_currentSortMode = appConfig.getSortMode();
	this->favoriteServers   = appConfig.getFavorites();
	this->serverList->SetFocus();
}

MainFrameImpl::~MainFrameImpl() {
#warning "Fix me"
	//appConfig.setFavorites(this->favoriteServers);
	//this->StoreColumnSizes();
	//appConfig.setWindowDimensions(GetRect());
	//appConfig.setSortMode(this->m_currentSortMode);
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

void MainFrameImpl::StoreColumnSizes() const {
	for(int col = 0; col < Config::COL_COUNT; col++)
		appConfig.setColumnWidth(Config::ColType(col), this->serverList->GetColumnWidth(col));
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
		list->SetItemData(idx,idx);

		// Name
		list->SetItem(idx, 1, current->name);

		// Type
		list->SetItem(idx, 2, current->GetType());

		// Player Count
		list->SetItem(idx, 3, wxString::Format(_T("%d"), current->GetPlayerCount()));
		
		// Ping
		list->SetItem(idx, 4, _("n/a"));

		// Color
		if(!current->fullyParsed)
			list->SetItemTextColour(idx, *wxRED);
		else if(current->IsFull())
			list->SetItemTextColour(idx, *wxBLUE);
		else if(current->IsEmpty())
			list->SetItemTextColour(idx, *wxLIGHT_GREY);
		else
			list->SetItemTextColour(idx, *wxBLACK);
		
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
	const wxString s = this->GetServerByIdx(event.GetData())->serverHostPort;
	wxGetApp().SetSelectedServer(s);
}

Server* MainFrameImpl::GetServerByIdx(int idx) {
	return wxGetApp().listServerHandler.serverList.Item(idx)->GetData();
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
	app.LaunchSelectedServer();
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
		//Not used: list->SetItemImage(idx, 5 , this->imgFavIdx);
		this->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ));
		this->serverList->SetItem(idx, 5, _T("Yes"));
	}
	else {
		this->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ));
		this->serverList->SetItem(idx, 5, _T("No"));
	}
}

void MainFrameImpl::EventPingServer(wxCommandEvent& WXUNUSED(event)) {
	wxGetApp().SetStatusText(_("Not implemented yet"));
}
