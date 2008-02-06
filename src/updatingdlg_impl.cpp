#include "updatingdlg_impl.h"

UpdatingDlgImpl::UpdatingDlgImpl( wxWindow* parent )
: UpdatingDlg( parent ) {
	this->Pulse();
}

void UpdatingDlgImpl::Pulse() {
	this->processIndicator->Pulse();
	this->processIndicator->Update();
}
