#include "appstate.h"

AppState::AppState(void)
{
    /* Scene */
    this->sierp = sierp_new();

    this->scene.recenter = false;
    this->scene.draw_vertex = true;
    this->scene.draw_outline = true;
    this->scene.render_per_sec = 0.0f;
    this->scene.cursor.win_x = 0;
    this->scene.cursor.win_y = 0;
    this->scene.cursor.scene_x = 0;
    this->scene.cursor.scene_y = 0;

    /* Control Panel */
    this->control_margin = 10;  // margin for controls
}

AppState::~AppState(void)
{
    sierp = sierp_delete(sierp);
}
