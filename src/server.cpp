#include <wx/log.h>

#include "server.h"

void Server::ParseServerInfoBZFS0026(const wxString& info) {
	char theBuf[55];

	strncpy(theBuf,info.ToAscii(),54);
	theBuf[54] = (char)NULL;

	char* p = theBuf;
	p = this->UnpackHex16(p, this->gameStyle);
	p = this->UnpackHex16(p, this->maxShots);
	p = this->UnpackHex16(p, this->shakeWins);
	p = this->UnpackHex16(p, this->shakeTimeout);
	p = this->UnpackHex16(p, this->maxPlayerScore);
	p = this->UnpackHex16(p, this->maxTeamScore);
	p = this->UnpackHex16(p, this->maxTime);
	p = this->UnpackHex8(p, this->maxPlayers);
	p = this->UnpackHex8(p, this->rogueTeam.count);
	p = this->UnpackHex8(p, this->rogueTeam.max);
	p = this->UnpackHex8(p, this->redTeam.count);
	p = this->UnpackHex8(p, this->redTeam.max);
	p = this->UnpackHex8(p, this->greenTeam.count);
	p = this->UnpackHex8(p, this->greenTeam.max);
	p = this->UnpackHex8(p, this->blueTeam.count);
	p = this->UnpackHex8(p, this->blueTeam.max);
	p = this->UnpackHex8(p, this->purpleTeam.count);
	p = this->UnpackHex8(p, this->purpleTeam.max);
	p = this->UnpackHex8(p, this->observerTeam.count);
	p = this->UnpackHex8(p, this->observerTeam.max);
}

// Pretty much stolen from BZFlag
int Server::Hex2bin(char d) {
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

char* Server::UnpackHex16(char* buf, uint16_t& d) {
	d = 0;
	d = (d << 4) | this->Hex2bin(*buf++);
	d = (d << 4) | this->Hex2bin(*buf++);
	d = (d << 4) | this->Hex2bin(*buf++);
	d = (d << 4) | this->Hex2bin(*buf++);
	return buf;
}

char* Server::UnpackHex8(char* buf, uint8_t& d) {
	d = 0;
	d = (d << 4) | this->Hex2bin(*buf++);
	d = (d << 4) | this->Hex2bin(*buf++);
	return buf;
}
