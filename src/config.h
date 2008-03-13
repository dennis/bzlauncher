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

/// Manages configuration settings (load/save)
class Config {
public:
	typedef enum {
		COL_SERVER,
		COL_NAME,
		COL_TYPE,
		COL_PLAYERS,
		COL_PING,
		COL_FAVORITE,
		COL_COUNT
	} ColType;

protected:
	int getColumnDefaultWidth(ColType) const;
	wxString getColumnName(ColType) const;
	wxString getColumnKey(ColType) const;

public:
	Config();
	~Config();

	void setBZFlagCommand(const wxString&, const wxString& = _T("default"));
	wxString getBZFlagCommand(const wxString&) const;

	int getSortMode() const;
	void setSortMode(int);

	wxRect getWindowDimensions() const;
	void   setWindowDimensions(wxRect);

	int getColumnWidth(ColType) const;
	void setColumnWidth(ColType,int);

	wxArrayString getFavorites() const;
	void setFavorites(const wxArrayString&);

	wxString getListServerURL(int=0) const;
};

/// I prefer a global variable over static class and singleton
extern Config appConfig;

#endif
