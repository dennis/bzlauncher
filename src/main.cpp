#include "main.h"

#include "listserverhandler.h"
#include "server.h"
#include "mainframe_impl.h"

IMPLEMENT_APP(BZLauncherApp)

BZLauncherApp::~BZLauncherApp() {
	delete this->mainFrame;
}

bool BZLauncherApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);

	this->iconsDirectory = wxString::Format(_T("%s/icons/"), wxGetCwd().c_str());

	this->mainFrame = new  MainFrameImpl(NULL);
	//this->mainFrame->RefreshServerGrid();
	this->mainFrame->Show(true);

	this->SetTopWindow(this->mainFrame);

	this->SetAppName(_T("BZLauncher"));
	return TRUE;
}

void BZLauncherApp::SetStatusText(const wxString& text) {
	this->mainFrame->SetStatusText(text);
}

void BZLauncherApp::RefreshServerList() {
	this->listServerHandler.GetServerList();
}

const wxString BZLauncherApp::GetSelectedServer() {
	return this->selectedServerHostPort;
}

void BZLauncherApp::SetSelectedServer(const wxString& s) {
	this->selectedServerHostPort = s;
}

void BZLauncherApp::LaunchSelectedServer() {
	Server* server = this->listServerHandler.FindByName(this->selectedServerHostPort);

	if(this->selectedServerHostPort.IsEmpty() || !server ) {
		this->SetStatusText(_("No server selected!"));
		return;
	}

	const wxString defaultCmd = _T("xmessage %s");
	wxString cmd = defaultCmd;

	// Read configuration
	wxConfig* cfg = new wxConfig(_T("bzlauncher"));
	wxString key = wxString::Format(_T("bzflag/%s"), server->protocolVersion.Lower().c_str());

	if(! cfg->Read(key, &cmd))
		cfg->Read(_T("bzflag/default"), &cmd);
	delete cfg;

	// Execute!
	::wxShell(wxString::Format(cmd,this->selectedServerHostPort.c_str()));
}
