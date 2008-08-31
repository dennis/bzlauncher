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
			/*
			for(serverlist_t::iterator i = this->serverlist.begin(); i != this->serverlist.end(); ++i) {
				this->updateAttribute(*i, this->lblping, Attribute<uint16_t>(count));
			}
			*/
		}
		wxSleep(1);
	}
	return 0;
}

void Pinger::eventNewServer(const wxString& name, const Server& server) {
	wxLogDebug(_T("Pinger::eventNewServer(%s)"), name.c_str());
	wxMutexLocker m(this->lock);

	AttributeBase* ip = server.get(Server::ip_label);
	if(ip) {
		Attribute< wxIPV4address >* attrip = static_cast< Attribute< wxIPV4address >* >(ip);
		this->serverlist[name] = Ping(attrip->value);
	}
}
