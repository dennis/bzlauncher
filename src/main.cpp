#include "main.h"

#include "listserver.h"

IMPLEMENT_APP(BZLauncherApp)

bool BZLauncherApp::OnInit() {
	this->mainFrame = new  MainFrameImpl(NULL);

	this->mainFrame->Show(TRUE);
	SetTopWindow(this->mainFrame);

	ListServer	ls;

	ls.GetServerList();

	return TRUE;
}

void BZLauncherApp::SetStatusText(const wxString& text) {
	this->mainFrame->SetStatusText(text);
}
