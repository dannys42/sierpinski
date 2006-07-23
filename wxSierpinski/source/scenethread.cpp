#include <SDL/SDL.h>

#include "sierp/sierp.h"

#include "scenethread.h"

#ifndef min
#define min(a,b)   ((a)<(b)?(a):(b))
#endif

SceneThread::SceneThread(wxGLCanvas *glcanvas) : wxThread()
{
    screen_last_time = 0;
    scene_last_time = 0;

    screen_period = 1000/30;   // 1000msec / (30 fps) 
    scene_period = 10;

    this->glcanvas = glcanvas;
    this->sierp = sierp_new();

    glcanvas->GetSize(&this->width, &this->height);
    this->radius = min(width, height) / 2;

    sierp_vertex_set(sierp, 3, this->radius);
    sierp_points_size_set(sierp, 100000);
}

SceneThread::~SceneThread()
{
    sierp_delete(this->sierp);
}

void SceneThread::draw_pixel(int x, int y, wxUint32 color)
{
}

void SceneThread::draw_point(const SIERP_POINT *point, wxUint32 color)
{
    double x, y;
    x = point->x;
    y = point->y;

    y = -y;   // flip vertically

    // now center it
    x += this->width/2;
    y += this->height/2;

    draw_pixel((int)x, (int)y, color);
}

void *SceneThread::Entry(void)
{
    while(!TestDestroy()) {
        timeNow = SDL_GetTicks();

        if( (timeNow-screen_last_time) > screen_period /*&& screen_dirty*/ ) {
            update_screen();
            screen_last_time = timeNow;
            screen_dirty = false;
        }
        if( (timeNow-scene_last_time) > scene_period ) {
            update_scene();
            scene_last_time = timeNow;
        }
        wxMilliSleep(100);
    }

    return NULL;
}

void SceneThread::update_screen(void)
{
    glcanvas->Refresh();
}

void SceneThread::update_scene(void)
{
    int i;
    int num;
    const SIERP_POINT_LIST *points;

    sierp_update(sierp, 100);
    points = sierp_points(sierp);
    num = sierp_point_list_size_get(points);
    for(i=0; i<num; i++) {
        const SIERP_POINT *p;
        p = sierp_point_list_get_index(points, i);
        draw_point(p, 0xcccccc);
    }
    
    
    /* Draw Vertices */
    num = sierp_vertex_num(sierp);
    for(i=0; i<num; i++) {
        const SIERP_POINT *p;
        p = sierp_vertex_get(sierp, i);
        draw_point(p, 0x7f7fff);
    }

    screen_dirty = true;
}

void SceneThread::render(void)
{
    int i, n;
    double x, y;
    const SIERP_POINT_LIST *points;

    glcanvas->SetCurrent();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    gluOrtho2D(-radius, radius, -radius, radius);
    glMatrixMode(GL_MODELVIEW);

    glColor3f(0.50, 0.50, 0.50);
    glBegin( GL_TRIANGLE_STRIP );
        glVertex2f(-100.0, -100.0);
        glVertex2f(100.0, -100.0);
        glVertex2f(0.0, 100.0);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    glBegin( GL_TRIANGLE_STRIP );
        glVertex2f(-1.0, -1.0);
        glVertex2f(1.0, -1.0);
        glVertex2f(0.0, 1.0);
    glEnd();

    /* draw points */
    glColor3f(0.0, 0.75, 0.0);
    glBegin( GL_POINTS );
    points = sierp_points(sierp);
    n = sierp_point_list_size_get(points);
    for(i=0; i<n; i++) {
        const SIERP_POINT *p;
        p = sierp_point_list_get_index(points, i);
        glVertex2f(p->x, p->y);
    }
    glEnd();
#if 1
    /* draw vertices */
    glColor3f(1.0, 1.0, 1.0);
    glBegin( GL_POINTS );
    n = sierp_vertex_num(sierp);
    for(i=0; i<n; i++) {
        const SIERP_POINT *p;
        p = sierp_vertex_get(sierp, i);
        x = p->x;
        y = p->y;
        glVertex2f(x, y);
    }
    glEnd();
#endif

    glPopMatrix();
    glFlush();

    glcanvas->SwapBuffers();
}
