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
#ifndef __listserver_h__
#define __listserver_h__

#include <wx/log.h>
#include <wx/utils.h>

class DataController;

#include "datasrc.h"

class ListServer : public DataSource {
private:
	wxString		rawResponse;
	wxArrayString	listServers;

	Label*			lblserver;
	Label*			lblip;
	Label*			lblprotocol;
	Label*			lbltext;

	Label*			lbltype;

	Label*			lblctf;
	Label*			lblrc;
	Label*			lblffa;

	Label*			lblsuperflags;
	Label*			lbljumping;
	Label*			lblinertia;
	Label*			lblantidote;
	Label*			lblshakable;
	Label*			lblricochet;
	Label*			lblhandicap;

	Label*			lblmaxshots;
	Label*			lblshakewins;
	Label*			lblshaketimeout;
	Label*			lblmaxplayerscore;
	Label*			lblmaxteamscore;
	Label*			lblmaxtime;
	Label*			lblmaxplayers;

	Label*			lblroguecount;
	Label*			lblroguemax;

	Label*			lblredcount;
	Label*			lblredmax;

	Label*			lblgreencount;
	Label*			lblgreenmax;

	Label*			lblbluecount;
	Label*			lblbluemax;

	Label*			lblpurplecount;
	Label*			lblpurplemax;

	Label*			lblobservercount;
	Label*			lblobservermax;

public:
	ListServer(const wxArrayString&);
	~ListServer();

	void initializeLabels(DataController*);

	ExitCode Entry();
	bool GetListServerResponse();
	bool ParseLine(const wxString&);
	void GetServerList();

	void ServerHexParserBZFS0026(const wxString&,const wxString&);
};

#endif
