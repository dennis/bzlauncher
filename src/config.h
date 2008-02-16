#ifndef __config__
#define __config__

#include <wx/config.h>
#include <wx/gdicmn.h> // wxRect
#include <wx/arrstr.h>

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
	wxConfig*	cfg;

	int getColumnDefaultWidth(ColType) const;
	wxString getColumnName(ColType) const;
	wxString getColumnKey(ColType) const;

public:
	
	Config();
	~Config();

	wxString getBZFlagCommand(const wxString&) const;

	int getSortMode() const;
	void setSortMode(int);

	wxRect getWindowDimensions() const;
	void   setWindowDimensions(wxRect);

	int getColumnWidth(ColType) const;
	void setColumnWidth(ColType,int);

	wxArrayString getFavorites() const;
	void setFavorites(const wxArrayString&);
};

extern Config appConfig;

#endif
