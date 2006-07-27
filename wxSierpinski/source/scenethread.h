#ifndef SCENETHREAD_H
#define SCENETHREAD_H

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <wx/glcanvas.h>

#include "sierp/sierp.h"
#include "win_common.h"

class SceneThread : public wxThread
{
public:
    SceneThread(wxGLCanvas *glcanvas);
    ~SceneThread();

    void render(void);

private:
    wxGLCanvas *glcanvas;
    int timeNow;
    SIERP *sierp;
    int radius;
    int width;
    int height;

    int screen_period;     // [msec] time to wait before screen updates
    int screen_last_time;     // [msec] last time screen was drawn

    int scene_period;       // [msec] time to wait before updating scene
    int scene_last_time;    // [msec] las ttime scene was updated
    bool screen_dirty;      // only draw screen if scene is dirty

    void *Entry();  // thread entry-point
    void draw_pixel(int x, int y, wxUint32 color);
    void draw_point(const SIERP_POINT *point, wxUint32 color);
    void update_screen();   // handle drawing of screen
    void update_scene();    // handle update/calculation of scene

};

#endif /* SCENETHREAD_H */
