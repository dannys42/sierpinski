#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "win_common.h"

#include "glpanel.h"
#include "control_panel.h"

class MainFrame : public wxFrame {
    DECLARE_CLASS(MainFrame)
    DECLARE_EVENT_TABLE()

private:
    GLPanel *glpanel;
    ControlPanel *controlpanel;
    wxStatusBar *statusbar;
    float render_per_sec;

    void onFileExit(wxCommandEvent &event);
    void onHelpAbout(wxCommandEvent &event);
    void OnSize(wxSizeEvent &event);
    void OnIdle(wxIdleEvent &event);

public:
    MainFrame();
};

#endif /* MAINFRAME_H */
