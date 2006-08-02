#ifndef WIN_COMMON_H
#define WIN_COMMON_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <sierp/sierp.h>

#include "appstate.h"

enum {
    ID_FRAME = 10000,
    ID_SDL_PANEL,
    ID_CONTROL_PANEL,
    ID_CONTROL_VERTICES,
    ID_CONTROL_DIVISOR,
    ID_CONTROL_APPLY,
    IDM_FILE_EXIT,
    IDM_HELP_ABOUT
};

#endif /* WINIDS_H */
