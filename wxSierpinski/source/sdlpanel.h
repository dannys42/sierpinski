#ifndef SDLPANEL_H
#define SDLPANEL_H

#include <SDL/SDL.h>

#include "win_common.h"
#include "sdlthread.h"

class SDLPanel : public wxPanel 
{
    DECLARE_CLASS(SDLPanel);
    DECLARE_EVENT_TABLE();
private:
    SDL_Surface *screen;
    SDLThread *thread;

    void onPaint(wxPaintEvent &event);
    void onEraseBackground(wxEraseEvent &event);
    void onIdle(wxIdleEvent &event);
    void createScreen();

public:
    SDLPanel(wxWindow *parent);
    ~SDLPanel();
};

#endif /* SDLPANEL_H */
