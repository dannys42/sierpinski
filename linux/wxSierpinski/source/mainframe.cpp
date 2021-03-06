#include <stdio.h>

#include "mainframe.h"

#include "sierpicon.xpm"

IMPLEMENT_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_SIZE(MainFrame::OnSize)
    EVT_IDLE(MainFrame::OnIdle)
    EVT_MENU(IDM_FILE_EXIT, MainFrame::onFileExit)
    EVT_MENU(IDM_HELP_ABOUT, MainFrame::onHelpAbout)
END_EVENT_TABLE()

MainFrame::MainFrame(AppState *appstate)
{
    this->appstate = appstate;

    Create(NULL, ID_FRAME, wxT("wxSierpinski"), 
        wxDefaultPosition,
        wxDefaultSize, 
        wxDEFAULT_FRAME_STYLE);

    // Set the icon for this window
    SetIcon(wxIcon(sierpicon_xpm));

    wxMenuBar *mbar = new wxMenuBar;

    // Create the file menu
    wxMenu *fileMenu = new wxMenu;
    fileMenu->Append(IDM_FILE_EXIT, wxT("E&xit"));
    
    mbar->Append(fileMenu, wxT("&File"));

    // Create the help menu
    wxMenu *helpMenu = new wxMenu;
    helpMenu->Append(IDM_HELP_ABOUT, wxT("About"));

    mbar->Append(helpMenu, wxT("&Help"));

    SetMenuBar(mbar); // assign the menubar

    // Create the StatusBar
    statusbar = CreateStatusBar(1, 0, wxID_ANY);;
    statusbar->SetFieldsCount(3);
    statusbar->SetStatusText(_T("This is a test"), 0);
    statusbar->SetStatusText(_T("Field 2"), 1);
    statusbar->SetStatusText(_T("Field 3"), 2);

    // Create the SDL panel
    /*sdlpanel = new SDLPanel(this);*/
    glpanel = new GLPanel(this, appstate);
    
    // Create the control panel
    controlpanel = new ControlPanel(this, appstate);

    // Create the Layout handler, and assign the panes
    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(glpanel, 1, wxEXPAND);
    sizer->Add(controlpanel, 0, wxEXPAND | wxLEFT, 5);
    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);

    render_per_sec = 0.0f;
}

void MainFrame::onFileExit(wxCommandEvent &) 
{ 
    Close(); 
}

void MainFrame::onHelpAbout(wxCommandEvent &)
{
    wxMessageBox(
        wxT(
            "wxSierpinski\n"
            "(c)2006 Danny Sung\n"
            "\n"
            "A simple GUI to experiment with\n"
            "Sierpinski's Triangle\n"
            "\n"
            "Official website:\n"
            "    http://sierpinski.sourceforge.net/\n"
            "\n"
            "This program is released under the GNU GPL.\n"
            "\n"
            "Thanks to:\n"
            " Matthew Wessner - for introducing me to\n"
            "     higher level Sierpinski's shapes\n"
            ),
        wxT("About wxSierpinski"),
        wxOK | wxICON_INFORMATION);
}

void MainFrame::OnSize(wxSizeEvent &event)
{
    int w, h;
    wxSizer *sizer;
    sizer = GetSizer();
    if( sizer != NULL ) {
        GetSize(&w, &h);
        sizer->SetDimension(0, 0, w, h);
        sizer->RecalcSizes();
    }
    event.Skip();   // keep processing event
}

void MainFrame::OnIdle(wxIdleEvent &event)
{
    float newval;
    wxString str;
    double newx, newy;

    newval = appstate->scene.render_per_sec;
    if( newval != render_per_sec ) {
        render_per_sec = newval;
        str.Printf(_("%0.2f fps"), render_per_sec);
        statusbar->SetStatusText(str, 2);
    }

    newx = appstate->scene.cursor.scene_x;
    newy = appstate->scene.cursor.scene_y;
    if( newx != cursor_x || newy != cursor_y ) {
        cursor_x = newx;
        cursor_y = newy;
        str.Printf(_("x=%0.3f y=%0.3f"), cursor_x, cursor_y);
        statusbar->SetStatusText(str, 0);
    }

}
