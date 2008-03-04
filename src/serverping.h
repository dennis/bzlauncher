#ifndef __serverping_h__
#define __serverping_h__

#include <wx/socket.h>
#include <wx/list.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

class ServerPing;
class ServerPingImpl;

WX_DECLARE_LIST(ServerPingImpl, ServerPingList);

class ServerPingTracker {
private:
	ServerPingTracker() {};
protected:
	static ServerPingList	list;
	static const int        maxpings;
public:
	static ServerPingImpl* Ping(const wxIPV4address &ip);
	static void Unping(ServerPing*);
	static void Work();
};


class ServerPing {	
protected:
	ServerPingImpl*		impl;
	friend class ServerPingTracker;
public:
	ServerPing();
	ServerPing(const wxIPV4address&);
	ServerPing(const ServerPing&);
	~ServerPing();
};

class ServerPingImpl {
public:
	wxString	ip;
	//wxStopWatch		timer;

private:
	int refcount;
	friend class ServerPing;

protected:
	typedef enum { PING_QUEUED, PING_PENDING, PING_FAILED, PING_SUCCESS } Status;
	Status			status;

public:
	ServerPingImpl(const wxIPV4address& _ip) : ip(_ip.IPAddress()), status(PING_PENDING)  {}

	bool isQueued() { return this->status == PING_QUEUED; }
	bool isPending() { return this->status == PING_PENDING; }
	bool isReady()  { return (this->status == PING_SUCCESS || this->status == PING_FAILED); }
	
	void queued() {
		this->status = PING_QUEUED; 
	}
};

class ServerPingTrackerTimer : public wxTimer {
public:
	void Notify() {
		ServerPingTracker::Work();
	}
};

#endif
