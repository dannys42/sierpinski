#ifndef SIERP_H
#define SIERP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sierp/sierp_point.h"

struct SIERP;
typedef struct SIERP SIERP;

SIERP *sierp_vertex_set(SIERP *sierp, int num_vertices, int radius);
SIERP *sierp_delete(SIERP *sierp);
SIERP *sierp_new(int num_vertices);
const SIERP_POINT *sierp_vertex_get(SIERP *sierp, int index);
int sierp_vertex_num(SIERP *sierp);
int sierp_radius_get(SIERP *sierp);

#ifdef __cplusplus
}
#endif
#endif /* SIERP_H */
