#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "win_common.h"

#include "sdlpanel.h"
#include "glpanel.h"
#include "control_panel.h"

class MainFrame : public wxFrame {
    DECLARE_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()

private:
    SDLPanel *sdlpanel;
    GLPanel *glpanel;
    ControlPanel *controlpanel;
    wxStatusBar *statusbar;

    void onFileExit(wxCommandEvent &event);
    void onHelpAbout(wxCommandEvent &event);
    void OnSize(wxSizeEvent &event);

public:
    MainFrame();
    void sendIdleEvent(void);
};

#endif /* MAINFRAME_H */
