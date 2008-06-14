/*
The MIT License

Copyright (c) 2008 Dennis Møllegaard Pedersen <dennis@moellegaard.dk>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#include "serverdlg_impl.h"
#include "main.h"
#include "mainframe_impl.h"

#include <wx/msgdlg.h>

// These variables are used with the ComboBox.. It got a ClientData ptr, which is pointed as these variables
static const Server::team_color_t	TeamRogue = Server::team_rogue;
static const Server::team_color_t	TeamRed = Server::team_red;
static const Server::team_color_t	TeamGreen = Server::team_green;
static const Server::team_color_t	TeamBlue = Server::team_blue;
static const Server::team_color_t	TeamPurple = Server::team_purple;
static const Server::team_color_t	TeamObserver = Server::team_observer;
static const Server::team_color_t	TeamAny = Server::team_count;

ServerDlgImpl::ServerDlgImpl(wxWindow* parent, Server* selectedServer)
: ServerDlg( parent ) {
	// ServerHostPort + Name
	this->serverHostPort->SetLabel(selectedServer->name());
	this->serverName->SetLabel(selectedServer->longName);

	// Server-notice. If any
	if(!selectedServer->fullyParsed) {
		this->notice->SetForegroundColour(*wxRED);
		this->notice->SetLabel(wxString::Format(_("This server uses an unsupported protocol (%s)"), selectedServer->protocolVersion().c_str()));
		this->notice->Show();
	}
	else if(selectedServer->IsFull()) {
		this->notice->SetForegroundColour(*wxBLACK);
		this->notice->SetLabel(_("This server is full"));
		this->notice->Show();
	}
	else if(selectedServer->IsEmpty()) {
		this->notice->SetForegroundColour(*wxBLACK);
		this->notice->SetLabel(_("This server is empty"));
		this->notice->Show();
	}

	// LEFT COLUMN

	// Players
	this->playersVal->SetValue(wxString::Format(_T("%d/%d"), 
		selectedServer->GetPlayerCount(), 
		selectedServer->maxPlayers.value));

	this->rogueVal->SetValue(selectedServer->teams.color[Server::team_rogue]);
	this->redVal->SetValue(selectedServer->teams.color[Server::team_red]);
	this->greenVal->SetValue(selectedServer->teams.color[Server::team_green]);
	this->blueVal->SetValue(selectedServer->teams.color[Server::team_blue]);
	this->purpleVal->SetValue(selectedServer->teams.color[Server::team_purple]);
	this->observersVal->SetValue(selectedServer->teams.color[Server::team_observer]);

	// RIGHT COLUMN

	// Shots
	this->shotsVal->SetValue(selectedServer->maxShots());

	// Game type
	/*
	if(selectedServer->IsCTF())
		this->gameTypeVal->SetValue(_("CTF"));
	else if(selectedServer->IsFFA()) 
		this->gameTypeVal->SetValue(_("FFA"));
	else if(selectedServer->IsRC()) 
		this->gameTypeVal->SetValue(_("RC"));
	else
		this->gameTypeVal->SetValue(_("???"));

	*/
	this->superFlagsVal->SetValue(selectedServer->gotSuperFlags()); // Super flags
	this->antidoteVal->SetValue(selectedServer->gotAntidote()); 	// Antidote flags
	this->ricochetVal->SetValue(selectedServer->gotRicochet());		// Ricochet
	this->handicapVal->SetValue(selectedServer->gotHandicap());		// Handicap
	this->jumpVal->SetValue(selectedServer->gotJumping()); 			// Jumping
	
	// Launch dropdown
	this->teamCbx->Clear();
	this->teamCbx->Append(_T("")); // Default

	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_rogue].value.max) this->teamCbx->Append(_("as Rogue"), (void*)&TeamRogue);
	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_red].value.max) this->teamCbx->Append(_("as Red"), (void*)&TeamRed);
	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_green].value.max) this->teamCbx->Append(_("as Green"), (void*)&TeamGreen);
	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_blue].value.max) this->teamCbx->Append(_("as Blue"), (void*)&TeamBlue);
	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_purple].value.max) this->teamCbx->Append(_("as Purple"), (void*)&TeamPurple);
	if(!selectedServer->fullyParsed || selectedServer->teams.color[Server::team_observer].value.max) this->teamCbx->Append(_("as Observer"), (void*)&TeamObserver);
	this->teamCbx->Refresh();
	
	this->closeBtn->SetFocus();

	// Select server
	const wxString s = selectedServer->name();
	wxGetApp().SetSelectedServer(s);
}

void ServerDlgImpl::EventClose(wxCommandEvent& WXUNUSED(event)) {
	this->EndModal(0);
}

void ServerDlgImpl::onLaunch(wxCommandEvent& WXUNUSED(event)) {
	Server::team_color_t*	team = NULL;
	int selected = this->teamCbx->GetSelection();
	if(selected != wxNOT_FOUND)
		team = (Server::team_color_t*)this->teamCbx->GetClientData(selected);
	if(team == NULL) team = const_cast<Server::team_color_t*>(&TeamAny);
	wxGetApp().mainFrame->AddAsRecentServer(this->serverHostPort->GetLabel());
	wxGetApp().LaunchSelectedServer(this, *team);
}
