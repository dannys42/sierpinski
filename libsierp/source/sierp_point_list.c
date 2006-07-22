#include <stdlib.h>
#include <string.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include "sierp/sierp_point_list.h"
#include "sierp_point_list_prv.h"


SIERP_POINT_LIST *sierp_point_list_new(void)
{
    SIERP_POINT_LIST *pl;

    pl = (SIERP_POINT_LIST *)malloc(sizeof(SIERP_POINT_LIST));
    if( pl == NULL )
        return NULL;
    pl->maxlen = 0;
    pl->point = NULL;
    pl->head = 0;
    pl->tail = 0;

    return pl;
}

SIERP_POINT_LIST *sierp_point_list_delete(SIERP_POINT_LIST *point_list)
{
    if( point_list == NULL )
        return NULL;
    if( point_list->point != NULL ) {
        free(point_list->point);
        point_list->point = NULL;
    }
    free(point_list);
    return NULL;
}

int sierp_point_list_is_empty(const SIERP_POINT_LIST *point_list)
{
    if( point_list->head == point_list->tail )
        return 1;
    return 0;
}

int sierp_point_list_is_full(const SIERP_POINT_LIST *point_list)
{
    if( point_list->tail+1 == point_list->head )
        return 1;
    if( point_list->head == 0 && point_list->tail == point_list->maxlen-1 )
        return 1;
    return 0;
}

void sierp_point_list_size_set(SIERP_POINT_LIST *point_list, int newsize)
{
    point_list->head = 0;
    point_list->tail = 0;
    if( point_list->point != NULL ) {
        free(point_list->point);
    }
    point_list->point = (SIERP_POINT *)calloc(newsize+1, sizeof(SIERP_POINT));
    point_list->maxlen = newsize+1;
}

int sierp_point_list_size_get(const SIERP_POINT_LIST *point_list)
{
    int length;

    length = point_list->tail - point_list->head;
    if( length < 0 )
        length += point_list->maxlen;

    return length;
}

int sierp_point_list_push_int(SIERP_POINT_LIST *point_list, double x, double y)
{
    SIERP_POINT p;
    p.x = x;
    p.y = y;
    p.iter = 0;
    return sierp_point_list_push_point(point_list, &p);
}

int sierp_point_list_push_point(SIERP_POINT_LIST *point_list, const SIERP_POINT *point)
{
    if( sierp_point_list_is_full(point_list) ) {
        sierp_point_list_pop(point_list);
    }
    memcpy(&point_list->point[ point_list->tail ], point, sizeof(SIERP_POINT));
    point_list->tail++;
    point_list->tail %= point_list->maxlen;

    return 1;
}

const SIERP_POINT *sierp_point_list_pop(SIERP_POINT_LIST *point_list)
{
    SIERP_POINT *p;

    if( sierp_point_list_is_empty(point_list) )
        return NULL;
    p = &point_list->point[ point_list->head ];
    point_list->head++;
    point_list->head %= point_list->maxlen;

    return p;
}

const SIERP_POINT *sierp_point_list_get_index(const SIERP_POINT_LIST *point_list, int index)
{
    int idx;
    if( index > sierp_point_list_size_get(point_list) )
        return NULL;
    idx = point_list->head + index;
    idx %= point_list->maxlen;

    return &point_list->point[ idx ];
}
