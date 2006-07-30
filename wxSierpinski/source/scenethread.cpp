#include <SDL/SDL.h>

#include "sierp/sierp.h"

#include "scenethread.h"

#ifndef min
#define min(a,b)   ((a)<(b)?(a):(b))
#endif

#define UPDATE_PERIOD       1000    // [msec] time to update fps counters
#define CURSOR_SIZE         5       // [px]

SceneThread::SceneThread(wxGLCanvas *glcanvas) : wxThread()
{
    float mx, my;

    screen_last_time = 0;
    scene_last_time = 0;

    screen_period = 1000/30;   // 1000msec / (30 fps) 
    scene_period = 10;

    this->glcanvas = glcanvas;
    this->sierp = sierp_new();

    glcanvas->GetSize(&this->width, &this->height);
    this->radius = 100;

    sierp_vertex_set(sierp, 3, this->radius);
    sierp_points_size_set(sierp, 100000);

    this->margin_x = 5;
    this->margin_y = 5;

    this->x_min = sierp_x_min(sierp);
    this->x_max = sierp_x_max(sierp);
    this->y_min = sierp_y_min(sierp);
    this->y_max = sierp_y_max(sierp);

    mx = (x_max - x_min) * margin_x / 100.0;
    my = (y_max - y_min) * margin_y / 100.0;

    this->x_min -= mx;
    this->x_max += mx;
    this->y_min -= my;
    this->y_max += my;

    this->render_count_new = 0;
    this->render_count = 0;
    this->render_time = 0;

    this->cursor_x = 0;
    this->cursor_y = 0;
}

SceneThread::~SceneThread()
{
    sierp_delete(this->sierp);
}

void *SceneThread::Entry(void)
{
    while(!TestDestroy()) {
        timeNow = SDL_GetTicks();

        if( (timeNow-screen_last_time) > screen_period /*&& screen_dirty*/ ) {
            UpdateScreen();
            screen_last_time = timeNow;
            screen_dirty = false;
        }
        if( (timeNow-scene_last_time) > scene_period ) {
            UpdateScene();
            scene_last_time = timeNow;
        }
        wxMilliSleep(100);
    }

    return NULL;
}

void SceneThread::CursorSet(int x, int y)
{
    cursor_x = x;
    cursor_y = y;
}

void SceneThread::UpdateScreen(void)
{
    glcanvas->Refresh();
}

void SceneThread::UpdateScene(void)
{
    sierp_update(sierp, 100);

    screen_dirty = true;
}

void SceneThread::Render(void)
{
    float time_delta;
    glcanvas->SetCurrent();

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    gluOrtho2D(x_min, x_max, y_min, y_max);
    glMatrixMode(GL_MODELVIEW);

    this->RenderGrid();

    /* draw points */
    glColor3f(0.0, 0.75, 0.0);
    this->RenderSierpPoints();

    /* draw vertices */
    glColor3f(1.0, 1.0, 1.0);
    this->RenderSierpVertices();

    this->RenderCursor();

    glPopMatrix();
    glFlush();

    glcanvas->SwapBuffers();

    /* Update render count */
    render_count_new++;
    time_delta = timeNow - render_time;
    if( time_delta > UPDATE_PERIOD ) {
        render_count = render_count_new;
        render_time = timeNow;
        render_count_new = 0;
        if( time_delta > 0 ) {
            render_per_sec = (float)render_count / time_delta * 1000.0f;
        }
    }
}

void SceneThread::RenderGrid(void)
{
    GLfloat i;
    float grid_x = 10.0;    // number of divisions along x
    float grid_y = 10.0;    // number of divisions along y
    float grid_width;
    float grid_height;
    float center;

    glColor3f(0.25, 0.25, 0.25);
    glBegin( GL_LINES );

    grid_width = (x_max - x_min) / grid_x;
    grid_height = (y_max - y_min) / grid_y;

    /* horizontal lines */
    for(i=this->x_min; i<= this->x_max; i+= grid_width) {
        glVertex2f(i, this->y_min);
        glVertex2f(i, this->y_max);
    }
    /* vertical lines */
    for(i=this->y_min; i<=this->y_max; i+=grid_height) {
        glVertex2f(this->x_min, i);
        glVertex2f(this->x_max, i);
    }
    glEnd();

    glColor3f(0.50, 0.50, 0.50);
    /* center lines */
    glBegin( GL_LINES );
    center = (this->x_max + this->x_min)/2;
    glVertex2f(center, this->y_min);
    glVertex2f(center, this->y_max);

    center = (this->y_max + this->y_min)/2;
    glVertex2f(this->x_min, center);
    glVertex2f(this->x_max, center);
    glEnd();
}

void SceneThread::RenderSierpPoints(void)
{
    int i, n;
    const SIERP_POINT_LIST *points;

    glBegin( GL_POINTS );
    points = sierp_points(sierp);
    n = sierp_point_list_size_get(points);
    for(i=0; i<n; i++) {
        const SIERP_POINT *p;
        p = sierp_point_list_get_index(points, i);
        glVertex2f(p->x, p->y);
    }
    glEnd();
}

void SceneThread::RenderSierpVertices(void)
{
    int i, n;
    const SIERP_POINT *p;
    double x, y;

    glBegin( GL_POINTS );
    n = sierp_vertex_num(sierp);
    for(i=0; i<n; i++) {
        p = sierp_vertex_get(sierp, i);
        x = p->x;
        y = p->y;
        glVertex2f(x, y);
    }
    glEnd();
}

void SceneThread::RenderCursor(void)
{
    GLdouble modelMatrix[16];
    GLdouble projMatrix[16];
    int viewport[4];
    GLdouble x, y, z;
    GLdouble xw, yw, zw;

    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
    glGetIntegerv(GL_VIEWPORT, viewport);

    gluUnProject(
        cursor_x,
        cursor_y,
        0,
        modelMatrix,
        projMatrix,
        viewport,
        &x, &y, &z
    );

    /* Figure out size of cursor box */
    gluUnProject(
        cursor_x-CURSOR_SIZE,
        cursor_y-CURSOR_SIZE,
        0,
        modelMatrix,
        projMatrix,
        viewport,
        &xw, &yw, &zw
    );
    xw = fabs(xw-x);
    yw = fabs(yw-y);
    zw = fabs(zw-z);

    /* Cursor color */
    glColor4f(1.0, 1.0, 1.0, 0.5);
    /* Draw horizontal/veritcal lines */
    glBegin(GL_LINES);
    glVertex3f(x, this->y_min, z);
    glVertex3f(x, y-yw, z);

    glVertex3f(x, y+yw, z);
    glVertex3f(x, this->y_max, z);

    glVertex3f(this->x_min, y, z);
    glVertex3f(x-xw, y, z);

    glVertex3f(x+xw, y, z);
    glVertex3f(this->x_max, y, z);
    glEnd();

    /* draw box around cursor point */
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glVertex2f(x-xw, y-yw);
    glVertex2f(x+xw, y-yw);
    glVertex2f(x+xw, y+yw);
    glVertex2f(x-xw, y+yw);
    glEnd();
}
