#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "win_common.h"

class ControlPanel : public wxPanel
{
    DECLARE_CLASS(ControlPanel);
    DECLARE_EVENT_TABLE();
private:
    wxPanel *panel;
    wxButton *button1;

public:
    ControlPanel(wxWindow *parent);
    void onPaint(wxPaintEvent &event);
};


#endif /* CONTROL_PANEL_H */
