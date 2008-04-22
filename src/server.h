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
#ifndef __server__
#define __server__

#include <wx/string.h>
#include <wx/socket.h>

#include "serverping.h"

#ifdef _WIN32
	// Visual C++ may not have the std int defines
	#ifndef __int16
		typedef __int16 int16_t;
		typedef unsigned __int16 uint16_t;
		typedef __int8 int8_t;
		typedef unsigned __int8 uint8_t;
	#endif
#else
# include <stdint.h>
#endif

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

	wxString	name;

public:
	/// Represents a team. How many players and maximum
	typedef enum { TEAM_ROGUE, TEAM_RED, TEAM_GREEN, TEAM_BLUE, TEAM_PURPLE, TEAM_OBSERVER, TEAM_COUNT } team_t;

	static team_t	TeamRogue;
	static team_t	TeamRed;
	static team_t	TeamGreen;
	static team_t	TeamBlue;
	static team_t	TeamPurple;
	static team_t	TeamObserver;
	static team_t	TeamAuto;

	class Team {
	public:
		Team() : count(0), max(0) {};
		uint8_t count;
		uint8_t max;
	};
	
	wxIPV4address	ip;
	ServerPing		ping;

	wxString	protocolVersion;
	wxString	flags;
	wxString	longName;
	uint16_t	gameStyle;
	uint16_t	maxShots;
	uint16_t	shakeWins;
	uint16_t	shakeTimeout;
	uint16_t	maxPlayerScore;
	uint16_t	maxTeamScore;
	uint16_t	maxTime;
	uint8_t		maxPlayers;

	Team		team[TEAM_COUNT];

	bool		fullyParsed;
	bool		favorite;
	bool		recent;

	Server();

	wxString getName() const;
	void     setName(const wxString&);

	void setIP(const wxIPV4address&);
	const wxIPV4address& getIP() const;

	bool IsFull() const;
	bool IsEmpty() const;
	bool IsCTF() const;
	bool IsRC() const;
	bool IsFFA() const;

	const wxString GetType() const;

	bool GotAntidote() const;
	bool GotHandicap() const;
	bool GotJumping() const;
	bool GotRicochet() const;
	bool GotSuperFlags() const;

	int GetPlayerCount() const;
};

#endif
