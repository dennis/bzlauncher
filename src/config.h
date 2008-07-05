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
#ifndef __config__
#define __config__

#include <wx/gdicmn.h> // wxRect
#include <wx/arrstr.h>
#include "view.h"

class Label;

/// Manages configuration settings (load/save)
class Config {
public:
	Config();
	~Config();

	void versionCheck();

	void setBZFlagCommand(const wxString&, const wxString& = _T("default"));
	wxString getBZFlagCommand(const wxString&) const;

	wxRect getWindowDimensions() const;
	void   setWindowDimensions(wxRect);

	wxArrayString getFavorites() const;
	void setFavorites(const wxArrayString&);

	wxArrayString getListServers() const;
	wxString getListServerURL(int=0) const;

	viewlist_t getViews() const;
	void setViews(viewlist_t);

	bool getToolbarVisible();
	void setToolbarVisible(bool);

	wxArrayString getRecentServers() const;
	void setRecentServers(const wxArrayString&);

	void saveLabelSettings(Label*);
	void loadLabelSettings(Label*) const;
};

/// I prefer a global variable over static class and singleton
extern Config appConfig;

#endif
