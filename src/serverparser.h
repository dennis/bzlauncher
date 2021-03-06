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
#ifndef __serverparser__
#define __serverparser__

#include "server.h"

/// Generic serverhex parser. 
/// This class is used to parse the hex-string we get from listserver. 
/// The hex depends on what version of BZFS is used
class ServerHexParser {
public:
	/// Factory - supply a version-string (eg BZFS0026) and it will
	/// return a point to a ServerHexParser object - or NULL if not 
	/// supported
	static ServerHexParser* GetParser(const wxString&);

	virtual ~ServerHexParser() {};

	/// Parse the hexstring and updates the server object
	virtual void parse(const wxString, Server&) = 0;
};

/// Parser for BZFS0026 
class ServerHexParserBZFS0026 : public ServerHexParser {
private:	
	/// GameStyle as defined in BZFlag source
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
public:
	void parse(const wxString, Server&);
};

#endif
