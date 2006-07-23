#include <wx/dcbuffer.h>
#include <wx/image.h>

#include "glpanel.h"

BEGIN_EVENT_TABLE(GLPanel, wxGLCanvas)
    EVT_WINDOW_CREATE(GLPanel::OnWindowCreate)
    EVT_SIZE(GLPanel::OnSize)
    EVT_PAINT(GLPanel::OnPaint)
END_EVENT_TABLE()

GLPanel::GLPanel(wxWindow *parent, wxWindowID id,
        const wxPoint &pos,
        const wxSize &size,
        long style,
        const wxString &name,
        int *gl_attrib)
        : wxGLCanvas(parent, id, pos, size,
        style|wxFULL_REPAINT_ON_RESIZE, name, gl_attrib)
{
    this->scene = NULL;
}

GLPanel::~GLPanel()
{
    if( this->scene != NULL ) {
        scene->Delete();
    }
}

void GLPanel::OnPaint(wxPaintEvent &event)
{
    // OnPaint handlers MUSt always create a wxPaintDC
    wxPaintDC dc(this);

    if( scene != NULL ) {
        if( scene->IsAlive() ) {
            scene->render();
        }
    }
}

void GLPanel::OnSize(wxSizeEvent &event)
{
    // This is necessary on some platforms
    wxGLCanvas::OnSize(event);

    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms)
    int w, h;
    GetClientSize(&w, &h);;

#ifndef __WXMOTIF__
    if( GetContext() )
#endif
    {
        SetCurrent();
        glViewport(0, 0, (GLint)w, (GLint)h);
    }
}

void GLPanel::OnWindowCreate(wxWindowCreateEvent &event)
{
    this->scene = new SceneThread(this);
    scene->Create();
    scene->Run();
    event.Skip();   // continue processing event
}
