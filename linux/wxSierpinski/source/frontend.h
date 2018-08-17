#ifndef FRONTEND_H
#define FRONTEND_H

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "mainframe.h"
#include "win_common.h"

class MainApp : public wxApp 
{
    DECLARE_CLASS(MainApp)

private:
    MainFrame *frame;
    AppState *appstate;

public:
    bool OnInit();
    int OnRun();
    int OnExit();
};

#endif /* FRONTEND_H */
