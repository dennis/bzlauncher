#ifndef __serverping_h__
#define __serverping_h__

#include <wx/socket.h>
#include <wx/list.h>
#include <wx/stopwatch.h>

class ServerPing;

WX_DECLARE_LIST(ServerPing, ServerPingList);

class ServerPing {
private:
	enum { PING_FAILED, PING_PENDING, PING_SUCCESS } status;
public:
	wxIPV4address	ip;
	wxStopWatch		timer;

	bool isReady() {
		return this->status == PING_SUCCESS;
	}
};

class ServerPingTracker {
protected:
	ServerPingList	list;
public:
	const ServerPing* Ping(const wxIPV4address &ip);	// If not exists, add ServerPing to list and begin pinging it.
};
#endif
