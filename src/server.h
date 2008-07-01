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
public:
	typedef enum { team_observer, team_red, team_green, team_blue, team_purple, team_rogue, team_count } team_color_t;
	wxIPV4address	ip;

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

	class GameType {
	public:
		typedef enum { FFA, CTF, RC } gametype_t;
	private:
		gametype_t	value;
	public:
		GameType& operator=(gametype_t v) { this->value = v; return *this; };

		bool isFFA() const { return this->value == FFA; };
		bool isCTF() const { return this->value == CTF; };
		bool isRC() const { return this->value == RC; };
	};
		
	bool		fullyParsed;
	bool		favorite;
	bool		recent;

	Server();

	void setIP(const wxIPV4address&);
	const wxIPV4address& getIP() const;

	bool IsFull() const;
	bool IsEmpty() const;
	int GetPlayerCount() const;

	Attribute<wxString>	name;					// server:port
	Attribute<wxString> protocolVersion;		// bzfs00057
	Attribute<wxString> longName;				// Home of DUB. Welcome. 

	Attribute<GameType>	gameType;
	Attribute<bool>		gotSuperFlags;
	Attribute<bool>		gotJumping;
	Attribute<bool>		gotInertia;
	Attribute<bool>		gotRicochet;
	Attribute<bool>		gotShakable;
	Attribute<bool>		gotAntidote;
	Attribute<bool>		gotHandicap;

	Attribute<Ping>		ping;
	
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

inline wxString convertTowxString(const Server::GameType& v) {
	if(v.isCTF()) {
		return wxString(_("CTF"));
	}
	else if(v.isFFA()) {
		return wxString(_("FFA"));
	}
	else if(v.isRC()) {
		return wxString(_("RC")); 
	}
	else {
		return wxString(_("???"));
	}
}

#endif
