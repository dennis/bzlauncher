#include <wx/config.h>

#include "config.h"

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
	wxConfig* cfg = new wxConfig(_T("bzlauncher")); \
	{ op } while(0); \
	delete cfg; \
	cfg = NULL;

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


void Config::setBZFlagCommand(const wxString& cmd, const wxString& proto) {
	CFG_OP(cfg, cfg->Write(wxString::Format(_T("bzflag/%s"), proto.c_str()), cmd););
}

int Config::getSortMode() const {
	int mode;
	CFG_OP(cfg, mode = cfg->Read(_T("window/sortmode"), -4); );
	return mode;
}

void Config::setSortMode(int s) {
	CFG_OP(cfg, cfg->Write(_T("window/sortmode"), s); );
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

wxString Config::getColumnName(ColType t) const {
	static const wxString names [] = {
		_T("server"),
		_T("name"),
		_T("type"),
		_T("players"),
		_T("ping"),
		_T("favorite")
	};

	return names[t];
}

wxString Config::getColumnKey(ColType type) const {
	wxString key;
	key += _T("window/col_");
	key += this->getColumnName(type);
	key += _T("_width");
	return key;
}

int Config::getColumnDefaultWidth(ColType t) const {
	const int widths[] = {
		157,
		300,
		47,
		30,
		46,
		46
	};

	return widths[t];
}

int Config::getColumnWidth(ColType type) const {
	int w;
	wxString key = this->getColumnKey(type);
	CFG_OP(cfg,
		w = cfg->Read(key, this->getColumnDefaultWidth(type));
	);
	return w;
}

void Config::setColumnWidth(ColType type, int w) {
	wxString key = this->getColumnKey(type);
	CFG_OP(cfg, cfg->Write(key,w););
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
