#ifndef SIERP_POINT_LIST_H
#define SIERP_POINT_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sierp/sierp.h"

struct SIERP_POINT_LIST;
typedef struct SIERP_POINT_LIST SIERP_POINT_LIST;

SIERP_POINT_LIST *sierp_point_list_new(void);
SIERP_POINT_LIST *sierp_point_list_delete(SIERP_POINT_LIST *point_list);
void sierp_point_list_clear(SIERP_POINT_LIST *point_list);
int sierp_point_list_is_empty(const SIERP_POINT_LIST *point_list);
int sierp_point_list_is_full(const SIERP_POINT_LIST *point_list);
void sierp_point_list_size_set(SIERP_POINT_LIST *point_list, int newsize);
int sierp_point_list_size_get(const SIERP_POINT_LIST *point_list);
int sierp_point_list_push_point(SIERP_POINT_LIST *point_list, const SIERP_POINT *point);
int sierp_point_list_push_int(SIERP_POINT_LIST *point_list, double x, double y);
const SIERP_POINT *sierp_point_list_pop(SIERP_POINT_LIST *point_list);
const SIERP_POINT *sierp_point_list_get_index(const SIERP_POINT_LIST *point_list, int index);

#ifdef __cplusplus
}
#endif
#endif /* SIERP_POINT_LIST_H */
