#ifndef SDLTHREAD_H
#define SDLTHREAD_H

#include <SDL/SDL.h>

#include "sierp/sierp.h"
#include "win_common.h"

class SDLThread : public wxThread
{
public:
    SDLThread(wxPanel *panel, SDL_Surface *screen);
    SDLThread::~SDLThread();

private:
    SDL_Surface *screen;
    wxPanel *panel;
    int timeNow;
    SIERP *sierp;

    int screen_period;     // [msec] time to wait before screen updates
    int screen_last_time;     // [msec] last time screen was drawn

    int scene_period;       // [msec] time to wait before updating scene
    int scene_last_time;    // [msec] las ttime scene was updated
    bool screen_dirty;      // only draw screen if scene is dirty

    void *Entry();  // thread entry-point
    void draw_pixel(int x, int y, wxUint32 color);
    void update_screen();   // handle drawing of screen
    void update_scene();    // handle update/calculation of scene
};

#endif /* SDLTHREAD_H */
