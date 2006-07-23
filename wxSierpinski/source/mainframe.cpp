
#include "mainframe.h"

IMPLEMENT_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(IDM_FILE_EXIT, MainFrame::onFileExit)
    EVT_MENU(IDM_HELP_ABOUT, MainFrame::onHelpAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(void)
{
    Create(NULL, ID_FRAME, wxT("Frame Title"), wxDefaultPosition,
        wxDefaultSize, 
        wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX);

    wxMenuBar *mb = new wxMenuBar;

    // Create the file menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(IDM_FILE_EXIT, wxT("E&xit"));
    
    mb->Append(fileMenu, wxT("&File"));

    // Create the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(IDM_HELP_ABOUT, wxT("About"));

    mb->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mb); // assign the menubar

    // Create the StatusBar
    statusbar = CreateStatusBar(1, 0, wxID_ANY);;
    statusbar->SetFieldsCount(3);
    statusbar->SetStatusText(_T("This is a test"), 0);
    statusbar->SetStatusText(_T("Field 2"), 1);
    statusbar->SetStatusText(_T("Field 3"), 2);

    // Create the SDL panel
    sdlpanel = new SDLPanel(this);
    
    // Create the control panel
    controlpanel = new ControlPanel(this);

    // Create the Layout handler, and assign the panes
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(sdlpanel, 0, wxGROW);
    sizer->Add(controlpanel, 0, wxGROW);
    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}

void MainFrame::onFileExit(wxCommandEvent &) 
{ 
    Close(); 
}

void MainFrame::onHelpAbout(wxCommandEvent &)
{
    wxMessageBox(
        wxT("wxSierpinski (c)2006 Danny Sung"),
        wxT("About wxSierpinski"),
        wxOK | wxICON_INFORMATION);
}

void MainFrame::sendIdleEvent(void)
{
    wxIdleEvent event;
    event.SetEventObject(sdlpanel);
    sdlpanel->AddPendingEvent(event);
}
