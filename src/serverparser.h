#ifndef __serverparser__
#define __serverparser__

#include "server.h"

class ServerHexParser {
public:
	static ServerHexParser* GetParser(const wxString&);

	virtual ~ServerHexParser() {};

	virtual void parse(const wxString, Server&) = 0;
};

class ServerHexParserBZFS0026 : public ServerHexParser {
public:
	void parse(const wxString, Server&);
};

#endif
