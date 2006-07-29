#ifndef SIERP_H
#define SIERP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sierp/sierp_point.h"
#include "sierp/sierp_point_list.h"

#define SIERP_FLAG_ALIGN_BOTTOM   (1<<0)

struct SIERP;
typedef struct SIERP SIERP;

SIERP *sierp_new(void);
SIERP *sierp_delete(SIERP *sierp);

double sierp_divisor_get(SIERP *sierp);
double sierp_divisor_set(SIERP *sierp, double divisor);
int sierp_flag_clear(SIERP *sierp, int flags);
int sierp_flag_isset(SIERP *sierp, int flags);
int sierp_flag_set(SIERP *sierp, int flags);
const struct SIERP_POINT_LIST *sierp_points(SIERP *sierp);
int sierp_points_size_set(SIERP *sierp, int size);
int sierp_radius_get(SIERP *sierp);
int sierp_update(SIERP *sierp, int steps);
SIERP *sierp_vertex_set(SIERP *sierp, int num_vertices, int radius);
const SIERP_POINT *sierp_vertex_get(SIERP *sierp, int index);
int sierp_vertex_num(SIERP *sierp);
double sierp_x_min(SIERP *sierp);
double sierp_x_max(SIERP *sierp);
double sierp_y_min(SIERP *sierp);
double sierp_y_max(SIERP *sierp);

#ifdef __cplusplus
}
#endif
#endif /* SIERP_H */
