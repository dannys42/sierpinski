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
    appstate = new AppState();

    frame = new MainFrame(appstate);
    frame->SetClientSize(640, 480);
    frame->Centre();
    frame->Show();

    SetTopWindow(frame);
    return true;
}

int MainApp::OnRun(void)
{
    // initialize SDL
    if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_TIMER) < 0) {
        std::cerr << "unable to init SDL: " << SDL_GetError() << '\n';
        
        return -1;
    }

    // start the main loop
    return wxApp::OnRun();
}

int MainApp::OnExit(void)
{
    SDL_Quit(); // cleanup SDL

    delete appstate;
    return wxApp::OnExit();
}
