#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"

#include "listserverhandler.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {
	this->serverGrid->SetSelectionMode(wxGrid::wxGridSelectRows);
}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}

void MainFrameImpl::EventRefresh(wxCommandEvent&) {
	BZLauncherApp& app = wxGetApp();

	app.RefreshServerList();

	wxGrid* grid;
	grid = this->serverGrid;

	// Clear list
	grid->BeginBatch();
	grid->DeleteRows(0,grid->GetNumberRows());
	grid->InsertRows(0,app.listServerHandler.serverList.GetCount());

	// Content
	ServerList::iterator i;
	int row = 0;
	int col = 0;
	for(i = app.listServerHandler.serverList.begin(); i != app.listServerHandler.serverList.end(); ++i) {
		Server*	current = *i;
		col = 0;

		// Favorites
		grid->SetCellValue(row, col, _T("n/a"));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxLIGHT_GREY);

		// Name
		col++;
		wxString s = current->serverHostPort;
		if( current->isCTF() ) s += _T(" CTF");
		else if( current->isFFA() ) s += _T(" FFA");
		else if( current->isRH() ) s += _T(" RH");
		grid->SetCellValue(row, col, s);
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxBLACK);

		// Type
		col++;
		grid->SetCellValue(row, col, current->name);
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxBLACK);

		// Players
		col++;
		grid->SetCellValue(row, col, wxString::Format(_T("%d"), current->getPlayerCount()));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxBLACK);

		// Ping
		col++;
		grid->SetCellValue(row, col, _T("n/a"));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxLIGHT_GREY);

		row++;
	}
	
	grid->EndBatch();
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
	ServerDlgImpl dlg(this, app.GetSelectedServer());
	dlg.ShowModal();
}

void MainFrameImpl::EventSelectServer(wxGridEvent& event) {
	BZLauncherApp& app = wxGetApp();
	app.SetSelectedServer((app.listServerHandler.serverList.Item(event.GetRow()))->GetData());
}

