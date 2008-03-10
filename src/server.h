#ifndef __server__
#define __server__

#include <wx/string.h>
#include <wx/socket.h>

#include "serverping.h"

#ifdef _WIN32
  // Visual C++ 2008 dosn't ship with stdint.h
  typedef __int16 int16_t;
  typedef unsigned __int16 uint16_t;
  typedef __int8 int8_t;
  typedef unsigned __int8 uint8_t;
#else
# include <stdint.h>
#endif

/// BZFlag server description
struct Server {
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
	class Team {
	public:
		Team() : count(0), max(0) {};
		uint8_t count;
		uint8_t max;
	};
	wxIPV4address	ip;
	ServerPing		ping;

	wxString	serverHostPort;
	wxString	protocolVersion;
	wxString	flags;
	wxString	name;
	uint16_t	gameStyle;
	uint16_t	maxShots;
	uint16_t	shakeWins;
	uint16_t	shakeTimeout;
	uint16_t	maxPlayerScore;
	uint16_t	maxTeamScore;
	uint16_t	maxTime;
	uint8_t		maxPlayers;
	Team		rogueTeam;
	Team		redTeam;
	Team		greenTeam;
	Team		blueTeam;
	Team		purpleTeam;
	Team		observerTeam;

	bool		fullyParsed;
	bool		favorite;

	Server();

	void setIP(const wxIPV4address&);
	const wxIPV4address& getIP();

	bool IsFull() const;
	bool IsEmpty() const;
	bool IsCTF() const;
	bool IsRH() const;
	bool IsFFA() const;

	const wxString GetType() const;

	bool GotAntidote() const;
	bool GotHandicap() const;
	bool GotJumping() const;
	bool GotRicochet() const;
	bool GotSuperFlags() const;

	int GetPlayerCount() const;

	void ParseServerInfoBZFS0026(const wxString&);

private:
	int Hex2bin(char);
	char* UnpackHex16(char*, uint16_t&);
	char* UnpackHex8(char*, uint8_t&);
};

#endif
