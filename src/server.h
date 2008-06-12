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
#ifndef __server_h__
#define __server_h__

#include <map>
#include <wx/string.h>
#include <wx/socket.h>

#include "attribute.h"
#include "ping.h"
#include "types.h"

/// BZFlag server description
class Server {
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
	typedef enum { team_observer, team_red, team_green, team_blue, team_purple, team_rogue, team_count } team_color_t;
	wxIPV4address	ip;
	Ping			ping;

	uint16_t	gameStyle;

	class Team {
	public:
		Team() : count(0), max(0) {};
		uint8_t count;
		uint8_t max;

		uint8_t getCount() const {
			return this->count;
		}
		uint8_t getMax() const {
			return this->max;
		}
	};

	class Teams {
	public:
		typedef std::map< team_color_t, Attribute<Team> > teammap_t;
		teammap_t	color;
	};
		
	bool		fullyParsed;
	bool		favorite;
	bool		recent;

	Server();

	void setIP(const wxIPV4address&);
	const wxIPV4address& getIP() const;

	bool IsCTF() const;
	bool IsRC() const;
	bool IsFFA() const;

	const wxString GetType() const;

	bool GotAntidote() const;
	bool GotHandicap() const;
	bool GotJumping() const;
	bool GotRicochet() const;
	bool GotSuperFlags() const;

	bool IsFull() const;
	bool IsEmpty() const;
	int GetPlayerCount() const;

	Attribute<wxString>	name;					// server:port
	Attribute<wxString> protocolVersion;		// bzfs00057
	Attribute<wxString> longName;				// Home of DUB. Welcome. 
	
	Attribute<uint8_t>	maxPlayers;				// Integers..
	Attribute<uint16_t>	maxShots;
	Attribute<uint16_t>	shakeWins;
	Attribute<uint16_t>	shakeTimeout;
	Attribute<uint16_t>	maxPlayerScore;
	Attribute<uint16_t>	maxTeamScore;
	Attribute<uint16_t>	maxTime;

	Teams				teams;
};

inline wxString convertTowxString(const Server::Team& v) {
	return wxString::Format(_T("%d/%d"), v.count, v.max);
}

#endif
