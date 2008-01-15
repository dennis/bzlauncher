#include "main.h"
#include "mainframe_impl.h"
#include "aboutdlg_impl.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {

}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}

void MainFrameImpl::EventRefresh(wxCommandEvent&) {
	wxGetApp().RefreshServerList();
}

void MainFrameImpl::EventShowAbout(wxCommandEvent&) {
	AboutDlgImpl dlg(this);
	dlg.ShowModal();
}
