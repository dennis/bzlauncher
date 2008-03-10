#include "serverdlg_impl.h"

#include <wx/msgdlg.h>

ServerDlgImpl::ServerDlgImpl(wxWindow* parent, Server* server)
: ServerDlg( parent ) {

	// ServerHostPort + Name
	this->serverHostPort->SetLabel(server->getName());
	this->serverName->SetLabel(server->longName);

	// Server-notice. If any
	if(!server->fullyParsed) {
		this->notice->SetForegroundColour(*wxRED);
		this->notice->SetLabel(wxString::Format(_("This server uses an unsupported protocol (%s)"), server->protocolVersion.c_str()));
		this->notice->Show();
	}
	/*
	else if(server->IsFull()) {
		this->notice->SetLabel(_("This server is full"));
		this->notice->Show();
	}
	else if(server->IsEmpty()) {
		this->notice->SetLabel(_("This server is deserted"));
		this->notice->Show();
	}
	*/

	// LEFT COLUMN

	// Players
	this->playersVal->SetValue(wxString::Format(_T("%d/%d"), server->GetPlayerCount(), server->maxPlayers));

	this->rogueVal->SetValue(wxString::Format(_T("%d/%d"), server->rogueTeam.count, server->rogueTeam.max));
	this->redVal->SetValue(wxString::Format(_T("%d/%d"), server->redTeam.count, server->redTeam.max));
	this->greenVal->SetValue(wxString::Format(_T("%d/%d"), server->greenTeam.count, server->greenTeam.max));
	this->blueVal->SetValue(wxString::Format(_T("%d/%d"), server->blueTeam.count, server->blueTeam.max));
	this->purpleVal->SetValue(wxString::Format(_T("%d/%d"), server->purpleTeam.count, server->purpleTeam.max));
	this->observersVal->SetValue(wxString::Format(_T("%d/%d"), server->observerTeam.count, server->observerTeam.max));

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

void ServerDlgImpl::EventClose(wxCommandEvent& WXUNUSED(event)) {
	this->EndModal(0);
}
