#include "main.h"

#include "listserver.h"
#include "mainframe_impl.h"

IMPLEMENT_APP(BZLauncherApp)

bool BZLauncherApp::OnInit() {
	wxImage::AddHandler(new wxPNGHandler);

	this->mainFrame = new  MainFrameImpl(NULL);

	this->mainFrame->Show(TRUE);
	SetTopWindow(this->mainFrame);

	return TRUE;
}

void BZLauncherApp::SetStatusText(const wxString& text) {
	this->mainFrame->SetStatusText(text);
}

void BZLauncherApp::RefreshServerList() {
	ListServer	ls;
	ls.GetServerList();
}
