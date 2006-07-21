#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "win_common.h"

#include "sdlpanel.h"
#include "control_panel.h"

class MainFrame : public wxFrame {
    DECLARE_CLASS(MainFrame);
    DECLARE_EVENT_TABLE();

private:
    SDLPanel *sdlpanel;
    ControlPanel *controlpanel;
    wxStatusBar *statusbar;

    void onFileExit(wxCommandEvent &event);
    void onHelpAbout(wxCommandEvent &event);


public:
    MainFrame();
    void sendIdleEvent(void);
};

#endif /* MAINFRAME_H */
