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

class ServerPingTracker {
private:
	ServerPingTracker() {};
protected:
	static ServerPingList	list;
	static const int        maxpings;
public:
	static wxWindow*		receiver;
	static ServerPingImpl* Ping(const wxIPV4address &ip);
	static void Unping(ServerPing*);
	static void Work();
	static int CountQueued();
	static void SendEvent(const wxIPV4address&);
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

	ServerPing& operator=(const ServerPing&);
	
	bool isOK();
	long getDuration();
};

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
};

class ServerPingTrackerTimer : public wxTimer {
public:
	void Notify();
};

#endif
