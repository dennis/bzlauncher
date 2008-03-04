#include "main.h"

#include <wx/fs_inet.h>

#include "config.h"
#include "listserverhandler.h"
#include "server.h"
#include "mainframe_impl.h"

IMPLEMENT_APP(BZLauncherApp)

BZLauncherApp::~BZLauncherApp() {
}

bool BZLauncherApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);
	wxFileSystem::AddHandler(new wxInternetFSHandler);

	this->mainFrame = new  MainFrameImpl(NULL);
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

#ifdef _WINDOWS
void BZLauncherApp::LaunchSelectedServer(wxWindow* w) {
#else
void BZLauncherApp::LaunchSelectedServer(wxWindow*) {
#endif
	Server* server = this->listServerHandler.FindByName(this->selectedServerHostPort);

	if(this->selectedServerHostPort.IsEmpty() || !server ) {
		this->SetStatusText(_("No server selected!"));
		return;
	}

	wxString cmd = appConfig.getBZFlagCommand(server->protocolVersion);
	if(cmd.IsEmpty()) {	// We dont know how to launch it
#ifdef _WINDOWS
		// Since BZFlag isnt likely to be in PATH, we need to explicit
		// ask where it is
		wxFileDialog dia(w,_("Choose the BZFlag executable"), _T("c:/"),
			_T("bzflag.exe"), _T("*.exe"), wxFD_OPEN|wxFD_FILE_MUST_EXIST);

		if(dia.ShowModal() == wxID_OK) {
			cmd = dia.GetDirectory() + _T("\\") + dia.GetFilename();

			cmd += _T(" %s");

			// Save it before we move on
			appConfig.setBZFlagCommand(cmd);
		}
#else 
		// We assume BZFlag is in PATH
		cmd = _T("bzflag %s");
		appConfig.setBZFlagCommand(cmd);
#endif
	}
	wxSetWorkingDirectory(wxFileName::FileName(cmd).GetPath());
	::wxExecute(wxString::Format(cmd,this->selectedServerHostPort.c_str()),wxEXEC_SYNC);
}
