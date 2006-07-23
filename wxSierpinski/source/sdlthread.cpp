
#include "sierp/sierp.h"

#include "sdlthread.h"

#ifndef min
#define min(a,b)   ((a)<(b)?(a):(b))
#endif

SDLThread::SDLThread(wxPanel *panel, SDL_Surface *screen) : wxThread()
{
    screen_last_time = 0;
    scene_last_time = 0;

    screen_period = 1000/30;   // 1000msec / (30 fps) 
    scene_period = 10;

    this->panel = panel;
    this->screen = screen;

    this->sierp = sierp_new();

    panel->GetSize(&this->width, &this->height);
printf("panel dimensions: %d x %d\n", this->width, this->height);
    this->radius = min(width, height) / 2;
    sierp_vertex_set(sierp, 3, this->radius);
}

SDLThread::~SDLThread()
{
    sierp_delete(this->sierp);
}

void SDLThread::draw_pixel(int x, int y, wxUint32 color)
{
    if( x < 0 || x >= screen->w )
        return;
    if( y < 0 || y >= screen->h )
        return;
    wxUint8 *pixels = static_cast<wxUint8 *>(screen->pixels) + 
                      (y * screen->pitch) +
                      (x * screen->format->BytesPerPixel);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    pixels[0] = color & 0xFF;
    pixels[1] = (color >> 8) & 0xFF;
    pixels[2] = (color >> 16) & 0xFF;
#else
    pixels[0] = (color >> 16) & 0xFF;
    pixels[1] = (color >> 8) & 0xFF;
    pixels[2] = color & 0xFF;
#endif
}

void SDLThread::draw_point(const SIERP_POINT *point, wxUint32 color)
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

void *SDLThread::Entry(void)
{
    while(!TestDestroy()) {
        timeNow = SDL_GetTicks();

        if( (timeNow-screen_last_time) > screen_period && screen_dirty ) {
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

void SDLThread::update_screen(void)
{
    panel->Refresh();
}

void SDLThread::update_scene(void)
{
    int i;
    int num;
    const SIERP_POINT_LIST *points;

    if( !screen )
        return;

    // Lock surface if needed
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }

    // Ask SDL for the time in milliseconds
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
    // Unlock if needed
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    screen_dirty = true;
}
