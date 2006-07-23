#ifndef GLPANEL_H
#define GLPANEL_H

#include <wx/glcanvas.h>

#include "win_common.h"
#include "sdlthread.h"
#include "scenethread.h"

class GLPanel: public wxGLCanvas
{
    DECLARE_EVENT_TABLE()
public:
    GLPanel(wxWindow *parent, wxWindowID id=wxID_ANY,
        const wxPoint &pos = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = 0,
        const wxString &name = _T("GLCanvas"),
        int *gl_attrib = NULL
        );
    ~GLPanel();

    void OnWindowCreate(wxWindowCreateEvent &event);
    void OnPaint(wxPaintEvent &event);
    void OnSize(wxSizeEvent &event);

private:
    SceneThread *scene;
    void draw(void);
};

#endif /* GLPANEL_H */

