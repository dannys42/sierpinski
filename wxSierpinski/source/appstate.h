#ifndef APPSTATE_H
#define APPSTATE_H

#include <sierp/sierp.h>

/*
 * This class contains the "master state" of the application.
 * This amounts to a container for global data.
 *
 * All colors, sizes, margines, etc.
 *
 * In addition, it contains some flags intended to relay information,
 * such as recentering the viewport.
 *
 */
class AppState
{
    public:
        SIERP *sierp;
        struct APPSTATE_SCENE {
            bool recenter;
            bool draw_vertex;
            bool draw_outline;
            float render_per_sec;

            struct APPSTATE_SCENE_CURSOR {
                int win_x;
                int win_y;
                double scene_x;
                double scene_y;
            } cursor;
        } scene;

        int control_margin;

        AppState(void);
        ~AppState(void);
};


#endif /* APPSTATE_H */
