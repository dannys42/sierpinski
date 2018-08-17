#include "control_panel.h"

IMPLEMENT_CLASS(ControlPanel, wxPanel)
BEGIN_EVENT_TABLE(ControlPanel, wxPanel)
    EVT_PAINT(ControlPanel::OnPaint)
    //EVT_ERASE_BACKGROUND(SDLPanel::onEraseBackground)
    EVT_SPINCTRL(ID_CONTROL_VERTICES, ControlPanel::OnVertexUpdate)
    EVT_TEXT_ENTER(ID_CONTROL_DIVISOR, ControlPanel::OnDivisorUpdate)
    EVT_BUTTON(ID_CONTROL_APPLY, ControlPanel::OnApply)
    EVT_CHECKBOX(ID_CONTROL_OUTLINE, ControlPanel::OnOutline)
END_EVENT_TABLE()

ControlPanel::ControlPanel(wxWindow *parent, AppState *appstate) :
    wxPanel(parent, ID_CONTROL_PANEL)
{
    wxStaticBoxSizer *sizer;
    wxSizer *layout;
    wxButton *button;
    wxCheckBox *checkbox;

    this->appstate = appstate;
    this->sierp = appstate->sierp;
    this->vertices = sierp_vertex_num(sierp);
    this->divisor = sierp_divisor_get(sierp);
    this->sierp = sierp;

    this->SetAutoLayout(true);

    // put a box around the control window
    sizer = new wxStaticBoxSizer(wxVERTICAL, this, _T("Controls"));

    // Populate contents
    button = new wxButton(this, ID_CONTROL_APPLY, _T("Apply"));
    sizer->Add(button, 0, wxEXPAND | wxLEFT|wxRIGHT,
        appstate->control_margin);

    // Control for number of vertices
    vertexctrl = new wxSpinCtrl(this, ID_CONTROL_VERTICES, _T("3"));
    layout = AddLabeledControl(_T("Vertices"), vertexctrl);
    sizer->Add(layout, 0, wxEXPAND | wxRIGHT|wxLEFT,
        appstate->control_margin );

    divisorctrl = new wxTextCtrl(this, 
        ID_CONTROL_DIVISOR,
        _T("2"),
        wxDefaultPosition,
        wxDefaultSize,
        wxTE_PROCESS_ENTER
        );
    layout = AddLabeledControl(_T("Divisor"), divisorctrl);
    sizer->Add(layout, 0, wxEXPAND | wxRIGHT|wxLEFT,
        appstate->control_margin );

    // Display settings
    checkbox = new wxCheckBox(this, ID_CONTROL_OUTLINE, _T("Outline"));
    checkbox->SetValue( appstate->scene.draw_outline );
    sizer->Add(checkbox, 0, wxEXPAND | wxRIGHT|wxLEFT,
        appstate->control_margin);

    SetSizer(sizer);
    sizer->Fit(this);
    sizer->SetSizeHints(this);
}

void ControlPanel::OnPaint(wxPaintEvent &event)
{
    wxPaintDC dc(this);
}

void ControlPanel::OnApply(wxCommandEvent &event)
{
    wxCommandEvent null_command;
    wxSpinEvent null_spin;
    
    OnVertexUpdate(null_spin);
    OnDivisorUpdate(null_command);
}

void ControlPanel::OnVertexUpdate(wxSpinEvent &event)
{
    int val;

    val = vertexctrl->GetValue();

    sierp_vertex_set(sierp, val, SIERP_RADIUS_DEFAULT);
    sierp_clear(sierp);
    
    /* Notify scene that it needs to recenter the display */
    appstate->scene.recenter = true;
    /* Update the control with the actual value (just in case) */
    vertexctrl->SetValue(sierp_vertex_num(sierp));
}

void ControlPanel::OnDivisorUpdate(wxCommandEvent &event)
{
    double val;
    wxString str;

    str = divisorctrl->GetValue();
    str.ToDouble(&val);

    if( val == 0 ) {
        val = sierp_divisor_get(sierp);
    } else {
        sierp_divisor_set(sierp, val);
        sierp_clear(sierp);

        /* Notify scene that it needs to recenter the display */
        appstate->scene.recenter = true;
    }

    /* Update the control with the actual value (just in case) */
    val = sierp_divisor_get(sierp);
    str.sprintf(_T("%0.4f"), val);
    divisorctrl->SetValue(str);
}

void ControlPanel::OnOutline(wxCommandEvent &event)
{
    appstate->scene.draw_outline = event.IsChecked();
}

wxBoxSizer *ControlPanel::AddLabeledControl(const wxChar *str, wxWindow *control)
{
    wxStaticText *label;
    wxBoxSizer *layout;

    label = new wxStaticText(this, wxID_ANY, str);
    layout = new wxBoxSizer(wxHORIZONTAL);
    layout->Add(label, 0, wxALIGN_CENTER_VERTICAL );
    layout->Add(control, 0, wxALIGN_CENTER_VERTICAL );

    return(layout);
}

