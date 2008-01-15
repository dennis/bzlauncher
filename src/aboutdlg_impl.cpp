#include "aboutdlg_impl.h"

#include <wx/msgdlg.h>

AboutDlgImpl::AboutDlgImpl( wxWindow* parent )
: AboutDlg( parent ) {

}

void AboutDlgImpl::EventClose(wxCommandEvent& event) {
	EndModal(0);
}
