
#include "sierp/sierp.h"

#include "sdlthread.h"

SDLThread::SDLThread(wxPanel *panel, SDL_Surface *screen) : wxThread()
{
    screen_last_time = 0;
    scene_last_time = 0;

    screen_period = 1000/30;   // 1000msec / (30 fps) 
    scene_period = 10;

    this->panel = panel;
    this->screen = screen;

    this->sierp = sierp_new();
    sierp_vertex_set(sierp, 5, 200);
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

    if( !screen )
        return;

    // Lock surface if needed
    if (SDL_MUSTLOCK(screen)) {
        if (SDL_LockSurface(screen) < 0) {
            return;
        }
    }

    // Ask SDL for the time in milliseconds
    int tick = timeNow;
    /*
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            wxUint32 color = (y*y) + (x*x) + tick + x*tick/(y+1);
            draw_pixel(x,  y, color);
        }
    }*/
    
    num = sierp_vertex_num(sierp);
    for(i=0; i<num; i++) {
        int x, y;
        const SIERP_POINT *p;
        p = sierp_vertex_get(sierp, i);
        x = (int)(p->x + 200);
        y = (int)(p->y + 200);
        draw_pixel(x, y, 0xffffff ^ tick);
    }
    // Unlock if needed
    if (SDL_MUSTLOCK(screen)) {
        SDL_UnlockSurface(screen);
    }

    screen_dirty = true;
}
