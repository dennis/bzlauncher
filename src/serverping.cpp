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
#include "serverping.h"
#include <wx/listimpl.cpp>
#include <wx/log.h>

WX_DEFINE_LIST(ServerPingList);
DEFINE_EVENT_TYPE(wxEVT_PING_CHANGED)

ServerPing::ServerPing() : impl(NULL)  {
}

ServerPing::ServerPing(const wxIPV4address& ip) : impl(ServerPingTracker::Ping(ip)) {
	this->impl->refcount++;
	assert(this->impl->refcount);
}

ServerPing::ServerPing(const ServerPing& s) : impl(s.impl) { 
	this->impl->refcount++;
	assert(this->impl->refcount);
}

ServerPing::~ServerPing() { 
	assert(this->impl->refcount);
	this->impl->refcount--;
	if( this->impl && this->impl->refcount == 0) {
		ServerPingTracker::Unping(this);
		delete impl; 
	}
}

ServerPing& ServerPing::operator=(const ServerPing& s) {
	this->impl = s.impl;
	this->impl->refcount++;
	assert(this->impl->refcount);
	
	return *this;
}

bool ServerPing::isOK() {
	return this->impl->isSuccess();
}

long ServerPing::getDuration() {
	return this->impl->getDuration();
}

void ServerPing::ping() {
	assert(this->impl);
	this->impl->ping();
}

ServerPingList ServerPingTracker::list;
const int ServerPingTracker::maxpings = 5;
wxWindow* ServerPingTracker::receiver = NULL;

ServerPingImpl* ServerPingTracker::Ping(const wxIPV4address &ip) {
	ServerPingImpl*	ping = NULL;
	wxString ipReal = ip.IPAddress();

	// Try to find existing ping object for requested ip
	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); i != ServerPingTracker::list.end(); ++i) {
		ping = *i;
		if(ipReal.compare(ping->ip.IPAddress()) == 0) {
			return ping;

		}
	}

	// None found, add new
	ping = new ServerPingImpl(ip);
	ServerPingTracker::list.push_back(ping);

	return ping;
}

void ServerPingTracker::Unping(ServerPing* ping) {
	ServerPingTracker::list.DeleteObject(ping->impl);
}

void ServerPingTracker::Work() {
	int queued = ServerPingTracker::CountQueued();

	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); i != ServerPingTracker::list.end(); ++i) {
		// Start pinging more IPs, if queued is below maxpings
		if((*i)->isPending() && queued < ServerPingTracker::maxpings ) {
			(*i)->measurePingStart();
			queued++;
		}
		else if((*i)->isQueued()) {
			(*i)->measurePingContinue();
		}
	}
}

int ServerPingTracker::CountQueued() {
	int queued = 0;
	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); i != ServerPingTracker::list.end(); ++i) {
		if((*i)->isQueued())
			queued++;
	}

	return queued;
}

ServerPingImpl::ServerPingImpl() : refcount(0), status(PING_FAILED)  {
}
ServerPingImpl::ServerPingImpl(const wxIPV4address& _ip) : ip(_ip), refcount(0), status(PING_PENDING)  {
}

ServerPingImpl::~ServerPingImpl() {
}

void ServerPingImpl::measurePingStart() {
	this->ping();
}

void ServerPingImpl::measurePingContinue() {
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
		ServerPingTracker::SendEvent(this->ip);
	}
}

void ServerPingImpl::ping() {
	this->timer.Start();
	this->sock.Connect(ip,false);
	this->sock.SetFlags(wxSOCKET_NOWAIT);
	this->sock.SetTimeout(2);
	this->status = PING_QUEUED;
}

void ServerPingTrackerTimer::Notify() {
	ServerPingTracker::Work();
}

void ServerPingTracker::SendEvent(const wxIPV4address& ip) {
	wxCommandEvent event(wxEVT_PING_CHANGED);
	event.SetString(wxString::Format(_T("%s:%ld"), ip.IPAddress().c_str(), ip.Service()));
	ServerPingTracker::receiver->GetEventHandler()->ProcessEvent(event);
}
