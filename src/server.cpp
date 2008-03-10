#include "server.h"

Server::Server() : gameStyle(0), maxShots(0), shakeWins(0), shakeTimeout(0), maxPlayerScore(0), maxTeamScore(0),
	maxTime(0), maxPlayers(0), fullyParsed(false), favorite(false) {
}

void Server::setIP(const wxIPV4address& val) {
	this->ip = val;
	this->ping = ServerPing(val);
}

const wxIPV4address& Server::getIP() {
	return this->ip;
}

int Server::GetPlayerCount() const {
	return 
		this->rogueTeam.count+
		this->redTeam.count+
		this->greenTeam.count+
		this->blueTeam.count+
		this->purpleTeam.count;
}

bool Server::IsFull() const {
	return this->GetPlayerCount() == this->maxPlayers;
}

bool Server::IsEmpty() const {
	return this->GetPlayerCount() == 0;
}

bool Server::IsCTF() const {
	return (this->gameStyle & TeamFlagGameStyle) == TeamFlagGameStyle;
}

bool Server::IsRH() const {
	return (this->gameStyle & RabbitChaseGameStyle) == RabbitChaseGameStyle;
}

bool Server::IsFFA() const {
	return !this->IsCTF() && !this->IsRH();
}

const wxString Server::GetType() const {
	if( this->IsCTF() )
		return wxT("CTF");
	else if( this->IsFFA() )
		return wxT("FFA");
	else if( this->IsRH() )
		return wxT("RH");
	else
		return wxT("??");
}

bool Server::GotSuperFlags() const {
	return this->gameStyle && SuperFlagGameStyle;
}

bool Server::GotJumping() const {
	return this->gameStyle && JumpingGameStyle;
}

bool Server::GotAntidote() const {
	return this->gameStyle && AntidoteGameStyle;
}

bool Server::GotRicochet() const {
	return this->gameStyle && RicochetGameStyle;
}

bool Server::GotHandicap() const {
	return this->gameStyle && HandicapGameStyle;
}

