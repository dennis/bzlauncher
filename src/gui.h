///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 28 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __gui__
#define __gui__
#include "up22.h"
#include "stop22.h"
#include "refresh32.h"
#include "ping32.h"
#include "favorite32.h"
#include "down22.h"
#include "bzflag32.h"
#include "about32.h"

#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/toolbar.h>
#include <wx/listctrl.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/hyperlink.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/gauge.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrame
///////////////////////////////////////////////////////////////////////////////
class MainFrame : public wxFrame 
{
	private:
	
	protected:
		enum
		{
			ID_REFRESH_LIST = 1000,
			ID_SEARCH,
			ID_QUIT,
			ID_LAUNCH,
			ID_VIEW_DETAILS,
			ID_PING,
			ID_FAVORITE,
			ID_ABOUT,
		};
		
		wxMenuBar* menubar;
		wxMenu* actionMenu;
		wxMenu* serverMenu;
		wxMenu* helpMenu;
		wxStatusBar* statusBar;
		wxToolBar* toolBar;
		wxListCtrl* serverList;
		wxPanel* findPanel;
		wxBitmapButton* closeBtn;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrl2;
		wxStaticLine* m_staticline2;
		wxBitmapButton* m_bpButton2;
		wxBitmapButton* m_bpButton3;
		
		// Virtual event handlers, overide them in your derived class
		virtual void EventRefresh( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventQuit( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventLaunch( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventViewServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventFavoriteToggle( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventPingServer( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventShowAbout( wxCommandEvent& event ){ event.Skip(); }
		virtual void EventColClick( wxListEvent& event ){ event.Skip(); }
		virtual void EventActivated( wxListEvent& event ){ event.Skip(); }
		virtual void EventRightClick( wxListEvent& event ){ event.Skip(); }
		virtual void EventSelectServer( wxListEvent& event ){ event.Skip(); }
		
	
	public:
		MainFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("BZLauncher v1.0"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 597,601 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxWANTS_CHARS );
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDlg
///////////////////////////////////////////////////////////////////////////////
class AboutDlg : public wxDialog 
{
	private:
	
	protected:
		wxHyperlinkCtrl* m_hyperlink1;
		wxStaticBitmap* m_bitmap1;
		wxTextCtrl* m_textCtrl2;
		wxButton* m_button3;
		
		// Virtual event handlers, overide them in your derived class
		virtual void EventClose( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		AboutDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ServerDlg
///////////////////////////////////////////////////////////////////////////////
class ServerDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* serverHostPort;
		wxStaticText* serverName;
		wxStaticText* notice;
		wxStaticLine* m_staticline2;
		wxStaticText* playersLbl;
		wxTextCtrl* playersVal;
		wxStaticText* shotsLbl;
		wxTextCtrl* shotsVal;
		wxStaticText* rogueLbl;
		wxTextCtrl* rogueVal;
		wxStaticText* gameTypeLbl;
		wxTextCtrl* gameTypeVal;
		wxStaticText* redLbl;
		wxTextCtrl* redVal;
		wxStaticText* superFlagsLbl;
		wxTextCtrl* superFlagsVal;
		wxStaticText* greenLbl;
		wxTextCtrl* greenVal;
		wxStaticText* antidoteLbl;
		wxTextCtrl* antidoteVal;
		wxStaticText* blueLbl;
		wxTextCtrl* blueVal;
		wxStaticText* ricochetLbl;
		wxTextCtrl* ricochetVal;
		wxStaticText* purpleLbl;
		wxTextCtrl* purpleVal;
		wxStaticText* handicapLbl;
		wxTextCtrl* handicapVal;
		wxStaticText* observersLbl;
		wxTextCtrl* observersVal;
		wxStaticText* jumpLbl;
		wxTextCtrl* jumpVal;
		wxButton* closeBtn;
		
		// Virtual event handlers, overide them in your derived class
		virtual void EventClose( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		ServerDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Server details"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE );
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UpdatingDlg
///////////////////////////////////////////////////////////////////////////////
class UpdatingDlg : public wxDialog 
{
	private:
	
	protected:
		wxGauge* processIndicator;
	
	public:
		UpdatingDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Updating"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP );
	
};

#endif //__gui__
