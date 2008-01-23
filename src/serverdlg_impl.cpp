#include "serverdlg_impl.h"

#include <wx/msgdlg.h>

ServerDlgImpl::ServerDlgImpl(wxWindow* parent, Server* server)
: ServerDlg( parent ) {

	// ServerName
	serverName->SetLabel(server->serverHostPort);

	// LEFT COLUMN

	// Players
	playersVal->SetValue(wxString::Format(_T("%d/%d"), server->GetPlayerCount(), server->maxPlayers));

	// Rogue
	rogueVal->SetValue(wxString::Format(_T("%d/%d"), server->rogueCount, server->rogueMax));

	// red
	redVal->SetValue(wxString::Format(_T("%d/%d"), server->redCount, server->redMax));

	// green
	greenVal->SetValue(wxString::Format(_T("%d/%d"), server->greenCount, server->greenMax));
	// blue
	blueVal->SetValue(wxString::Format(_T("%d/%d"), server->blueCount, server->blueMax));
	// purple
	purpleVal->SetValue(wxString::Format(_T("%d/%d"), server->purpleCount, server->purpleMax));
	// observers
	observersVal->SetValue(wxString::Format(_T("%d/%d"), server->observerCount, server->observerMax));

	// RIGHT COLUMN

	// Shots
	shotsVal->SetValue(wxString::Format(_T("%d"), server->maxShots));

	// Game type
	if(server->IsCTF())
		gameTypeVal->SetValue(_("CTF"));
	else if(server->IsFFA()) 
		gameTypeVal->SetValue(_("FFA"));
	else if(server->IsRH()) 
		gameTypeVal->SetValue(_("RH"));
	else
		gameTypeVal->SetValue(_("???"));
	
	// Super flags
	if(server->GotSuperFlags())
		superFlagsVal->SetValue(_("Yes"));
	else
		superFlagsVal->SetValue(_("No"));
	
	// Antidote flags
	if(server->GotAntidote())
		antidoteVal->SetValue(_("Yes"));
	else
		antidoteVal->SetValue(_("No"));

	// Ricochet
	if(server->GotRicochet())
		ricochetVal->SetValue(_("Yes"));
	else
		ricochetVal->SetValue(_("No"));

	// Handicap
	if(server->GotHandicap())
		handicapVal->SetValue(_("Yes"));
	else
		handicapVal->SetValue(_("No"));

	// Jumping
	if(server->GotJumping())
		jumpVal->SetValue(_("Yes"));
	else
		jumpVal->SetValue(_("No"));
	
	closeBtn->SetFocus();
}

void ServerDlgImpl::EventClose(wxCommandEvent& event) {
	EndModal(0);
}
