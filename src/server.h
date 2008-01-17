#ifndef __server__
#define __server__

#include <wx/string.h>
#include <wx/socket.h>
#include <stdint.h>

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

struct Server {
public:

	wxString	serverHost;
	int			serverPort;
	wxString	protocolVersion;
	wxString	flags;
	wxIPV4address	ip;
	wxString	name;
	uint16_t	gameStyle;
	uint16_t	maxShots;
	uint16_t	shakeWins;
	uint16_t	shakeTimeout;
	uint16_t	maxPlayerScore;
	uint16_t	maxTeamScore;
	uint16_t	maxTime;
	uint8_t		maxPlayers;
	uint8_t		rogueCount;
	uint8_t		rogueMax;
	uint8_t		redCount;
	uint8_t		redMax;
	uint8_t		greenCount;
	uint8_t		greenMax;
	uint8_t		blueCount;
	uint8_t		blueMax;
	uint8_t		purpleCount;
	uint8_t		purpleMax;
	uint8_t		observerCount;
	uint8_t		observerMax;

	int getPlayerCount() {
		return 
			this->rogueCount+
			this->redCount+
			this->greenCount+
			this->blueCount+
			this->purpleCount;
	}

	bool isCTF() {
		return this->gameStyle & TeamFlagGameStyle;
	}

	bool isRH() {
		return this->gameStyle & RabbitChaseGameStyle;
	}

	bool isFFA() {
		return !this->isCTF() && !this->isRH();
	}

	bool gotSuperFlags() {
		return this->gameStyle & SuperFlagGameStyle;
	}

	bool gotJumping() {
		return this->gameStyle & JumpingGameStyle;
	}

	bool gotAntidote() {
		return this->gameStyle & AntidoteGameStyle;
	}

	bool gotRicochet() {
		return this->gameStyle & RicochetGameStyle;
	}

	bool gotHandicap() {
		return this->gameStyle & HandicapGameStyle;
	}

	void ParseServerInfo(const wxString&);

	Server() : gameStyle(0), maxShots(0), shakeWins(0), shakeTimeout(0), maxPlayerScore(0), maxTeamScore(0),
		maxTime(0), maxPlayers(0), rogueCount(0), rogueMax(0), redCount(0), redMax(0), greenCount(0), greenMax(0),
		blueCount(0), blueMax(0), purpleCount(0), purpleMax(0), observerCount(0), observerMax(0) {
	}
private:
	int hex2bin(char);
	char* unpackHex16(char*, uint16_t&);
	char* unpackHex8(char*, uint8_t&);
};

#endif
