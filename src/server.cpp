#include <wx/log.h>

#include "server.h"

void Server::ParseServerInfo(const wxString& info) {
	char theBuf[55];

	strncpy(theBuf,info.ToAscii(),54);
	theBuf[54] = (char)NULL;

	char* p = theBuf;
	p = this->unpackHex16(p, this->gameStyle);
	p = this->unpackHex16(p, this->maxShots);
	p = this->unpackHex16(p, this->shakeWins);
	p = this->unpackHex16(p, this->shakeTimeout);
	p = this->unpackHex16(p, this->maxPlayerScore);
	p = this->unpackHex16(p, this->maxTeamScore);
	p = this->unpackHex16(p, this->maxTime);
	p = this->unpackHex8(p, this->maxPlayers);
	p = this->unpackHex8(p, this->rogueCount);
	p = this->unpackHex8(p, this->rogueMax);
	p = this->unpackHex8(p, this->redCount);
	p = this->unpackHex8(p, this->redMax);
	p = this->unpackHex8(p, this->greenCount);
	p = this->unpackHex8(p, this->greenMax);
	p = this->unpackHex8(p, this->blueCount);
	p = this->unpackHex8(p, this->blueMax);
	p = this->unpackHex8(p, this->purpleCount);
	p = this->unpackHex8(p, this->purpleMax);
	p = this->unpackHex8(p, this->observerCount);
	p = this->unpackHex8(p, this->observerMax);
}

// Pretty much stolen from BZFlag
int Server::hex2bin(char d) {
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

char* Server::unpackHex16(char* buf, uint16_t& d) {
	d = 0;
	d = (d << 4) | hex2bin(*buf++);
	d = (d << 4) | hex2bin(*buf++);
	d = (d << 4) | hex2bin(*buf++);
	d = (d << 4) | hex2bin(*buf++);
	return buf;
}

char* Server::unpackHex8(char* buf, uint8_t& d) {
	d = 0;
	d = (d << 4) | hex2bin(*buf++);
	d = (d << 4) | hex2bin(*buf++);
	return buf;
}
