#ifndef CONTROL_PANEL_H
#define CONTROL_PANEL_H

#include "win_common.h"

#include <wx/spinctrl.h>

#include "sierp/sierp.h"

class ControlPanel : public wxPanel
{
    DECLARE_CLASS(ControlPanel)
    DECLARE_EVENT_TABLE()
private:
    AppState *appstate;
    wxPanel *panel;
    wxButton *button1;
    wxSpinCtrl *vertexctrl;
    wxTextCtrl *divisorctrl;

    int vertices;
    double divisor;
    SIERP *sierp;

    wxBoxSizer *AddLabeledControl(const wxChar *str, wxWindow *control);

public:
    bool sceneRecenter;

    ControlPanel(wxWindow *parent, AppState *appstate, SIERP *sierp);
    void OnPaint(wxPaintEvent &event);
    void OnVertexUpdate(wxSpinEvent &event);
    void OnDivisorUpdate(wxCommandEvent &event);


};

#endif /* CONTROL_PANEL_H */
