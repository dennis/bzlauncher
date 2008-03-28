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
#include "server.h"

Server::Server() : gameStyle(0), maxShots(0), shakeWins(0), shakeTimeout(0), maxPlayerScore(0), maxTeamScore(0),
	maxTime(0), maxPlayers(0), fullyParsed(false), favorite(false) {
}

wxString Server::getName() const {
	return this->name;
}

void Server::setName(const wxString& n) {
	this->name = n;
}

void Server::setIP(const wxIPV4address& val) {
	this->ip = val;
	this->ping = ServerPing(val);
}

const wxIPV4address& Server::getIP() const {
	return this->ip;
}

int Server::GetPlayerCount() const {
	return 
		this->team[TEAM_ROGUE].count+
		this->team[TEAM_RED].count+
		this->team[TEAM_GREEN].count+
		this->team[TEAM_BLUE].count+
		this->team[TEAM_PURPLE].count;
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

