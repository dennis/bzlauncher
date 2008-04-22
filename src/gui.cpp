///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep 28 2007)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include <wx/mstream.h>
#include "../config.h"

inline wxBitmap _wxGetBitmapFromMemory(const unsigned char *data, int length) { wxMemoryInputStream is(data, length); return wxBitmap(wxImage(is, wxBITMAP_TYPE_ANY, -1), -1); }
 #define wxGetBitmapFromMemory(name) _wxGetBitmapFromMemory(name ## _png, sizeof(name ## _png))

///////////////////////////////////////////////////////////////////////////

MainFrame::MainFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	menubar = new wxMenuBar( 0 );
	actionMenu = new wxMenu();
	wxMenuItem* refreshList = new wxMenuItem( actionMenu, ID_REFRESH_LIST, wxString( _("Refresh list") ) + wxT('\t') + wxT("CTRL-R"), wxEmptyString, wxITEM_NORMAL );
	actionMenu->Append( refreshList );
	wxMenuItem* search = new wxMenuItem( actionMenu, ID_SEARCH, wxString( _("Search") ) + wxT('\t') + wxT("CTRL-F"), wxEmptyString, wxITEM_NORMAL );
	actionMenu->Append( search );
	wxMenuItem* toolbar = new wxMenuItem( actionMenu, ID_TOOLBAR, wxString( _("Toolbar") ) + wxT('\t') + wxT("CTRL-T"), wxEmptyString, wxITEM_CHECK );
	actionMenu->Append( toolbar );
	toolbar->Check( true );
	
	
	actionMenu->AppendSeparator();
	wxMenuItem* quit = new wxMenuItem( actionMenu, ID_QUIT, wxString( _("&Quit") ) + wxT('\t') + wxT("CTRL-Q"), wxEmptyString, wxITEM_NORMAL );
	actionMenu->Append( quit );
	menubar->Append( actionMenu, _("&Action") );
	
	serverMenu = new wxMenu();
	wxMenuItem* joinServer = new wxMenuItem( serverMenu, ID_LAUNCH, wxString( _("Join server") ) , wxEmptyString, wxITEM_NORMAL );
	serverMenu->Append( joinServer );
	wxMenuItem* pingServer = new wxMenuItem( serverMenu, ID_PING, wxString( _("Ping server") ) , wxEmptyString, wxITEM_NORMAL );
	serverMenu->Append( pingServer );
	wxMenuItem* favoriteToggle = new wxMenuItem( serverMenu, ID_FAVORITE, wxString( _("Favorite?") ) , wxEmptyString, wxITEM_NORMAL );
	serverMenu->Append( favoriteToggle );
	menubar->Append( serverMenu, _("&Server") );
	
	helpMenu = new wxMenu();
	wxMenuItem* about = new wxMenuItem( helpMenu, ID_ABOUT, wxString( _("About") ) + wxT('\t') + wxT("F1"), wxEmptyString, wxITEM_NORMAL );
	helpMenu->Append( about );
	menubar->Append( helpMenu, _("&Help") );
	
	this->SetMenuBar( menubar );
	
	statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	wxFlexGridSizer* bSizer2;
	bSizer2 = new wxFlexGridSizer( 3, 1, 0, 0 );
	bSizer2->AddGrowableRow( 1 );
	bSizer2->SetFlexibleDirection( wxVERTICAL );
	bSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_ALL );
	
	toolBar = new wxToolBar( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL|wxTB_TEXT ); 
	toolBar->AddTool( ID_REFRESH_LIST, _("Refresh"), wxGetBitmapFromMemory(refresh32), wxNullBitmap, wxITEM_NORMAL, _("Refresh list"), _("Refresh list") );
	toolBar->AddTool( ID_SEARCH, _("Search"), wxGetBitmapFromMemory(search32), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	toolBar->AddSeparator();
	toolBar->AddTool( ID_LAUNCH, _("Play"), wxGetBitmapFromMemory(bzflag32), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	toolBar->AddTool( ID_PING, _("Ping"), wxGetBitmapFromMemory(ping32), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	toolBar->AddTool( ID_FAVORITE, _("Favorite"), wxGetBitmapFromMemory(favorite32), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	toolBar->AddSeparator();
	toolBar->AddTool( ID_ABOUT, _("About"), wxGetBitmapFromMemory(about32), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString );
	toolBar->Realize();
	
	bSizer2->Add( toolBar, 0, wxEXPAND, 5 );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	tabs = new wxAuiNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_NB_CLOSE_BUTTON|wxAUI_NB_SCROLL_BUTTONS|wxAUI_NB_WINDOWLIST_BUTTON );
	
	mainSizer->Add( tabs, 1, wxALL|wxEXPAND, 0 );
	
	bSizer2->Add( mainSizer, 1, wxEXPAND, 5 );
	
	findPanel = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	findPanel->Hide();
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 1, 6, 0, 0 );
	fgSizer3->AddGrowableCol( 1 );
	fgSizer3->SetFlexibleDirection( wxBOTH );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticText1 = new wxStaticText( findPanel, wxID_ANY, _("Search"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer3->Add( m_staticText1, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	queryText = new wxTextCtrl( findPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	queryText->SetMaxLength( 20 ); 
	fgSizer3->Add( queryText, 0, wxALIGN_CENTER_VERTICAL|wxEXPAND, 0 );
	
	closeBtn = new wxBitmapButton( findPanel, wxID_ANY, wxGetBitmapFromMemory(stop22), wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW );
	fgSizer3->Add( closeBtn, 0, wxALL, 5 );
	
	findPanel->SetSizer( fgSizer3 );
	findPanel->Layout();
	fgSizer3->Fit( findPanel );
	bSizer2->Add( findPanel, 1, wxEXPAND | wxALL, 0 );
	
	this->SetSizer( bSizer2 );
	this->Layout();
	
	// Connect Events
	this->Connect( refreshList->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::EventRefresh ) );
	this->Connect( toolbar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::EventToolbarToggle ) );
	this->Connect( quit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::EventQuit ) );
	this->Connect( joinServer->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MainFrame::EventLaunch ) );
	this->Connect( ID_SEARCH, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::EventSearch ) );
	this->Connect( ID_PING, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::EventPingServer ) );
	this->Connect( ID_FAVORITE, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::EventFavoriteToggle ) );
	this->Connect( ID_ABOUT, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( MainFrame::EventShowAbout ) );
	tabs->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler( MainFrame::EventViewChanged ), NULL, this );
	tabs->Connect( wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler( MainFrame::EventViewClose ), NULL, this );
	queryText->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( MainFrame::EventSearchText ), NULL, this );
}

AboutDlg::AboutDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 2, 1, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	m_hyperlink1 = new wxHyperlinkCtrl( this, wxID_ANY, _("BZLauncher"), wxT("http://dennis.moellegaard.dk/projects/bzlauncher/"), wxDefaultPosition, wxDefaultSize, wxHL_ALIGN_CENTRE|wxHL_DEFAULT_STYLE );
	m_hyperlink1->SetFont( wxFont( 24, 74, 90, 90, false, wxT("Sans") ) );
	
	bSizer3->Add( m_hyperlink1, 0, wxALL, 5 );
	
	m_bitmap1 = new wxStaticBitmap( this, wxID_CANCEL, wxGetBitmapFromMemory(bzflag32), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_bitmap1, 0, wxALL, 5 );
	
	fgSizer2->Add( bSizer3, 1, wxEXPAND, 5 );
	
	m_textCtrl2 = new wxTextCtrl( this, wxID_ANY, _("\nVersion " VERSION "\n\nAuthor:\nDennis Moellegaard Pedersen\ndennis@moellegaard.dk\n\nThanks to:\nSuavegirl, Strayer and\nthe BZFlag developers"), wxDefaultPosition, wxSize( -1,200 ), wxTE_AUTO_URL|wxTE_CENTRE|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	fgSizer2->Add( m_textCtrl2, 0, wxALL|wxEXPAND, 5 );
	
	m_button3 = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_button3, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	// Connect Events
	m_button3->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AboutDlg::EventClose ), NULL, this );
}

ServerDlg::ServerDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->Centre( wxBOTH );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	serverHostPort = new wxStaticText( this, wxID_ANY, _("ServerPortHost"), wxDefaultPosition, wxDefaultSize, 0 );
	serverHostPort->Wrap( -1 );
	serverHostPort->SetFont( wxFont( 14, 74, 90, 90, false, wxT("Sans") ) );
	
	bSizer3->Add( serverHostPort, 0, wxALL, 5 );
	
	serverName = new wxStaticText( this, wxID_ANY, _("ServerName"), wxDefaultPosition, wxDefaultSize, 0 );
	serverName->Wrap( -1 );
	bSizer3->Add( serverName, 0, wxALL, 5 );
	
	notice = new wxStaticText( this, wxID_ANY, _("This server uses a unsupported protocol (BZFS0099)\nData below is incorrect"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
	notice->Wrap( -1 );
	notice->SetForegroundColour( wxColour( 255, 0, 0 ) );
	notice->Hide();
	
	bSizer3->Add( notice, 0, wxALL|wxEXPAND, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	wxGridSizer* serverDetailSizer;
	serverDetailSizer = new wxGridSizer( 2, 4, 0, 0 );
	
	playersLbl = new wxStaticText( this, wxID_ANY, _("Players:"), wxDefaultPosition, wxDefaultSize, 0 );
	playersLbl->Wrap( -1 );
	serverDetailSizer->Add( playersLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	playersVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( playersVal, 0, wxALL, 5 );
	
	shotsLbl = new wxStaticText( this, wxID_ANY, _("Shots:"), wxDefaultPosition, wxDefaultSize, 0 );
	shotsLbl->Wrap( -1 );
	serverDetailSizer->Add( shotsLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	shotsVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( shotsVal, 0, wxALL, 5 );
	
	rogueLbl = new wxStaticText( this, wxID_ANY, _("Rogues:"), wxDefaultPosition, wxDefaultSize, 0 );
	rogueLbl->Wrap( -1 );
	serverDetailSizer->Add( rogueLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	rogueVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( rogueVal, 0, wxALL, 5 );
	
	gameTypeLbl = new wxStaticText( this, wxID_ANY, _("Game type:"), wxDefaultPosition, wxDefaultSize, 0 );
	gameTypeLbl->Wrap( -1 );
	serverDetailSizer->Add( gameTypeLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	gameTypeVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( gameTypeVal, 0, wxALL, 5 );
	
	redLbl = new wxStaticText( this, wxID_ANY, _("Red:"), wxDefaultPosition, wxDefaultSize, 0 );
	redLbl->Wrap( -1 );
	serverDetailSizer->Add( redLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	redVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( redVal, 0, wxALL, 5 );
	
	superFlagsLbl = new wxStaticText( this, wxID_ANY, _("Super flags:"), wxDefaultPosition, wxDefaultSize, 0 );
	superFlagsLbl->Wrap( -1 );
	serverDetailSizer->Add( superFlagsLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	superFlagsVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( superFlagsVal, 0, wxALL, 5 );
	
	greenLbl = new wxStaticText( this, wxID_ANY, _("Green:"), wxDefaultPosition, wxDefaultSize, 0 );
	greenLbl->Wrap( -1 );
	serverDetailSizer->Add( greenLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	greenVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( greenVal, 0, wxALL, 5 );
	
	antidoteLbl = new wxStaticText( this, wxID_ANY, _("Antidote flags:"), wxDefaultPosition, wxDefaultSize, 0 );
	antidoteLbl->Wrap( -1 );
	serverDetailSizer->Add( antidoteLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	antidoteVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( antidoteVal, 0, wxALL, 5 );
	
	blueLbl = new wxStaticText( this, wxID_ANY, _("Blue:"), wxDefaultPosition, wxDefaultSize, 0 );
	blueLbl->Wrap( -1 );
	serverDetailSizer->Add( blueLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	blueVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( blueVal, 0, wxALL, 5 );
	
	ricochetLbl = new wxStaticText( this, wxID_ANY, _("Ricochet:"), wxDefaultPosition, wxDefaultSize, 0 );
	ricochetLbl->Wrap( -1 );
	serverDetailSizer->Add( ricochetLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	ricochetVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( ricochetVal, 0, wxALL, 5 );
	
	purpleLbl = new wxStaticText( this, wxID_ANY, _("Purple:"), wxDefaultPosition, wxDefaultSize, 0 );
	purpleLbl->Wrap( -1 );
	serverDetailSizer->Add( purpleLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	purpleVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( purpleVal, 0, wxALL, 5 );
	
	handicapLbl = new wxStaticText( this, wxID_ANY, _("Handicap:"), wxDefaultPosition, wxDefaultSize, 0 );
	handicapLbl->Wrap( -1 );
	serverDetailSizer->Add( handicapLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	handicapVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( handicapVal, 0, wxALL, 5 );
	
	observersLbl = new wxStaticText( this, wxID_ANY, _("Observers:"), wxDefaultPosition, wxDefaultSize, 0 );
	observersLbl->Wrap( -1 );
	serverDetailSizer->Add( observersLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	observersVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( observersVal, 0, wxALL, 5 );
	
	jumpLbl = new wxStaticText( this, wxID_ANY, _("Jump:"), wxDefaultPosition, wxDefaultSize, 0 );
	jumpLbl->Wrap( -1 );
	serverDetailSizer->Add( jumpLbl, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	jumpVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), wxTE_READONLY|wxTE_RIGHT );
	serverDetailSizer->Add( jumpVal, 0, wxALL, 5 );
	
	bSizer3->Add( serverDetailSizer, 1, wxALL|wxEXPAND|wxSHAPED, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer3->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );
	
	launchBtn = new wxButton( this, wxID_ANY, _("Launch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( launchBtn, 0, 0, 5 );
	
	teamCbx = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY );
	teamCbx->Append( _("asdf") );
	bSizer5->Add( teamCbx, 0, wxEXPAND, 5 );
	
	bSizer3->Add( bSizer5, 0, wxALIGN_CENTER, 5 );
	
	closeBtn = new wxButton( this, wxID_ANY, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( closeBtn, 0, wxALL|wxEXPAND, 5 );
	
	this->SetSizer( bSizer3 );
	this->Layout();
	bSizer3->Fit( this );
	
	// Connect Events
	launchBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerDlg::onLaunch ), NULL, this );
	closeBtn->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ServerDlg::EventClose ), NULL, this );
}

UpdatingDlg::UpdatingDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	processIndicator = new wxGauge( this, wxID_ANY, 100, wxDefaultPosition, wxDefaultSize, wxGA_HORIZONTAL );
	bSizer4->Add( processIndicator, 0, wxALL|wxEXPAND, 10 );
	
	this->SetSizer( bSizer4 );
	this->Layout();
	bSizer4->Fit( this );
}
