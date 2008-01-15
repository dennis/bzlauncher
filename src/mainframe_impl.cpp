#include "mainframe_impl.h"

MainFrameImpl::MainFrameImpl( wxWindow* parent )
: MainFrame( parent ) {

}

void MainFrameImpl::SetStatusText(const wxString& t) {
	this->statusBar->SetStatusText(t);
}
