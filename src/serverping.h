#ifndef __serverping_h__
#define __serverping_h__

#include <wx/socket.h>
#include <wx/list.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

DECLARE_EVENT_TYPE(wxEVT_PING_CHANGED, -1)

class ServerPing;
class ServerPingImpl;

WX_DECLARE_LIST(ServerPingImpl, ServerPingList);

/// Static class that manage the pings. It controls the birth and deaths of ServerPingImpls
class ServerPingTracker {
private:
	ServerPingTracker() {};
protected:
	static ServerPingList	list;
	static const int        maxpings;
	static int CountQueued();
public:
	/// Where should SendEvent() send its events to?
	static wxWindow*		receiver;

	/// Ping a IP. This will return a ServerPingImpl that 
	/// represents the ping to that particular ip.
	static ServerPingImpl* Ping(const wxIPV4address &ip);

	/// Destroy a ping again
	static void Unping(ServerPing*);

	/// Process pings (send out more, wait for answer..)
	static void Work();

	/// Send event to a receiver once a ping is changed
	static void SendEvent(const wxIPV4address&);
};

/// The server ping object. Represents a ping to a server/ip. The
/// ping itself is implemented in ServerPingImpl and multiple ServerPing
/// may use the same ServerPingImpl assuming they're pinging the same ip
/// ServerPing uses refcounting to control if and when to destroy ServerPingImpl's
class ServerPing {	
protected:
	ServerPingImpl*		impl;
	friend class ServerPingTracker;
public:
	ServerPing();
	ServerPing(const wxIPV4address&);
	ServerPing(const ServerPing&);
	~ServerPing();

	ServerPing& operator=(const ServerPing&);
	
	bool isOK();
	long getDuration();
	void ping();
};

/// The implementation of a ping. Only one ServerPingImpl exists pr IP to avoid
/// pinging the same IP more than once (no need) - this is the reponsibility of
/// ServerPingTracker
class ServerPingImpl {
public:
	wxIPV4address	ip;
	wxStopWatch		timer;
	wxSocketClient	sock;

private:
	int refcount;
	friend class ServerPing;
	friend class ServerPingTracker; // FIXME This is temp!

	void measurePingStart();
	void measurePingContinue();

protected:
	typedef enum { PING_QUEUED, PING_PENDING, PING_FAILED, PING_SUCCESS } Status;
	Status			status;
	long			duration;

public:
	ServerPingImpl();
	ServerPingImpl(const wxIPV4address&);
	~ServerPingImpl();

	bool isQueued() { return this->status == PING_QUEUED; }
	bool isPending() { return this->status == PING_PENDING; }
	bool isReady()  { return (this->status == PING_SUCCESS || this->status == PING_FAILED); }
	bool isFailed()  { return this->status == PING_FAILED; }
	bool isSuccess()  { return this->status == PING_SUCCESS; }

	long getDuration() {
		return this->duration;
	}

	void ping();
};

/// Timer - used to make sure that ServerPingTracker can track its progress of measuring
/// the pings
class ServerPingTrackerTimer : public wxTimer {
public:
	void Notify();
};

#endif
