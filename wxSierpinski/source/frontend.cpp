/* Frontend - This is the main point of entry */
#include <iostream>

#include <SDL/SDL.h>

#include "win_common.h"
#include "frontend.h"
#include "mainframe.h"

IMPLEMENT_CLASS(MainApp, wxApp)
IMPLEMENT_APP(MainApp)

bool MainApp::OnInit(void)
{
    frame = new MainFrame;
    frame->SetClientSize(640, 480);
    frame->Centre();
    frame->Show();

    SetTopWindow(frame);
    return true;
}

int MainApp::OnRun(void)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }
    
#ifndef __WXMSW__ // don't do this if using wxWidgets on MS Windows
    // Setup video mode, but don't create a window
    SDL_SetVideoMode(0, 0, 0, SDL_SWSURFACE);
#endif

    // generate an initial idle event to start things
    frame->sendIdleEvent();

    // start the main loop
    return wxApp::OnRun();
}

int MainApp::OnExit(void)
{
    SDL_Quit(); // cleanup SDL

    return wxApp::OnExit();
}
