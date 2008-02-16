#include "config.h"

Config appConfig;

Config::Config() {
	this->cfg = new wxConfig(_T("bzlauncher"));
}

Config::~Config() {
	delete this->cfg;
}

wxString Config::getBZFlagCommand(const wxString& proto) const {
	// first try bzflag/proto (eg bzflag/bzfs0026)
	// then try bzflag/default
	// then use default ("bzflag %s")
	const wxString defaultCmd = _T("bzflag %s");

	wxString key = wxString::Format(_T("bzflag/%s"), proto.Lower().c_str());
	wxString cmd;

	if(! this->cfg->Read(key, &cmd))
		if(! this->cfg->Read(_T("bzflag/default"), &cmd))
			cmd = defaultCmd;

	return cmd;
}

int Config::getSortMode() const {
	return this->cfg->Read(_T("window/sortmode"), -4);
}

void Config::setSortMode(int s) {
	this->cfg->Write(_T("window/sortmode"), s);
}

wxRect Config::getWindowDimensions() const {
	wxRect wanted; 

	wanted.x      = this->cfg->Read(_T("window/x"), 10);
	wanted.y      = this->cfg->Read(_T("window/y"), 10);
	wanted.width  = this->cfg->Read(_T("window/w"), 600);
	wanted.height = this->cfg->Read(_T("window/h"), 600);

	return wanted;
}

void Config::setWindowDimensions(wxRect r) {
	this->cfg->Write(_T("window/x"), r.x);
	this->cfg->Write(_T("window/y"), r.y);
	this->cfg->Write(_T("window/h"), r.height);
	this->cfg->Write(_T("window/w"), r.width);
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
	wxString key = this->getColumnKey(type);
	return this->cfg->Read(key, this->getColumnDefaultWidth(type));
}

void Config::setColumnWidth(ColType type, int w) {
	wxString key = this->getColumnKey(type);
	this->cfg->Write(key,w);
}

wxArrayString Config::getFavorites() const {
	wxString      str;
	wxArrayString list;
	int           count = 0;

	while(this->cfg->Read(wxString::Format(_T("favorites/%d"), count), &str)) {
		list.Add(str);
		count++;
	}

	return list;
}

void Config::setFavorites(const wxArrayString& list) {
	for(unsigned int i = 0; i < list.GetCount(); i++) 
		this->cfg->Write(wxString::Format(_T("favorites/%d"), i), list.Item(i));
}
