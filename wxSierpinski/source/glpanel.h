#ifndef GLPANEL_H
#define GLPANEL_H

#ifdef WIN32
#include <windows.h>
#endif
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
        int *glattributelist = NULL
        );
    ~GLPanel();

    void OnWindowCreate(wxInitDialogEvent &evt);
    void OnPaint(wxPaintEvent &evt);
    void OnSize(wxSizeEvent &evt);
    void OnEraseBackground(wxEraseEvent &evt);

    void InitGL(void);
    void Render(void);
    void ResizeViewport(void);

private:
    bool hasInit;
    SceneThread *scene;

    void draw(void);
};

#endif /* GLPANEL_H */

