#include <wx/dcbuffer.h>
#include <wx/image.h>

#include "sdlpanel.h"

IMPLEMENT_CLASS(SDLPanel, wxPanel);
BEGIN_EVENT_TABLE(SDLPanel, wxPanel)
    EVT_PAINT(SDLPanel::onPaint)
    EVT_ERASE_BACKGROUND(SDLPanel::onEraseBackground)
    EVT_IDLE(SDLPanel::onIdle)
END_EVENT_TABLE()

void SDLPanel::onEraseBackground(wxEraseEvent &)
{
}

SDLPanel::SDLPanel(wxWindow *parent) : 
    wxPanel(parent, ID_SDL_PANEL), 
    screen(NULL)
{
    wxSize size(640, 480);
    SetMinSize(size);
    SetMaxSize(size);
}

SDLPanel::~SDLPanel()
{
    if( thread ) {
        thread->Delete();
    }
    if( screen != NULL ) {
        SDL_FreeSurface(screen);
        screen = NULL;
    }
}

void SDLPanel::onPaint(wxPaintEvent &)
{
    if( screen == NULL ) {
        wxPaintDC dc(this);
        return;
    }

    if( SDL_MUSTLOCK(screen) ) {
        if( SDL_LockSurface(screen) < 0 ) {
            return;
        }
    }

    wxBitmap bmp(wxImage(screen->w, screen->h,
        static_cast<unsigned char *>(screen->pixels),
        true));

    if( SDL_MUSTLOCK(screen) ) {
        SDL_UnlockSurface(screen);
    }
    wxBufferedPaintDC dc(this, bmp);
}

void SDLPanel::onIdle(wxIdleEvent &)
{
    createScreen();
}

void SDLPanel::createScreen(void)
{
    int width, height;
    if( screen != NULL )
        return;
    GetSize(&width, &height);
    screen = SDL_CreateRGBSurface(
        SDL_SWSURFACE, width, height,
        24, 0, 0, 0, 0);

    thread = new SDLThread(this, screen);
    thread->Create();
    thread->Run();
}