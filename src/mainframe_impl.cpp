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
#include <algorithm>
#include <wx/uri.h>

#include "main.h"
#include "config.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"
#include "updatingdlg_impl.h"

#include "listserverhandler.h"

#include "ping.h"
#include "datactrl.h"

static int SortHelper(int res, bool reverse=false) {
	if(res==0) return res;
	if(reverse) return res*(-1);
	return res;
}

static int wxCALLBACK ServerSortCallback(long item1, long item2, long col) {
	return 0;
	/*
	Server* s1 = reinterpret_cast<Server*>(item1);
	Server* s2 = reinterpret_cast<Server*>(item2);

	if(!s1 || !s2) 
		return 0;
	
	// Make sure fav's are placed at the top, no matter what
	if(s1->favorite && !s2->favorite) return -1;
	if(s2->favorite && !s1->favorite) return 1;

	bool ascending = (col<0);

	col = abs(col);

	switch(col) {
		case 1: // ServerHostPort
			return SortHelper(s1->name().CmpNoCase(s2->name()),ascending);
			break;
		case 2: // Name
			return SortHelper(s1->longName().CmpNoCase(s2->longName()),ascending);
			break;
		case 3:	// Type
			return SortHelper(s1->gameType().CmpNoCase(s2->gameType()),ascending);
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
			if(s1->ping.value.getDuration() < s2->ping.value.getDuration())
				r = -1;
			else if(s1->ping.value.getDuration() > s2->ping.value.getDuration())
				r = 1;
			return SortHelper(r,ascending);
			}
			break;
		default:
			return 0;
			break;
	}
	return -1;
	*/
}

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ), initialLoadTimer(this), filterEnabled(false), activeView(NULL)  { 
	PingTracker::receiver = this;
	this->toolBar->SetToolBitmapSize(wxSize(32,32));
	this->toolBar->Realize();

	this->favoriteServers   = appConfig.getFavorites();
	this->recentServers 	= appConfig.getRecentServers();

	this->Connect( this->initialLoadTimer.GetId(), wxEVT_TIMER, wxTimerEventHandler(MainFrameImpl::EventTimer));
	this->Connect(  wxID_ANY, wxEVT_PING_CHANGED, wxCommandEventHandler(MainFrameImpl::EventPingChanged));

	this->findPanel->Show(false);

	this->SetupViews();

	this->SetSize(this->DetermineFrameSize());
	this->toolBar->Show(appConfig.getToolbarVisible());
	this->initialLoadTimer.Start(300,true);

	this->activeView->serverList->SetFocus();
}

MainFrameImpl::~MainFrameImpl() {
	// Update the labels with the values from the gui (the width)
	// This assumes that the Column order havn't been changed!
	std::vector<Label*>	columns = this->GetViewableLabels();
	int col = 0;
	for(std::vector<Label*>::iterator i = columns.begin(); i != columns.end(); ++i) {
		wxLogDebug(_T("Column: #%d %s (%d)"), col, (*i)->getName().c_str(), (*i)->getWidth());
		(*i)->setWidth(this->activeView->serverList->GetColumnWidth(col));
		col++;
	}

	appConfig.setFavorites(this->favoriteServers);
	appConfig.setRecentServers(this->recentServers);
	appConfig.setWindowDimensions(GetRect());

	appConfig.setViews(this->viewList);
	appConfig.setToolbarVisible(this->toolBar->IsShown());

	this->toolBar->Show(appConfig.getToolbarVisible());

	for(viewlist_t::iterator i = this->viewList.begin(); i != this->viewList.end(); ++i )
		delete *i;
}

void MainFrameImpl::AddView(ServerListView* view) {
	this->viewList.push_back(view);

	this->AddViewAsTab(view);

	view->serverList->Layout();
	this->tabs->Layout();
}

void MainFrameImpl::RemoveView(ServerListView* view) {
	// Ignore the delete if we only got one view
	if(this->viewList.size()==1) {
		return;
	}

	viewlist_t::iterator i = find(this->viewList.begin(), this->viewList.end(), view);
	if(i == this->viewList.end())
		return;

	this->viewList.erase(i);
	this->ViewDisconnect(view);

	delete view;

	this->activeView = NULL; // FIXME
}

void MainFrameImpl::AddViewAsTab(ServerListView* view) {
	wxFlexGridSizer* fgSizer;
	fgSizer = new wxFlexGridSizer( 1, 1, 0, 0 );
	fgSizer->AddGrowableCol(0);
	fgSizer->AddGrowableRow(0);
	fgSizer->SetFlexibleDirection( wxBOTH );
	fgSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxPanel* panel = new wxPanel( this->tabs, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

	if(! view->serverList ) {
		view->serverList = new wxListCtrl( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
		this->SetupColumns(view);
	}

	fgSizer->Add(view->serverList, 0, wxEXPAND, 0);

	panel->SetSizer(fgSizer);
	panel->Layout();
	fgSizer->Fit( panel );

	view->serverList->Layout();

	this->ViewConnect(view);
	this->tabs->AddPage( panel, view->GetName(), true );
}

void MainFrameImpl::ViewConnect(ServerListView* view) {
	view->serverList->Connect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( MainFrameImpl::EventColClick ), NULL, this );
	view->serverList->Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MainFrameImpl::EventRightClick ), NULL, this );
	view->serverList->Connect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrameImpl::EventActivated ), NULL, this );
	view->serverList->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrameImpl::EventSelectServer ), NULL, this );
}

void MainFrameImpl::ViewDisconnect(ServerListView* view) {
	view->serverList->Disconnect( wxEVT_COMMAND_LIST_COL_CLICK, wxListEventHandler( MainFrameImpl::EventColClick ), NULL, this );
	view->serverList->Disconnect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( MainFrameImpl::EventRightClick ), NULL, this );
	view->serverList->Disconnect( wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler( MainFrameImpl::EventActivated ), NULL, this );
	view->serverList->Disconnect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( MainFrameImpl::EventSelectServer ), NULL, this );
}


void MainFrameImpl::SetupViews() {
	this->viewList = appConfig.getViews();

	assert(this->viewList.size());

	this->SwitchView(this->viewList[0]);

	for(viewlist_t::iterator i = this->viewList.begin(); i != this->viewList.end(); ++i ) {
		this->AddViewAsTab((*i));
	}

	this->tabs->SetSelection(0);
	this->tabs->Layout();
	this->tabs->Show(true);
}

static bool ColumnSorter(Label* l1, Label* l2) {
	return l1->getPos() < l2->getPos();
}

std::vector<Label*> MainFrameImpl::GetViewableLabels() {
	BZLauncherApp& app = wxGetApp();
	// We copy the map to a vector, so we can sort it easily with std::sort (etc)
	std::vector<Label*>	columns;
	for(DataController::labelmap_t::iterator i = app.dataControl.labelMap.begin(); i != app.dataControl.labelMap.end(); ++i) {
		if( i->second->getPos() >= 0 ) 
			columns.push_back(i->second);
	}

	sort(columns.begin(), columns.end(), ColumnSorter);

	// If nothing is selected, add "server"
	if( columns.size() == 0 ) {
		columns.push_back(app.dataControl.labelMap[_T("server")]);
	}

	return columns;
}

void MainFrameImpl::SetupColumns(ServerListView *view) {
	std::vector<Label*>	columns = this->GetViewableLabels();

	int col = 0;
	for(std::vector<Label*>::iterator i = columns.begin(); i != columns.end(); ++i) {
		wxLogDebug(_T("Column: #%d %s (%d)"), col, (*i)->getName().c_str(), (*i)->getWidth());
		view->serverList->InsertColumn(col,(*i)->getName());
		view->serverList->SetColumnWidth(col,(*i)->getWidth());
		col++;
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
	wxGetApp().RefreshServerList(this->favoriteServers, this->recentServers);
	this->RefreshActiveView();
}

void MainFrameImpl::RefreshActiveView() {
	wxLogDebug(_T("RefreshActiveView()"));
	BZLauncherApp& app = wxGetApp();

	wxListCtrl* list = this->activeView->serverList;
	std::vector<Label*>	columns = this->GetViewableLabels();

	QueryResult res = app.dataControl.search(this->activeView->query);

	// For now, always re-created the list
	list->DeleteAllItems();
	for(int row = 0; row < res.size(); ++row ) {
		int col = 0;
		int idx = 0;
		for(std::vector<Label*>::iterator i  = columns.begin(); i != columns.end() ; ++i ) {
			const wxString val = res.getAttribute(row, *i);
			wxLogDebug(_T("Updating col %d label %s, row %d, %s"),
				col, (*i)->getName().c_str(), row, val.c_str());
			if( col == 0 )
				idx = list->InsertItem(col, val);
			else if(idx != -1)
				list->SetItem(idx, col, val);
			col++;
		}
	}

	/*
	if(app.listServerHandler.getVersion() == this->activeView->version) {
		return;
	}

	for(viewlist_t::iterator i = this->viewList.begin(); i != this->viewList.end(); ++i ) {
		if( (*i)->version < app.listServerHandler.getVersion())
			(*i)->serverList->DeleteAllItems();
	}

	// Content
	ServerResultSet::iterator i;
	ServerResultSet	resultSet = app.listServerHandler.Search(this->activeView->query);
	this->activeView->version = app.listServerHandler.getVersion();

	int idx = 0;
	for(i = resultSet.begin(); i != resultSet.end(); ++i) {
		Server*	current = *i;

		// Check if its a favorite and recent
		current->favorite = (this->favoriteServers.Index(current->name()) != wxNOT_FOUND);
		current->recent   = (this->recentServers.Index(current->name()) != wxNOT_FOUND);

		// Server
		list->InsertItem(idx, current->name());
		list->SetItemPtrData(idx,reinterpret_cast<wxUIntPtr>(current));

		// Name
		list->SetItem(idx, 1, current->longName);

		if(current->fullyParsed) {
			// Type
			list->SetItem(idx, 2, current->gameType());

			// Player Count
			list->SetItem(idx, 3, wxString::Format(_T("%d"), current->GetPlayerCount()));
		}
		else {
			// Type
			list->SetItem(idx, 2, _T("?"));

			// Player Count
			list->SetItem(idx, 3, _T("?"));
		}
		
		// Ping
		list->SetItem(idx, 4, _("..."));

		// Favorite
		this->UpdateServer(this->activeView, idx,current);

		// Use colors to show if servers are supported by bzlauncher(Red), full (Blue), empty (grey)
		if(!current->fullyParsed)
			list->SetItemTextColour(idx, *wxRED);
		else if(current->IsFull())
			list->SetItemTextColour(idx, *wxBLUE);
		else if(current->IsEmpty())
			list->SetItemTextColour(idx, *wxLIGHT_GREY);
		else
			list->SetItemTextColour(idx, *wxBLACK);

		idx++;
	}

	this->activeView->serverList->SortItems(ServerSortCallback, this->activeView->currentSortMode);
	*/
	this->activeView->serverList->Layout();
}

void MainFrameImpl::EventShowAbout(wxCommandEvent&) {
	AboutDlgImpl dlg(this);
	dlg.ShowModal();
}

void MainFrameImpl::EventQuit(wxCommandEvent&) {
	this->Close();
}

void MainFrameImpl::ShowDetails() {
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
	/*
	Server* srv = reinterpret_cast<Server*>(event.GetData());
	const wxString s = srv->name();
	wxGetApp().SetSelectedServer(s);
	*/
}


void MainFrameImpl::EventRightClick(wxListEvent& WXUNUSED(event)) {
	this->PopupMenu(this->serverMenu);
}

void MainFrameImpl::EventActivated(wxListEvent& WXUNUSED(event)) {
	this->ShowDetails();
}

void MainFrameImpl::EventLaunch(wxCommandEvent& WXUNUSED(event)) {
	this->ShowDetails();
}

void MainFrameImpl::EventColClick(wxListEvent& event) {
	if(abs(this->activeView->currentSortMode) == (event.GetColumn()+1))
		this->activeView->currentSortMode *= -1;
	else
		this->activeView->currentSortMode = event.GetColumn()+1;
	this->activeView->serverList->SortItems(ServerSortCallback, this->activeView->currentSortMode);
}

void MainFrameImpl::EventFavoriteToggle(wxCommandEvent& WXUNUSED(event)) {
	/*
	BZLauncherApp& app = wxGetApp();
	Server* s = app.listServerHandler.FindByName(app.GetSelectedServer());
	if(s) {
		if(s->favorite) {
			// Remove from favoriteServers
			s->favorite = false;
			this->favoriteServers.Remove(s->name());
		}
		else {
			// Add to favoriteServers
			s->favorite = true;
			this->favoriteServers.Add(s->name());
		}
		
		int idx = this->activeView->serverList->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if(idx==-1) {
			app.SetStatusText(_("Couldnt find row! :("));
		}
		else {
			this->UpdateServer(this->activeView,idx,s);
			this->activeView->serverList->SortItems(ServerSortCallback, this->activeView->currentSortMode);
		}
	}
	else {
		app.SetStatusText(_("No server selected"));
	}
	*/
}

void MainFrameImpl::UpdateServer(ServerListView* view, int idx, Server* s) {
	/*
	if(s->favorite) {
		view->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Sans") ));
		view->serverList->SetItem(idx, 5, _("Yes"));
	}
	else {
		view->serverList->SetItemFont(idx, wxFont( 8, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Sans") ));
		view->serverList->SetItem(idx, 5, _("No"));
	}

	view->serverList->SetItem(idx, 4, s->ping);
	*/
}

void MainFrameImpl::EventPingServer(wxCommandEvent& WXUNUSED(event)) {
	/*
	BZLauncherApp& app = wxGetApp();
	Server* s = app.listServerHandler.FindByName(app.GetSelectedServer());
	if(s) {
		s->ping.value.ping();
	}
	else {
		app.SetStatusText(_("No server selected"));
	}
	*/
}

void MainFrameImpl::EventTimer(wxTimerEvent& WXUNUSED(event)) {
	BZLauncherApp& app = wxGetApp();
	app.RefreshServerList(this->favoriteServers, this->recentServers);
	this->RefreshActiveView();
	
	// Check if we got an argument, if so, this might be
	//   bzlauncher://server:port
	//   bzflag://server:port
	//   - these will all show the server-dialog for the server (if found)
	if(app.argv[1]) {
		wxURI	arg(app.argv[1]);
		wxString	server = arg.GetServer();
		wxString	port   = arg.GetPort();

		if(port.IsEmpty()) 
			port = _T("5154");

		wxString	serverPort = wxString::Format(_T("%s:%s"), server.c_str(), port.c_str());

		if( !server.IsEmpty() && ( arg.GetScheme().CmpNoCase(_T("bzflag")) || arg.GetScheme().CmpNoCase(_T("bzlauncher")))) {
			wxGetApp().SetSelectedServer(serverPort);
			if(app.listServerHandler.FindByName(app.GetSelectedServer())) {
				ShowDetails();
			}
		}
		else {
			wxMessageBox(wxString::Format(_("Unknown argument: %s"), app.argv[1]), _("Error"), wxOK |wxICON_ERROR);
		}
	}
	
	this->pingTimer.Start(10);
}

void MainFrameImpl::EventPingChanged(wxCommandEvent& event) {
	// Make sure the list is up-to-date
	if(wxGetApp().listServerHandler.getVersion() != this->activeView->version) {
		return;
	}

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
	ServerListView* view= NULL;
	for(viewlist_t::iterator i = this->viewList.begin(); i != this->viewList.end(); ++i ) {
		view = *i;

		for ( ;; ) {
			item = view->serverList->GetNextItem(item, wxLIST_NEXT_ALL, wxLIST_STATE_DONTCARE);

			if(item == -1)
				break;

			Server* s = reinterpret_cast<Server*>(view->serverList->GetItemData(item));
			this->UpdateServer(view, item, s);
		}
	}
}

void MainFrameImpl::EventSearch(wxCommandEvent& WXUNUSED(event)) {
	this->filterEnabled = !this->filterEnabled;
	this->findPanel->Show(this->filterEnabled);

	if(this->filterEnabled) {
		this->queryText->SetValue(this->activeView->query.get());
		this->queryText->SetFocus();
	}
	else {
		this->activeView->serverList->SetFocus();
	}
	this->Layout();
}

void MainFrameImpl::EventSearchText(wxCommandEvent& WXUNUSED(event)) {
	ServerListView*	view = new ServerListView(Query(this->queryText->GetValue().c_str()), this->activeView->currentSortMode);
	this->AddView(view);
	this->SwitchView(view);
	this->RefreshActiveView();
}

void MainFrameImpl::EventViewChanged(wxAuiNotebookEvent& event) {
	int selected = event.GetSelection();
	if( selected >= 0 && selected < (int)this->viewList.size() ) {
		this->SwitchView(this->viewList[event.GetSelection()]);
		this->RefreshActiveView();
	}
}
void MainFrameImpl::EventToolbarToggle(wxCommandEvent& event) {
	this->toolBar->Show(event.IsChecked());
	this->Layout();
}

void MainFrameImpl::SwitchView(ServerListView* newView) {
	// Make sure the newView got same column-widths as the current
	if(this->activeView) {
		for(int col = 0; col < newView->serverList->GetColumnCount(); col++)
			newView->serverList->SetColumnWidth(col,this->activeView->serverList->GetColumnWidth(col));
	}
	
	this->activeView = newView;
}

void MainFrameImpl::AddAsRecentServer(const wxString& server) {
	const unsigned int max_recent = 20;
	
	if(this->recentServers.Index(server) != wxNOT_FOUND)
		this->recentServers.Remove(server);
	this->recentServers.Add(server);

	// Store max max_recent servers
	if( this->recentServers.Count() > max_recent ) {
		this->recentServers.RemoveAt(max_recent, this->recentServers.Count() - max_recent);
	}
}

void MainFrameImpl::EventViewClose(wxAuiNotebookEvent& event) {
	if(!this->CanCloseView(this->activeView)) {
		event.Veto();
	}
	else {
		this->RemoveView(this->activeView);
	}
}

void MainFrameImpl::EventCloseView(wxCommandEvent&) {
	if(this->CanCloseView(this->activeView)) {
		ServerListView*	deleting = this->activeView;
		this->RemoveView(deleting);
		this->tabs->RemovePage(this->tabs->GetSelection());
	}
}

bool MainFrameImpl::CanCloseView(ServerListView* view) {
	if(this->viewList.size()==1) {
		this->SetStatusText(_("I won't remove the last view"));
		return false;
	}
	
	// "All", and "Recent" are protected
	if( view->query.get().CmpNoCase(_T("All")) == 0 ||
		view->query.get().CmpNoCase(_T("Recent")) == 0 ) {
		wxMessageBox(_("'All' and 'Recent' are protected so don't delete them, please"), _("Hey"), wxICON_INFORMATION | wxOK);
		return false;
	}

	return true;
}
