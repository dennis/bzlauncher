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

