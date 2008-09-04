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
#ifndef __ping_h__
#define __ping_h__

#include <wx/socket.h>
#include <wx/list.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

DECLARE_EVENT_TYPE(wxEVT_PING_CHANGED, -1)

/// The implementation of a ping. Only one PingImpl exists pr IP to avoid
/// pinging the same IP more than once (no need) - this is the reponsibility of
/// PingTracker
class PingImpl {
public:
	wxIPV4address	ip;
	wxStopWatch		timer;
	wxSocketClient	sock;

private:
	int refcount;
	friend class Ping;
	friend class PingTracker; // FIXME This is temp!

	void measurePingStart();
	void measurePingContinue();

protected:
	typedef enum { PING_QUEUED, PING_PENDING, PING_FAILED, PING_SUCCESS } Status;
	Status			status;
	long			duration;

public:
	PingImpl();
	PingImpl(const wxIPV4address&);
	~PingImpl();

	bool isQueued() const { return this->status == PING_QUEUED; }
	bool isPending() const { return this->status == PING_PENDING; }
	bool isReady() const { return (this->status == PING_SUCCESS || this->status == PING_FAILED); }
	bool isFailed() const { return this->status == PING_FAILED; }
	bool isSuccess() const  { return this->status == PING_SUCCESS; }

	long getDuration() const {
		return this->duration;
	}

	void ping();
};

/// The server ping object. Represents a ping to a server/ip. The
/// ping itself is implemented in PingImpl and multiple Ping
/// may use the same PingImpl assuming they're pinging the same ip
/// Ping uses refcounting to control if and when to destroy PingImpl's
class Ping {	
protected:
	PingImpl*		impl;
	friend class PingTracker;
public:
	Ping();
	Ping(const wxIPV4address&);
	Ping(const Ping&);
	~Ping();

	Ping& operator=(const Ping&);
	
	bool isOK() const;
	long getDuration() const;
	void ping();
};

WX_DECLARE_LIST(PingImpl, PingList);

/// Static class that manage the pings. It controls the birth and deaths of PingImpls
class Pinger;
class PingTracker {
private:
	PingTracker() {};
protected:
	static PingList	list;
	static const int        maxpings;
	static int CountQueued();
public:
	static Pinger*	pinger;
	/// Ping a IP. This will return a PingImpl that 
	/// represents the ping to that particular ip.
	static PingImpl* Ping(const wxIPV4address &ip);

	/// Destroy a ping again
	static void Unping(class Ping * );

	/// Process pings (send out more, wait for answer..)
	static void Work();
};



/// Timer - used to make sure that PingTracker can track its progress of measuring
/// the pings
class PingTrackerTimer : public wxTimer {
public:
	void Notify();
};

inline wxString convertTowxString(const Ping& v) {
	if(v.isOK())
		return wxString::Format(_T("%d"), v.getDuration());
	else
		return (_("..."));
}

#endif
