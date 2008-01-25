#include <wx/log.h>
#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"

#include "listserverhandler.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ), m_currentSortMode(1) {
	this->serverList->SetFocus();
	int col = 0;
	this->serverList->InsertColumn(col,_("Server"));
	this->serverList->SetColumnWidth(col,157);
	col++;

	this->serverList->InsertColumn(col,_("Name"));
	this->serverList->SetColumnWidth(col,300);
	col++;

	this->serverList->InsertColumn(col,_("Type"));
	this->serverList->SetColumnWidth(col,47);
	col++;

	this->serverList->InsertColumn(col,_("#"));
	this->serverList->SetColumnWidth(col,30);
	col++;

	this->serverList->InsertColumn(col,_("Ping"));
	this->serverList->SetColumnWidth(col,46);
	col++;
}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}

void MainFrameImpl::EventRefresh(wxCommandEvent&) {
	this->RefreshServerGrid();
}

void MainFrameImpl::RefreshServerGrid() {
	BZLauncherApp& app = wxGetApp();

	app.RefreshServerList();

	wxListCtrl* list = this->serverList;

	list->DeleteAllItems();

	// Content
	ServerList::iterator i;

	int idx = 0;
	for(i = app.listServerHandler.serverList.begin(); i != app.listServerHandler.serverList.end(); ++i) {
		Server*	current = *i;
	
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

		idx++;
	}
	
	this->serverList->SortItems(MainFrameImpl::ServerSortCallback, this->m_currentSortMode);
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

static int SortHelper(int res, bool reverse=false) {
	if(res==0) return res;
	if(reverse) return res*(-1);
	return res;
}
int wxCALLBACK MainFrameImpl::ServerSortCallback(long item1, long item2, long col) {
	Server* s1 = MainFrameImpl::GetServerByIdx(item1);
	Server* s2 = MainFrameImpl::GetServerByIdx(item2);

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

void MainFrameImpl::EventColClick(wxListEvent& event) {
	if(abs(this->m_currentSortMode) == (event.GetColumn()+1))
		this->m_currentSortMode *= -1;
	else
		this->m_currentSortMode = event.GetColumn()+1;
	wxLogMessage(wxString::Format(_T("SortMode = %d, event = %d"), this->m_currentSortMode, event.GetColumn()));
	this->serverList->SortItems(MainFrameImpl::ServerSortCallback, this->m_currentSortMode);
}

