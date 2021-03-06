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
#ifndef __pinger_h__
#define __pinger_h__

#include <wx/log.h>
#include <wx/utils.h>
#include <wx/thread.h>
#include <map>
#include <vector>

class DataController;

#include "datasrc.h"
#include "ping.h"

class Pinger : public DataSource {
private:
	Label*			lblping;

	typedef std::vector<wxString>	serverlist_t;
	typedef std::map<wxString, std::pair< Ping, serverlist_t > > addresslist_t;
	addresslist_t					addresslist;

	wxMutex	lock;
public:
	Pinger();

	void initializeLabels(DataController*);

	ExitCode Entry();

	void eventNewServer(const wxString&, const Server&);
	void eventNewPingResult(const wxIPV4address&,long);
	void newPingResultNoLockHelper(const wxIPV4address&, long);
	
};

#endif
