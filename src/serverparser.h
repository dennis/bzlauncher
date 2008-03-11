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
public:
	void parse(const wxString, Server&);
};

#endif
