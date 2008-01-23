#include "serverdlg_impl.h"

#include <wx/msgdlg.h>

ServerDlgImpl::ServerDlgImpl(wxWindow* parent, Server* server)
: ServerDlg( parent ) {

	// ServerHostPort + Name
	this->serverHostPort->SetLabel(server->serverHostPort);
	this->serverName->SetLabel(server->name);

	// LEFT COLUMN

	// Players
	this->playersVal->SetValue(wxString::Format(_T("%d/%d"), server->GetPlayerCount(), server->maxPlayers));

	this->rogueVal->SetValue(wxString::Format(_T("%d/%d"), server->rogueCount, server->rogueMax));
	this->redVal->SetValue(wxString::Format(_T("%d/%d"), server->redCount, server->redMax));
	this->greenVal->SetValue(wxString::Format(_T("%d/%d"), server->greenCount, server->greenMax));
	this->blueVal->SetValue(wxString::Format(_T("%d/%d"), server->blueCount, server->blueMax));
	this->purpleVal->SetValue(wxString::Format(_T("%d/%d"), server->purpleCount, server->purpleMax));
	this->observersVal->SetValue(wxString::Format(_T("%d/%d"), server->observerCount, server->observerMax));

	// RIGHT COLUMN

	// Shots
	this->shotsVal->SetValue(wxString::Format(_T("%d"), server->maxShots));

	// Game type
	if(server->IsCTF())
		this->gameTypeVal->SetValue(_("CTF"));
	else if(server->IsFFA()) 
		this->gameTypeVal->SetValue(_("FFA"));
	else if(server->IsRH()) 
		this->gameTypeVal->SetValue(_("RH"));
	else
		this->gameTypeVal->SetValue(_("???"));
	
	// Super flags
	if(server->GotSuperFlags())
		this->superFlagsVal->SetValue(_("Yes"));
	else
		this->superFlagsVal->SetValue(_("No"));
	
	// Antidote flags
	if(server->GotAntidote())
		this->antidoteVal->SetValue(_("Yes"));
	else
		this->antidoteVal->SetValue(_("No"));

	// Ricochet
	if(server->GotRicochet())
		this->ricochetVal->SetValue(_("Yes"));
	else
		this->ricochetVal->SetValue(_("No"));

	// Handicap
	if(server->GotHandicap())
		this->handicapVal->SetValue(_("Yes"));
	else
		this->handicapVal->SetValue(_("No"));

	// Jumping
	if(server->GotJumping())
		this->jumpVal->SetValue(_("Yes"));
	else
		this->jumpVal->SetValue(_("No"));
	
	this->closeBtn->SetFocus();
}

void ServerDlgImpl::EventClose(wxCommandEvent& event) {
	this->EndModal(0);
}
