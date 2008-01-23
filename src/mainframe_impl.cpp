#include <wx/log.h>
#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"
#include "serverdlg_impl.h"

#include "listserverhandler.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {
	this->serverGrid->SetSelectionMode(wxGrid::wxGridSelectRows);
	this->serverGrid->SetFocus();
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

	const wxColor* serverColor;
	wxGrid* grid = this->serverGrid;

	// Clear list
	grid->BeginBatch();
	grid->DeleteRows(0,grid->GetNumberRows());
	grid->ClearSelection();
	grid->InsertRows(0,app.listServerHandler.serverList.GetCount());

	// Content
	ServerList::iterator i;
	int row = 0;
	int col = 0;
	for(i = app.listServerHandler.serverList.begin(); i != app.listServerHandler.serverList.end(); ++i) {
		Server*	current = *i;
		col = 0;

		serverColor = wxBLACK;
		if(current->IsFull())
			serverColor = wxRED;
		else if(current->IsEmpty()) 
			serverColor = wxLIGHT_GREY;

		// Server
		grid->SetCellValue(row, col, current->serverHostPort);
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *serverColor);
		col++;

		// Name
		grid->SetCellValue(row, col, current->name);
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *serverColor);
		col++;

		// Type
		if( current->IsCTF() )
			grid->SetCellValue(row, col, _("CTF"));
		else if( current->IsFFA() )
			grid->SetCellValue(row, col, _T("FFA"));
		else if( current->IsRH() )
			grid->SetCellValue(row, col, _T("RH"));
		else
			grid->SetCellValue(row, col, _T("n/a"));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *serverColor);
		col++;

		// Players
		grid->SetCellValue(row, col, wxString::Format(_T("%d"), current->GetPlayerCount()));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *serverColor);
		col++;

		// Ping
		grid->SetCellValue(row, col, _T("n/a"));
		grid->SetReadOnly(row, col);
		grid->SetCellTextColour(row, col, *wxLIGHT_GREY);
		col++;

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

void MainFrameImpl::EventKeyUp(wxKeyEvent& event) {
	switch(event.GetKeyCode()) {
/* - there must be a better way
		case WXK_UP: {
			int row = this->serverGrid->GetGridCursorRow();
			if( row > 0 ) row--;
			this->serverGrid->SetGridCursor(row,0);
			this->serverGrid->MakeCellVisible(row,0);
			}
			break;

		case WXK_DOWN: {
			int row = this->serverGrid->GetGridCursorRow();
			if( row+1 < this->serverGrid->GetNumberRows() ) row++;
			this->serverGrid->SetGridCursor(row,0);
			this->serverGrid->MakeCellVisible(row,0);
			}
			break;
			
		case WXK_LEFT:
		case WXK_UP:
		case '8':
			serverGrid->MoveCursorUp(false);
			break;
		case WXK_RIGHT:
		case WXK_DOWN:
		case '2':
			serverGrid->MoveCursorDown(false);
			break;
		case WXK_PAGEUP:
		case '9':
			serverGrid->MovePageUp();
			break;
		case '3':
		case WXK_PAGEDOWN:
			serverGrid->MovePageDown();
			break;
*/
		default:
			event.Skip();
	}
}
void MainFrameImpl::EventRightClick(wxGridEvent& event) {
	// Select server
	BZLauncherApp& app = wxGetApp();
	app.SetSelectedServer((app.listServerHandler.serverList.Item(event.GetRow()))->GetData());
	this->serverGrid->SelectRow(event.GetRow());
	
	// Show menu
	this->PopupMenu(this->serverMenu);
}

void MainFrameImpl::EventLeftDClick(wxGridEvent& event) {
	this->LaunchGame();
}

void MainFrameImpl::EventLaunch(wxCommandEvent& event) {
	this->LaunchGame();
}

void MainFrameImpl::LaunchGame() {
	BZLauncherApp& app = wxGetApp();
	app.LaunchSelectedServer();
}
