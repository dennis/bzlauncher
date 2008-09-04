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
#include "ping.h"
#include "pinger.h"

#include <wx/listimpl.cpp>
#include <wx/log.h>

WX_DEFINE_LIST(PingList);
DEFINE_EVENT_TYPE(wxEVT_PING_CHANGED)

Ping::Ping() : impl(NULL)  {
}

Ping::Ping(const wxIPV4address& ip) : impl(PingTracker::Ping(ip)) {
	this->impl->refcount++;
	assert(this->impl->refcount);
}

Ping::Ping(const Ping& s) : impl(s.impl) { 
	if(this->impl) {
		this->impl->refcount++;
		assert(this->impl->refcount);
	}
}

Ping::~Ping() { 
	if( this->impl ) {
		assert(this->impl->refcount);
		this->impl->refcount--;
		if( this->impl && this->impl->refcount == 0) {
			PingTracker::Unping(this);
			delete impl; 
		}
	}
}

Ping& Ping::operator=(const Ping& s) {
	this->impl = s.impl;
	if(this->impl) {
		this->impl->refcount++;
		assert(this->impl->refcount);
	}
	return *this;
}

bool Ping::isOK() const {
	if(this->impl)
		return this->impl->isSuccess();
	else
		return true;
}

long Ping::getDuration() const {
	if(this->impl)
		return this->impl->getDuration();
	else
		return 0;
}

void Ping::ping() {
	assert(this->impl);
	this->impl->ping();
}

PingList PingTracker::list;
const int PingTracker::maxpings = 5;
Pinger* PingTracker::pinger = NULL;

PingImpl* PingTracker::Ping(const wxIPV4address &ip) {
	PingImpl*	ping = NULL;
	wxString ipReal = ip.IPAddress();
	wxLogDebug(_T("PingTracker::Ping() - %s"), ipReal.c_str());

	// Try to find existing ping object for requested ip
	for(PingList::iterator i = PingTracker::list.begin(); i != PingTracker::list.end(); ++i) {
		ping = *i;
		if(ipReal.compare(ping->ip.IPAddress()) == 0) {
			wxLogDebug(_T(" - Already pinging it, reusing.."));
			return ping;
		}
	}

	wxLogDebug(_T("Creating new PingImpl"));
	// None found, add new
	ping = new PingImpl(ip);
	PingTracker::list.push_back(ping);

	return ping;
}

void PingTracker::Unping(class Ping* ping) {
	PingTracker::list.DeleteObject(ping->impl);
}

void PingTracker::Work() {
	int queued = PingTracker::CountQueued();

	for(PingList::iterator i = PingTracker::list.begin(); i != PingTracker::list.end(); ++i) {
		// Start pinging more IPs, if queued is below maxpings
		if((*i)->isPending() && queued < PingTracker::maxpings ) {
			(*i)->measurePingStart();
			queued++;
		}
		else if((*i)->isQueued()) {
			(*i)->measurePingContinue();
		}
	}
}

int PingTracker::CountQueued() {
	int queued = 0;
	for(PingList::iterator i = PingTracker::list.begin(); i != PingTracker::list.end(); ++i) {
		if((*i)->isQueued())
			queued++;
	}

	return queued;
}

PingImpl::PingImpl() : refcount(0), status(PING_FAILED)  {
}
PingImpl::PingImpl(const wxIPV4address& _ip) : ip(_ip), refcount(0), status(PING_PENDING)  {
}

PingImpl::~PingImpl() {
}

void PingImpl::measurePingStart() {
	this->ping();
}

void PingImpl::measurePingContinue() {
	wxASSERT(PingTracker::pinger);
	if( this->sock.WaitOnConnect(0,0) ) {
		// Connected
		if(this->sock.IsConnected()) {
			this->status = PING_SUCCESS;
			this->timer.Pause();
			this->duration = this->timer.Time();
		}
		else {
			this->status = PING_FAILED;
			this->duration = 9999;
		}
		PingTracker::pinger->eventNewPingResult(this->ip, this->duration);
	}
}

void PingImpl::ping() {
	this->timer.Start();
	this->sock.Connect(ip,false);
	this->sock.SetFlags(wxSOCKET_NOWAIT);
	this->sock.SetTimeout(2);
	this->status = PING_QUEUED;
}

void PingTrackerTimer::Notify() {
	PingTracker::Work();
}
