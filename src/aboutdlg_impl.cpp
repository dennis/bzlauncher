#include "aboutdlg_impl.h"

#include <wx/msgdlg.h>

AboutDlgImpl::AboutDlgImpl( wxWindow* parent )
: AboutDlg( parent ) {

}

void AboutDlgImpl::EventClose(wxCommandEvent& WXUNUSED(event)) {
	EndModal(0);
}
