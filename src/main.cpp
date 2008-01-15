#include "main.h"
#include "gui.h"

#include "listserver.h"

bool BZLauncherApp::OnInit() {
	MainFrame*	frame = new  MainFrame(NULL, 0, _T("BZLauncher v.0.0"), wxPoint(-1,-1), wxSize(-1,-1), wxDEFAULT_FRAME_STYLE);

	frame->Show(TRUE);
	SetTopWindow(frame);

	ListServer	ls;

	ls.GetServerList();

	return TRUE;
}
