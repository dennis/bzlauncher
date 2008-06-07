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
#include "serverparser.h"

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
	uint8_t ui8;
	uint16_t ui16;
	p = UnpackHex16(p, s.gameStyle);
	p = UnpackHex16(p, ui16); s.maxShots = ui16;
	p = UnpackHex16(p, ui16); s.shakeWins = ui16;
	p = UnpackHex16(p, ui16); s.shakeTimeout = ui16;
	p = UnpackHex16(p, ui16); s.maxPlayerScore = ui16;
	p = UnpackHex16(p, ui16); s.maxTeamScore = ui16;
	p = UnpackHex16(p, ui16); s.maxTime = ui16;
	p = UnpackHex8(p, ui8); s.maxPlayers = ui8;
	p = UnpackHex8(p, s.team[Server::TEAM_ROGUE].count);
	p = UnpackHex8(p, s.team[Server::TEAM_ROGUE].max);
	p = UnpackHex8(p, s.team[Server::TEAM_RED].count);
	p = UnpackHex8(p, s.team[Server::TEAM_RED].max);
	p = UnpackHex8(p, s.team[Server::TEAM_GREEN].count);
	p = UnpackHex8(p, s.team[Server::TEAM_GREEN].max);
	p = UnpackHex8(p, s.team[Server::TEAM_BLUE].count);
	p = UnpackHex8(p, s.team[Server::TEAM_BLUE].max);
	p = UnpackHex8(p, s.team[Server::TEAM_PURPLE].count);
	p = UnpackHex8(p, s.team[Server::TEAM_PURPLE].max);
	p = UnpackHex8(p, s.team[Server::TEAM_OBSERVER].count);
	p = UnpackHex8(p, s.team[Server::TEAM_OBSERVER].max);

	s.fullyParsed = true;
}

