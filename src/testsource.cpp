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

#include "testsource.h"
#include "datactrl.h"

TestSource::TestSource() {
	wxLogDebug(_T("TestSource::out_queue = %p"), &this->out_queue);
}

void TestSource::initializeLabels(DataController* datactrl) {
	wxLogDebug(_T("TestSource::initializeLabels(datactrl)"));
	// Ownership is transferred to ctrl, so we dont need to free them
	datactrl->addLabel(this->lblsequence   = new Label(_T("dbgcount"),      _("DBG Count")));
}

TestSource::ExitCode TestSource::Entry() {
	uint16_t count = 0;
	while(!this->TestDestroy()) {
		for(serverlist_t::iterator i = this->serverlist.begin(); i != this->serverlist.end(); ++i) {
			this->updateAttribute(*i, this->lblsequence, Attribute<uint16_t>(count));
		}
		wxSleep(5);
		count++;
	}
	return 0;
}

void TestSource::eventNewServer(const wxString& server) {
	wxLogDebug(_T("TestSource::eventNewServer(%s)"), server.c_str());
	this->serverlist.push_back(server);
}
