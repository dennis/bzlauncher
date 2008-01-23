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

	return TRUE;
}

void BZLauncherApp::SetStatusText(const wxString& text) {
	this->mainFrame->SetStatusText(text);
}

void BZLauncherApp::RefreshServerList() {
	this->listServerHandler.GetServerList();
}

Server* BZLauncherApp::GetSelectedServer() {
	return this->selectedServer;
}

void BZLauncherApp::SetSelectedServer(Server* s) {
	this->selectedServer = s;
}

void BZLauncherApp::LaunchSelectedServer() {
	if(! this->selectedServer) {
		this->SetStatusText(_("No server selected!"));
		return;
	}

	const wxString cmd = _T("bzflag %s");

	::wxShell(wxString::Format(cmd,this->selectedServer->serverHostPort.c_str()));
}
