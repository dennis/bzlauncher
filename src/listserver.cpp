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
#include <wx/tokenzr.h>
#include <wx/url.h>
#include <wx/log.h>
#include <wx/intl.h>
#include <wx/filesys.h>

#include "datactrl.h"
#include "listserver.h"

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

ListServer::ListServer(const wxArrayString& _s) : listServers(_s) {
	wxLogDebug(_T("ListServer::out_queue = %p"), &this->out_queue);
}

void ListServer::initializeLabels(DataController* datactrl) {
	wxLogDebug(_T("ListServer::initializeLabels(datactrl)"));
	// Ownership is transferred to ctrl, so we dont need to free them
	datactrl->addLabel(this->lblserver       = new Label(_T("server"),      _("Server")));
	datactrl->addLabel(this->lblprotocol     = new Label(_T("protocol"),    _("Protocol Version")));
	datactrl->addLabel(this->lbltext	      = new Label(_T("text"),         _("Text")));

	datactrl->addLabel(this->lbltype	      = new Label(_T("type"),         _("Type")));
                                                                              
	datactrl->addLabel(this->lblctf          = new Label(_T("ctf"),         _("CTF")));
	datactrl->addLabel(this->lblrc           = new Label(_T("rc"),          _("RC")));
	datactrl->addLabel(this->lblffa          = new Label(_T("ffa"),         _("FFA")));
                                                                              
	datactrl->addLabel(this->lblsuperflags   = new Label(_T("superflags"),  _("Superflags")));
	datactrl->addLabel(this->lbljumping      = new Label(_T("jumping"),     _("Jumping")));
	datactrl->addLabel(this->lblinertia      = new Label(_T("inertia"),     _("Inertia")));
	datactrl->addLabel(this->lblantidote     = new Label(_T("antidote"),    _("Antidote")));
	datactrl->addLabel(this->lblshakable     = new Label(_T("shakable"),    _("Shakable")));
	datactrl->addLabel(this->lblricochet     = new Label(_T("ricochet"),    _("Ricohet")));
	datactrl->addLabel(this->lblhandicap     = new Label(_T("handicap"),    _("Handicap")));
                                                                              
	datactrl->addLabel(this->lblmaxshots     = new Label(_T("maxshots"),    _("Shots")));
	datactrl->addLabel(this->lblshakewins    = new Label(_T("shakewins"),   _("Shakewins")));
	datactrl->addLabel(this->lblshaketimeout = new Label(_T("shaketimeout"),_("Shaketimeout")));
	datactrl->addLabel(this->lblmaxplayerscore = new Label(_T("maxplayerscore"), _("Max player score")));
	datactrl->addLabel(this->lblmaxteamscore = new Label(_T("maxteamscore"),_("Max team score")));
	datactrl->addLabel(this->lblmaxtime      = new Label(_T("maxtime"),     _("Max time")));
	datactrl->addLabel(this->lblmaxplayers   = new Label(_T("maxplayers"),  _("#")));
                                                                              
	datactrl->addLabel(this->lblroguecount   = new Label(_T("roguecount"),  _("Rogues")));
	datactrl->addLabel(this->lblroguemax     = new Label(_T("roguemax"),    _("Max rogues")));
                                                                              
	datactrl->addLabel(this->lblredcount     = new Label(_T("redcount"),    _("Reds")));
	datactrl->addLabel(this->lblredmax       = new Label(_T("redmax"),      _("Max reds")));
                                                                              
	datactrl->addLabel(this->lblgreencount   = new Label(_T("greencount"),  _("Greens")));
	datactrl->addLabel(this->lblgreenmax     = new Label(_T("greenmax"),    _("Max greens")));
                                                                              
	datactrl->addLabel(this->lblbluecount    = new Label(_T("bluecount"),   _("Blues")));
	datactrl->addLabel(this->lblbluemax      = new Label(_T("bluemax"),     _("Max blues")));
                                                                              
	datactrl->addLabel(this->lblpurplecount  = new Label(_T("purplecount"), _("Purples")));
	datactrl->addLabel(this->lblpurplemax    = new Label(_T("purplemax"),   _("Max purples")));
                                                                              
	datactrl->addLabel(this->lblobservercount   = new Label(_T("observercount"), _("Observers")));
	datactrl->addLabel(this->lblobservermax     = new Label(_T("observermax"), _("Max observers")));
}

ListServer::~ListServer() {
}

ListServer::ExitCode ListServer::Entry() {
	this->GetServerList();
	while(!this->TestDestroy()) {
		wxSleep(1);
	}
	return 0;
}

void ListServer::GetServerList() {
	int count = 0;
	if( this->GetListServerResponse() ) {
		wxLogDebug(_T("[%lx] Parsing data from list-server..."), this->GetId());
		wxStringTokenizer tok(this->rawResponse, _T("\r\n"));

		while(tok.HasMoreTokens()) {	
			count++;
			wxString token = tok.GetNextToken();
			this->ParseLine(token);
		}
	}
	else {
		wxLogError(_("Can't connect to listserver!"));
	}
	wxLogDebug(_T("Found %d server(s)"), count);
}

bool ListServer::GetListServerResponse() {
	wxFileSystem	fs;
	wxFSFile*		listserv = NULL;
	bool			r = false;

	for(unsigned int c = 0; c < this->listServers.GetCount() && listserv == NULL; ++c)
		listserv = fs.OpenFile(this->listServers[c]);

	if(listserv) {
		wxInputStream *in_stream;
		in_stream = listserv->GetStream();

		if(in_stream) {
			char		buffer[1024];

			this->rawResponse.Clear();

			while(in_stream->Read(buffer,1024).LastRead()) {
				this->rawResponse << wxString::From8BitData(buffer,in_stream->LastRead());
			}

			r = true;
		}
		else {
			wxLogMessage(_("in_stream == null"));
		}

		delete listserv;
	}

	return r;
}

bool ListServer::ParseLine(const wxString& line) {
	wxString name;
	wxString proto;
	wxStringTokenizer tok(line, _T(" "));

	int i = 0;
	while(tok.HasMoreTokens()) {
		wxString token = tok.GetNextToken();
		switch(i) {
		case 0:	{ // servername:port
			name = token;
			this->updateAttribute(name, this->lblserver, Attribute<wxString>(name));
			}
			break;
		case 1:  // version 
			proto = token;
			this->updateAttribute(name, this->lblprotocol, Attribute<wxString>(token));
			break;
		case 2:  // Hex info
			if(proto.CmpNoCase(_T("BZFS0026")) == 0)
				ServerHexParserBZFS0026(name,token);
			break;
		case 3: 
			/*
			{
				wxIPV4address ip;
				ip.Hostname(token);
				ip.Service(port);
				s->setIP(ip);
				s->longName.value += tok.GetString();
			}
			*/
			this->updateAttribute(name, this->lbltext, Attribute<wxString>(tok.GetString()));
			break;
		}
		i++;
	}

	return true;
}

void ListServer::ServerHexParserBZFS0026(const wxString& name, const wxString& hex) {
	enum GameStyle {
	  PlainGameStyle =       0x0000,
	  TeamFlagGameStyle =    0x0001, // capture the flag
	  SuperFlagGameStyle =   0x0002, // superflags allowed
	  //FormerRogueStyle =   0x0004, // used to be rogue, but now we have rogue maxplayers 
	  JumpingGameStyle =     0x0008, // jumping allowed
	  InertiaGameStyle =     0x0010, // momentum for all
	  RicochetGameStyle =    0x0020, // all shots ricochet
	  ShakableGameStyle =    0x0040, // can drop bad flags
	  AntidoteGameStyle =    0x0080, // anti-bad flags
	  HandicapGameStyle =    0x0100, // handicap players based on score (eek! was TimeSyncGameStyle)
	  RabbitChaseGameStyle = 0x0200  // rabbit chase
	  // add here before reusing old ones above
	};
	char theBuf[55];

	strncpy(theBuf,hex.ToAscii(),54);
	theBuf[54] = (char)NULL;

	char* p = theBuf;

	uint16_t	gamestyle;
	p = UnpackHex16(p, gamestyle);

	bool isctf = (gamestyle & TeamFlagGameStyle) == TeamFlagGameStyle;
	bool isrc  = (gamestyle & RabbitChaseGameStyle) == RabbitChaseGameStyle;
	bool isffa = (!isctf && !isrc);

#define PB(x,y) \
	this->updateAttribute(name, x, Attribute<bool>(y));

	PB(this->lblctf, isctf);
	PB(this->lblrc , isrc);
	PB(this->lblffa, isffa);
	PB(this->lblsuperflags, (gamestyle & SuperFlagGameStyle) == SuperFlagGameStyle);
	PB(this->lbljumping,    (gamestyle & JumpingGameStyle) == JumpingGameStyle);
	PB(this->lblinertia,    (gamestyle & InertiaGameStyle) == InertiaGameStyle);
	PB(this->lblantidote,   (gamestyle & AntidoteGameStyle) == AntidoteGameStyle);
	PB(this->lblshakable,   (gamestyle & ShakableGameStyle) == ShakableGameStyle);
	PB(this->lblricochet,   (gamestyle & RicochetGameStyle) == RicochetGameStyle);
	PB(this->lblhandicap,   (gamestyle & HandicapGameStyle) == HandicapGameStyle);

	if(isctf) 
		this->updateAttribute(name, this->lbltype, Attribute<wxString>(_T("CTF")));
	else if(isrc)
		this->updateAttribute(name, this->lbltype, Attribute<wxString>(_T("RC")));
	else if(isffa)
		this->updateAttribute(name, this->lbltype, Attribute<wxString>(_T("FFA")));
	else
		this->updateAttribute(name, this->lbltype, Attribute<wxString>(_T("???")));

#undef PB

	uint8_t value8;
	uint16_t value16;

#define P16(x) \
	p = UnpackHex16(p, value16); this->updateAttribute(name, x, Attribute<uint16_t>(value16));
#define P8(x) \
	p = UnpackHex8(p, value8); this->updateAttribute(name, x, Attribute<uint8_t>(value8));

	P16(this->lblmaxshots);
	P16(this->lblshakewins);
	P16(this->lblshaketimeout);
	P16(this->lblmaxplayerscore);
	P16(this->lblmaxteamscore);
	P16(this->lblmaxtime);
	P8(this->lblmaxplayers);

	P8(this->lblroguecount);
	P8(this->lblroguemax);

	P8(this->lblredcount);
	P8(this->lblredmax);

	P8(this->lblgreencount);
	P8(this->lblgreenmax);

	P8(this->lblbluecount);
	P8(this->lblbluemax);

	P8(this->lblpurplecount);
	P8(this->lblpurplemax);

	P8(this->lblobservercount);
	P8(this->lblobservermax);
#undef P16
#undef P8
}
