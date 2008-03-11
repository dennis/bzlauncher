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
