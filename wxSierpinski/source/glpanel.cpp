#include <wx/dcbuffer.h>
#include <wx/image.h>

#include "glpanel.h"

BEGIN_EVENT_TABLE(GLPanel, wxGLCanvas)
    EVT_INIT_DIALOG(GLPanel::OnWindowCreate)
    EVT_IDLE(GLPanel::OnIdle)
    EVT_MOTION(GLPanel::OnMouseMove)
    EVT_ERASE_BACKGROUND(GLPanel::OnEraseBackground)
    EVT_SIZE(GLPanel::OnSize)
    EVT_PAINT(GLPanel::OnPaint)
END_EVENT_TABLE()

GLPanel::GLPanel(wxWindow *parent, wxWindowID id,
        const wxPoint &pos,
        const wxSize &size,
        long style,
        const wxString &name,
        int *glattributelist
        )
        : wxGLCanvas(parent, id, pos, size,
        style|wxFULL_REPAINT_ON_RESIZE, 
        name,
        glattributelist
        )
{
    this->hasInit = false;
    this->scene = NULL;
    this->width = -1;
    this->height = -1;
}

GLPanel::~GLPanel()
{
    if( this->scene != NULL ) {
        scene->Delete();
    }
}

void GLPanel::OnPaint(wxPaintEvent &evt)
{
    // OnPaint handlers MUSt always create a wxPaintDC
    wxPaintDC dc(this);

    if( !this->hasInit ) {
        InitGL();
        this->hasInit = true;
    }
    this->Render();
}

void GLPanel::OnIdle(wxIdleEvent &evt)
{
}

void GLPanel::OnMouseMove(wxMouseEvent &evt)
{
    if( this->scene == NULL )
        return;

    /* window coordinates are flipped from GL coordinates */
    this->scene->CursorSet( evt.GetX(), height - evt.GetY() );
}

void GLPanel::OnSize(wxSizeEvent &evt)
{
    // This is necessary on some platforms
    wxGLCanvas::OnSize(evt);

    GetSize(&width, &height);

    if( hasInit ) {
        ResizeViewport();
    }
}

void GLPanel::InitGL(void)
{
    this->SetCurrent();
    this->Show(TRUE);
    ResizeViewport();
    this->hasInit = true;
}

void GLPanel::ResizeViewport(void)
{
    // set GL viewport (not called by wxGLCanvas::OnSize on all platforms)
    int w, h;
    GetClientSize(&w, &h);;
#ifndef __WXMOTIF__
    if( GetContext() )
#endif
    {
        this->SetCurrent();
        glViewport(0, 0, (GLint)w, (GLint)h);
    }
}

void GLPanel::Render(void)
{
    if( !hasInit )
        return;
    if( this->scene == NULL ) {
        this->scene = new SceneThread(this);
        scene->Create();
        scene->Run();
    } else if( scene->IsAlive() ) {
        this->scene->Render();
    }
}

float GLPanel::RenderPerSec(void)
{
    if( this->scene == NULL ) {
        return(0.0f);
    }

    return(this->scene->render_per_sec);
}

void GLPanel::OnWindowCreate(wxInitDialogEvent &evt)
{
    this->scene = new SceneThread(this);
    scene->Create();
    scene->Run();
    evt.Skip();   // continue processing event
}

void GLPanel::OnEraseBackground(wxEraseEvent &WXUNUSED(evt))
{
    // do nothing, to avoid flashing
}
