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

	appConfig.versionCheck();

	this->mainFrame = new  MainFrameImpl(NULL);
	this->mainFrame->Show(true);

	this->SetTopWindow(this->mainFrame);

	this->SetAppName(_T("BZLauncher"));

	return TRUE;
}

void BZLauncherApp::SetStatusText(const wxString& text) {
	this->mainFrame->SetStatusText(text);
}

void BZLauncherApp::RefreshServerList(wxArrayString& favs, wxArrayString& recents) {
	this->listServerHandler.GetServerList(favs, recents);
}

const wxString BZLauncherApp::GetSelectedServer() {
	return this->selectedServerHostPort;
}

void BZLauncherApp::SetSelectedServer(const wxString& s) {
	this->selectedServerHostPort = s;
}

#ifdef _WINDOWS
void BZLauncherApp::LaunchSelectedServer(wxWindow* w, Server::team_t team) {
#else
void BZLauncherApp::LaunchSelectedServer(wxWindow*, Server::team_t team) {
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

	wxString	args;

	if(team == Server::TEAM_RED) args += _T("-team red ");
	if(team == Server::TEAM_GREEN) args += _T("-team green ");
	if(team == Server::TEAM_BLUE) args += _T("-team blue ");
	if(team == Server::TEAM_PURPLE) args += _T("-team purple ");
	if(team == Server::TEAM_ROGUE) args += _T("-team rogue ");
	if(team == Server::TEAM_OBSERVER) args += _T("-team observer ");

	args += this->selectedServerHostPort;

	// Launch BZFlag
	wxSetWorkingDirectory(wxFileName::FileName(cmd).GetPath());
	::wxExecute(wxString::Format(cmd,args.c_str()),wxEXEC_SYNC);
}
