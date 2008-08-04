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
#include <wx/fileconf.h>
#include <wx/log.h>

#include "config.h"
#include "label.h"
#include "queryresult.h"

Config appConfig;

Config::Config() {
}

Config::~Config() {
}

// FIXME - i hate to use this
// This is a macro that will allocate and deallocate the wxConfig, since
// our app crashes if we let it live too long (for some reason). This
// is a temporary hack, which should be fixed ASAP.
#define CFG_OP(cfg,op) \
	{ wxFileConfig* cfg = new wxFileConfig(_T("bzlauncher")); \
	 op \
	 delete cfg; \
	 cfg = NULL; \
	} while(0);

wxString Config::getBZFlagCommand(const wxString& proto) const {
	// first try bzflag/proto (eg bzflag/bzfs0026)
	// then try bzflag/default
	wxString key = wxString::Format(_T("bzflag/%s"), proto.Lower().c_str());
	wxString cmd;

	CFG_OP(cfg,
		if(! cfg->Read(key, &cmd)) {
			if(! cfg->Read(_T("bzflag/default"), &cmd)) {
				return _T("");
			}
		}
	);

	return cmd;
}

void Config::versionCheck() {
	int version = 0;
	CFG_OP(cfg,
		cfg->Read(_T("cfgversion"), &version);

		// Migrate if needed
		switch(version) {
			case 0:
				{
					long sortmode = 0;
					cfg->Read(_T("window/sortmode"), &sortmode);
					cfg->DeleteEntry(_T("window/sortmode"));

					cfg->Write(_T("cfgversion"), 1);
				}
				break;

			case 1:
				// Current
				break;
		}
	);
}

void Config::setBZFlagCommand(const wxString& cmd, const wxString& proto) {
	CFG_OP(cfg, cfg->Write(wxString::Format(_T("bzflag/%s"), proto.c_str()), cmd););
}

wxRect Config::getWindowDimensions() const {
	wxRect wanted; 

	CFG_OP(cfg,
		wanted.x      = cfg->Read(_T("window/x"), 10);
		wanted.y      = cfg->Read(_T("window/y"), 10);
		wanted.width  = cfg->Read(_T("window/w"), 600);
		wanted.height = cfg->Read(_T("window/h"), 600);
	);
	return wanted;
}

void Config::setWindowDimensions(wxRect r) {
	CFG_OP(cfg,
		cfg->Write(_T("window/x"), r.x);
		cfg->Write(_T("window/y"), r.y);
		cfg->Write(_T("window/h"), r.height);
		cfg->Write(_T("window/w"), r.width);
	);
}

wxArrayString Config::getFavorites() const {
	wxString      str;
	wxArrayString list;
	int           count = 0;

	CFG_OP(cfg,
		while(cfg->Read(wxString::Format(_T("favorites/%d"), count), &str)) {
			list.Add(str);
			count++;
		}
	);

	return list;
}

void Config::setFavorites(const wxArrayString& list) {
	CFG_OP(cfg,
		for(unsigned int i = 0; i < list.GetCount(); i++) 
			cfg->Write(wxString::Format(_T("favorites/%d"), i), list.Item(i));
	);
}

wxArrayString Config::getListServers() const {
	wxArrayString	list;
	wxString		str;
	int 			count = 0;

	CFG_OP(cfg,
		while(cfg->Read(wxString::Format(_T("listserver/%d"), count), &str)) {
			list.Add(str);
			count++;
		}
	);

	if(count == 0) 
		list.Add(_T("http://my.bzflag.org/db?action=LIST"));

	return list;
}

wxString Config::getListServerURL(int n) const {
	int count = 0;
	wxString str;
	CFG_OP(cfg,
		while(cfg->Read(wxString::Format(_T("listserver/%d"), count), &str)) {
			if(count == n) return str;
			count++;
		}
	);

	if(count == 0 && n == 1) 
		return _T("http://bzstats.strayer.de/stuff/listserver.php");
	else
		return _T("http://my.bzflag.org/db?action=LIST");
}

viewlist_t Config::getViews() const {
	int count = 0;
	
	wxString query;
	long sortmode = 0;

	viewlist_t	list;

	CFG_OP(cfg,
		while(
			cfg->Read(wxString::Format(_T("views/%d.query"), count), &query) &&
			cfg->Read(wxString::Format(_T("views/%d.sortmode"), count), &sortmode)
		) {
			// Read it into serverlist-thingy
			ServerListView*	view = new ServerListView(Query(query), sortmode);
			list.push_back(view);
			count++;
		}

		// Make sure we got at least one view
		if(list.size() == 0) {
			list.push_back(new ServerListView(Query(_T("All")), sortmode));
			list.push_back(new ServerListView(Query(_T("CTF")), sortmode));
			list.push_back(new ServerListView(Query(_T("FFA")), sortmode));
			list.push_back(new ServerListView(Query(_T("RC")), sortmode));
		}

	);

	return list;
}

void Config::setViews(viewlist_t list) {
	int count = 0;

	wxString query;
	long sortmode;

	viewlist_t old_list = this->getViews();

	CFG_OP(cfg,
		for(viewlist_t::iterator i = list.begin(); i != list.end(); ++i ) {
			sortmode = (*i)->currentSortMode;
			query = (*i)->query.get();
			cfg->Write(wxString::Format(_T("views/%d.query"), count), query);
			cfg->Write(wxString::Format(_T("views/%d.sortmode"), count), sortmode);
			count++;
		}
		for(;count < old_list.size(); count++) {
			cfg->DeleteEntry(wxString::Format(_T("views/%d.query"), count));
			cfg->DeleteEntry(wxString::Format(_T("views/%d.sortmode"), count));
		}
	);
}

bool Config::getToolbarVisible() {
	bool rc = true;

	CFG_OP(cfg,
		cfg->Read(_T("window/toolbar"), &rc);
	);

	return rc;
}

void Config::setToolbarVisible(bool b) {
	CFG_OP(cfg,
		cfg->Write(_T("window/toolbar"), b);
	);
}

wxArrayString Config::getRecentServers() const {
	wxLogDebug(_T("getRecentservers()"));
	wxString      str;
	wxArrayString list;
	int           count = 0;

	CFG_OP(cfg,
		while(cfg->Read(wxString::Format(_T("recent/%d"), count), &str)) {
			wxLogDebug(str);
			list.Insert(str,0);
			count++;
		}
	);

	return list;
}

void Config::setRecentServers(const wxArrayString& list) {
	CFG_OP(cfg,
		for(unsigned int i = 0; i < list.GetCount(); i++ ) 
			cfg->Write(wxString::Format(_T("recent/%d"), i), list.Item(i));
	);
}

void Config::loadLabelSettings(Label* label) const {
	wxString val;
	long labelwidth = 100;
	long labelpos   = -1;
	wxLogDebug(_T("loadLabelSettings(%s)"), label->getTag().c_str());
	CFG_OP(cfg,
		if( cfg->Read(wxString::Format(_T("label/%s"), label->getTag().c_str()), &val) ) {
			int pos = val.Find(',',true);
			if( pos != wxNOT_FOUND ) {
				val.Mid(0,pos).ToLong(&labelpos);
				val.Mid(pos+1).ToLong(&labelwidth);
			}
		}
	);

	label->setWidth(labelwidth);
	label->setPos(labelpos);
}

void Config::saveLabelSettings(Label* label) {
	wxLogDebug(_T("saveLabelSettings(%s)"), label->getTag().c_str());

	// No reason to store anything, if its the default values
	if( label->getPos() == -1 && label->getWidth() == 100 )
		return;

	CFG_OP(cfg,
		cfg->Write(wxString::Format(_T("label/%s"), label->getTag().c_str()), wxString::Format(_T("%d,%d"), label->getPos(), label->getWidth() ));
	);
}
