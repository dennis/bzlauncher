#include <wx/log.h>
#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"

#include "listserverhandler.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {
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

		// TODO: Colorization missing
	
		// Server
		list->InsertItem(idx, current->serverHostPort);

		// Name
		list->SetItem(idx, 1, current->name);

		// Type
		if( current->IsCTF() )
			list->SetItem(idx, 2, _("CTF"));
		else if( current->IsFFA() )
			list->SetItem(idx, 2, _("FFA"));
		else if( current->IsRH() )
			list->SetItem(idx, 2, _("RH"));
		else
			list->SetItem(idx, 2, _("n/a"));

		// Player Count
		list->SetItem(idx, 3, wxString::Format(_T("%d"), current->GetPlayerCount()));
		
		// Ping
		list->SetItem(idx, 4, _("n/a"));

		idx++;
	}
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
	int idx = event.m_itemIndex;
	BZLauncherApp& app = wxGetApp();
	const wxString s = app.listServerHandler.serverList.Item(idx)->GetData()->serverHostPort;
	app.SetSelectedServer(s);
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
