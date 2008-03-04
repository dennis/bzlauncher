#include "serverping.h"
#include <wx/listimpl.cpp>
#include <wx/log.h>

WX_DEFINE_LIST(ServerPingList);

ServerPing::ServerPing() : impl(NULL) {
}

ServerPing::ServerPing(const wxIPV4address& ip) : impl(ServerPingTracker::Ping(ip)) {
}

ServerPing::ServerPing(const ServerPing& s) : impl(s.impl) { 
	if( this->impl ) this->impl->refcount++; 
}

ServerPing::~ServerPing() { 
	if( this->impl && --this->impl->refcount == 0) {
		wxLogMessage(_T("Unpining %lx count %d"), (long int)this, this->impl->refcount);
		ServerPingTracker::Unping(this);
		delete impl; 
	}
}

ServerPingList ServerPingTracker::list;
const int ServerPingTracker::maxpings = 5;

ServerPingImpl* ServerPingTracker::Ping(const wxIPV4address &ip) {
	ServerPingImpl*	ping = NULL;
	wxString ipReal = ip.IPAddress();

	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); i != ServerPingTracker::list.end(); ++i) {
		ping = *i;
		if(ipReal.compare(ping->ip) == 0) {
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
	int queued = 0;
	ServerPingImpl*	ping = NULL;

	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); i != ServerPingTracker::list.end(); ++i) {
		ping = *i;

		if(ping->isQueued()) 
			queued++;
	}

	for(ServerPingList::iterator i = ServerPingTracker::list.begin(); queued < maxpings && i != ServerPingTracker::list.end(); ++i) {
		ping = *i;

		if(ping->isPending()) {
			wxLogMessage(_T("I want to ping %s"), ping->ip.c_str());
			queued++;
			ping->queued();
		}
	}
}
