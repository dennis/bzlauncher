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

	SetTopWindow(this->mainFrame);

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
