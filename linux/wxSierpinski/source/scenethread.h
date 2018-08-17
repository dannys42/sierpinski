#ifndef SCENETHREAD_H
#define SCENETHREAD_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/glcanvas.h>

#include "win_common.h"

class SceneThread : public wxThread
{
public:
    SceneThread(wxGLCanvas *glcanvas, AppState *appstate);
    ~SceneThread();

    void Render(void);
    void Recenter(void);

    float scene_per_sec;

private:
    AppState *appstate;
    SIERP *sierp;
    wxGLCanvas *glcanvas;
    int timeNow;            // [msec] clock value this thread period
    int radius;
    int width;
    int height;
    GLfloat cursor_x, cursor_y;

    GLfloat x_min;
    GLfloat x_max;
    GLfloat y_min;
    GLfloat y_max;
    double margin_x;        // percent margin to leave on x-borders
    double margin_y;        // percent margin to leave on y-borders

    int screen_period;     // [msec] time to wait before screen updates
    int screen_last_time;     // [msec] last time screen was drawn

    int scene_period;       // [msec] time to wait before updating scene
    int scene_last_time;    // [msec] las ttime scene was updated
    bool screen_dirty;      // only draw screen if scene is dirty

    void *Entry();  // thread entry-point
    void UpdateScreen();   // handle drawing of screen
    void UpdateScene();    // handle update/calculation of scene

    void RenderSierpVertices(void); // draw vertices as points
    void RenderSierpOutline(void);  // draw vertices as polygon
    void RenderSierpPoints(void);   // draw contents
    void RenderGrid(void);
    void RenderCursor(void);
    void Vertices(void);            // loop through all vertices
    
    long render_count_new;
    long render_count;
    long render_time;

};

#endif /* SCENETHREAD_H */
