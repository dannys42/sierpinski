#include "control_panel.h"

IMPLEMENT_CLASS(ControlPanel, wxPanel)
BEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_PAINT(ControlPanel::onPaint)
    //EVT_ERASE_BACKGROUND(SDLPanel::onEraseBackground)
END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow *parent) :
    wxPanel(parent, ID_CONTROL_PANEL)
{
    this->SetAutoLayout(true);

    // put a box around the control window
    wxStaticBoxSizer *sizer = new wxStaticBoxSizer(wxVERTICAL, this, _T("Controls"));

    // container for contents of StaticBox
    panel = new wxPanel(this);

    // Populate contents
    button1 = new wxButton(panel, wxID_ANY, _T("Button 1"));

    // Finish up
    sizer->Add(panel);
    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}

void ControlPanel::onPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
}

