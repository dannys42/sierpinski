#ifndef APPSTATE_H
#define APPSTATE_H
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
        bool scene_recenter;

        AppState(void);
};


#endif /* APPSTATE_H */
