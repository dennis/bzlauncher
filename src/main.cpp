#include "main.h"

#include "listserverhandler.h"
#include "server.h"
#include "mainframe_impl.h"

IMPLEMENT_APP(BZLauncherApp)

bool BZLauncherApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);

	this->mainFrame = new  MainFrameImpl(NULL);
	this->mainFrame->RefreshServerGrid();
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
	if(this->selectedServerHostPort.IsEmpty()) {
		this->SetStatusText(_("No server selected!"));
		return;
	}

	const wxString cmd = _T("bzflag %s");

	::wxShell(wxString::Format(cmd,this->selectedServerHostPort.c_str()));
}
