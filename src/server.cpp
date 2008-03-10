#include <wx/log.h>

#include "server.h"

static int Hex2bin(char d) {
	switch(d) {
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': 
		case 'A': return 10;
		case 'b': 
		case 'B': return 11;
		case 'c': 
		case 'C': return 12;
		case 'd': 
		case 'D': return 13;
		case 'e': 
		case 'E': return 14;
		case 'f': 
		case 'F': return 15;
	}
	return 0;
}

static char* UnpackHex16(char* buf, uint16_t& d) {
	d = 0;
	d = (d << 4) | Hex2bin(*buf++);
	d = (d << 4) | Hex2bin(*buf++);
	d = (d << 4) | Hex2bin(*buf++);
	d = (d << 4) | Hex2bin(*buf++);
	return buf;
}

static char* UnpackHex8(char* buf, uint8_t& d) {
	d = 0;
	d = (d << 4) | Hex2bin(*buf++);
	d = (d << 4) | Hex2bin(*buf++);
	return buf;
}

ServerHexParser* ServerHexParser::GetParser(const wxString& proto) {
	if(proto.CmpNoCase(_T("BZFS0026")) == 0)
		return new ServerHexParserBZFS0026();
	return NULL;
}

void ServerHexParserBZFS0026::parse(const wxString hex, Server& s) {
	char theBuf[55];

	strncpy(theBuf,hex.ToAscii(),54);
	theBuf[54] = (char)NULL;

	char* p = theBuf;
	p = UnpackHex16(p, s.gameStyle);
	p = UnpackHex16(p, s.maxShots);
	p = UnpackHex16(p, s.shakeWins);
	p = UnpackHex16(p, s.shakeTimeout);
	p = UnpackHex16(p, s.maxPlayerScore);
	p = UnpackHex16(p, s.maxTeamScore);
	p = UnpackHex16(p, s.maxTime);
	p = UnpackHex8(p, s.maxPlayers);
	p = UnpackHex8(p, s.rogueTeam.count);
	p = UnpackHex8(p, s.rogueTeam.max);
	p = UnpackHex8(p, s.redTeam.count);
	p = UnpackHex8(p, s.redTeam.max);
	p = UnpackHex8(p, s.greenTeam.count);
	p = UnpackHex8(p, s.greenTeam.max);
	p = UnpackHex8(p, s.blueTeam.count);
	p = UnpackHex8(p, s.blueTeam.max);
	p = UnpackHex8(p, s.purpleTeam.count);
	p = UnpackHex8(p, s.purpleTeam.max);
	p = UnpackHex8(p, s.observerTeam.count);
	p = UnpackHex8(p, s.observerTeam.max);

	s.fullyParsed = true;
}

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

