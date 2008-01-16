#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"

#include "listserver.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {

}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}

void MainFrameImpl::EventRefresh(wxCommandEvent&) {
	BZLauncherApp& app = wxGetApp();

	app.RefreshServerList();

	wxGrid* list;
	list = this->serverList;

	// Clear list
	list->BeginBatch();
	list->DeleteRows(0,list->GetNumberRows());
	list->InsertRows(0,app.list.list.GetCount());

	// Content
	ListServerList::iterator i;
	int row = 0;
	int col = 0;
	for(i = app.list.list.begin(); i != app.list.list.end(); ++i) {
		Server*	current = *i;
		col = 0;

		// Favorites
		list->SetCellValue(row, col, _T("n/a"));
		list->SetReadOnly(row, col);
		list->SetCellTextColour(row, col, *wxLIGHT_GREY);

		// Name
		col++;
		wxString s = current->serverHost + wxString::Format(_T(":%d"), current->serverPort);
		list->SetCellValue(row, col, s);
		list->SetReadOnly(row, col);
		list->SetCellTextColour(row, col, *wxBLACK);

		// Type
		col++;
		list->SetCellValue(row, col, current->name);
		list->SetReadOnly(row, col);
		list->SetCellTextColour(row, col, *wxBLACK);

		// Players
		col++;
		list->SetCellValue(row, col, wxString::Format(_T("%d"), current->getPlayerCount()));
		list->SetReadOnly(row, col);
		list->SetCellTextColour(row, col, *wxBLACK);

		// Ping
		col++;
		list->SetCellValue(row, col, _T("n/a"));
		list->SetReadOnly(row, col);
		list->SetCellTextColour(row, col, *wxLIGHT_GREY);

		row++;
	}
	
	list->EndBatch();
}

void MainFrameImpl::EventShowAbout(wxCommandEvent&) {
	AboutDlgImpl dlg(this);
	dlg.ShowModal();
}

void MainFrameImpl::EventQuit(wxCommandEvent&) {
	this->Close();
}
