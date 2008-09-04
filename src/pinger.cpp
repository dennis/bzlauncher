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
#include <wx/log.h>
#include <wx/intl.h>

#include "pinger.h"
#include "datactrl.h"

Pinger::Pinger() {
	wxLogDebug(_T("Pinger::out_queue = %p"), &this->out_queue);
	PingTracker::pinger = this;
}

void Pinger::initializeLabels(DataController* datactrl) {
	wxLogDebug(_T("Pinger::initializeLabels(datactrl)"));

	// Ownership is transferred to ctrl, so we dont need to free them
	datactrl->addLabel(this->lblping   = new Label(_T("ping"),      _("Ping")));
}

Pinger::ExitCode Pinger::Entry() {
	while(!this->TestDestroy()) {
		{
			wxMutexLocker m(this->lock);
			PingTracker::Work();
		}
		wxMilliSleep(2);
	}
	return 0;
}

void Pinger::eventNewServer(const wxString& name, const Server& server) {
	wxLogDebug(_T("Pinger::eventNewServer(%s)"), name.c_str());
	wxMutexLocker m(this->lock);

	AttributeBase* serverip   = server.get(Server::ip_label);
	AttributeBase* servername = server.get(Server::equality_label);
	if(serverip && servername) {
		Attribute<wxIPV4address>* attrip = static_cast< Attribute< wxIPV4address >* >(serverip);
		wxString theip = attrip->value.IPAddress();
		if(this->addresslist.find(theip) == this->addresslist.end())  {
			// New ip to ping
			this->addresslist[theip].first = Ping(attrip->value);
		}
		else if(this->addresslist[theip].first.isOK()) {
			// We already got the result ready.
			this->newPingResultNoLockHelper(attrip->value, this->addresslist[theip].first.getDuration());
		}

		this->addresslist[theip].second.push_back(servername->aswxString());
		wxLogDebug(_T(" added: %s -> %s. IP got %d entries. Total IPs %d"), theip.c_str(), servername->aswxString().c_str(), this->addresslist[theip].second.size(), this->addresslist.size());
	}
}

void Pinger::eventNewPingResult(const wxIPV4address& ip,long duration) {
	//FIXME wxMutexLocker m(this->lock);
	newPingResultNoLockHelper(ip,duration);
}

void Pinger::newPingResultNoLockHelper(const wxIPV4address& ip, long duration) {
	wxString theip = ip.IPAddress();
	wxLogDebug(_T("PING for %s is %ld"), theip.c_str(), duration);

	if(this->addresslist.find(theip) != this->addresslist.end()) {
		wxLogDebug(_T(" Found %d servers"), this->addresslist[theip].second.size());
		for(serverlist_t::iterator i = this->addresslist[theip].second.begin(); i != this->addresslist[theip].second.end(); ++i) {
			this->updateAttribute(*i, this->lblping, Attribute<uint16_t>(duration));
		}
	}
	else {
		wxLogDebug(_T("Response for a ping we don't care about?"));
	}
}

