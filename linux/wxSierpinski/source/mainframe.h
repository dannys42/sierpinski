#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "win_common.h"

#include "glpanel.h"
#include "control_panel.h"

class MainFrame : public wxFrame {
    DECLARE_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()

private:
    AppState *appstate;
    GLPanel *glpanel;
    ControlPanel *controlpanel;
    wxStatusBar *statusbar;

    // "old" values to see if things need to be redrawn
    float render_per_sec;
    double cursor_x, cursor_y;

    // Function prototypes
    void onFileExit(wxCommandEvent &event);
    void onHelpAbout(wxCommandEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnIdle(wxIdleEvent &event);

public:
    MainFrame(AppState *appstate);
};

#endif /* MAINFRAME_H */
