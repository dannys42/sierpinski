#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include "sierp/sierp.h"
#include "sierp/sierp_point.h"
#include "sierp/sierp_point_list.h"

#define SIERP_DEFAULT_VERTICES      3
#define SIERP_DEFAULT_DIVISOR       2
#define SIERP_DEFAULT_RADIUS        100
#define SIERP_DEFAULT_POINT_SIZE    1000

struct SIERP {
    SIERP_POINT_LIST *vertex;
    SIERP_POINT_LIST *points;
    SIERP_POINT point_last;
    double divisor;
    int flags;
    int radius;
    int iter_per_update;
};

static double sierp_radian_align_bottom(SIERP *sierp, int num_vertices);

SIERP *sierp_new(void)
{
    SIERP *sierp;
    sierp = (SIERP *)malloc(sizeof(SIERP));
    if( sierp == NULL )
        return NULL;

    /* Initialize values */
    sierp->vertex = NULL;
    sierp->points = NULL;
    sierp->radius = 100;
    sierp->iter_per_update = 100;
    sierp->flags = 0;
    sierp->divisor = SIERP_DEFAULT_DIVISOR;
    sierp_flag_set(sierp, SIERP_FLAG_ALIGN_BOTTOM);

    /* Default vertex type */
    sierp_vertex_set(sierp, SIERP_DEFAULT_VERTICES, SIERP_DEFAULT_RADIUS);

    /* Default point size */
    sierp->points = sierp_point_list_new();
    sierp_point_list_size_set(sierp->points, SIERP_DEFAULT_POINT_SIZE);

    return sierp;
}

SIERP *sierp_delete(SIERP *sierp)
{
    if( sierp == NULL )
        return NULL;
    if( sierp->vertex != NULL ) {
        sierp_point_list_delete(sierp->vertex);
        sierp->vertex = NULL;
    }
    if( sierp->points != NULL ) {
        sierp_point_list_delete(sierp->points);
        sierp->points = NULL;
    }
    free(sierp);
    sierp = NULL;
    return NULL;
}

SIERP *sierp_vertex_set(SIERP *sierp, int num_vertices, int radius)
{
    int i;
    SIERP_POINT p;
    double rotate = 0;

    if( sierp->vertex != NULL ) {
        sierp_point_list_delete(sierp->vertex);
    }
    sierp->vertex = sierp_point_list_new();
    sierp_point_list_size_set(sierp->vertex, num_vertices);

    if( sierp_flag_isset(sierp, SIERP_FLAG_ALIGN_BOTTOM) ) {
        rotate = sierp_radian_align_bottom(sierp, num_vertices);
    }

    for(i=0; i<num_vertices; i++) {
        double radian;
        radian = 2*M_PI * (double)i / num_vertices;
        radian += rotate;
        p.x = radius * cos(radian);
        p.y = radius * sin(radian);
        p.iter = 0;
        sierp_point_list_push_point(sierp->vertex, &p);
    }
    sierp->radius = radius;

    /* Just pick an arbitrary vertex to start with */
    sierp->point_last = p;
    return NULL;
}

int sierp_update(SIERP *sierp, int steps)
{
    int vertex_idx;
    const SIERP_POINT *vertex;
    SIERP_POINT point;

    for( ; steps > 0; steps-- ) {
        /* pick a random vertex to go to */
        float rval;
        rval = random();
        rval = rval / RAND_MAX * sierp_vertex_num(sierp);

        vertex_idx = (int)rval;
        vertex = sierp_vertex_get(sierp, vertex_idx);

        point.x = sierp->point_last.x + vertex->x;
        point.x /= sierp->divisor;

        point.y = sierp->point_last.y + vertex->y;
        point.y /= sierp->divisor;

        sierp_point_list_push_point(sierp->points, &point);
        sierp->point_last = point;
    }
    return(0);
}

const SIERP_POINT_LIST *sierp_points(SIERP *sierp)
{
    return(sierp->points);
}

int sierp_points_size_set(SIERP *sierp, int size)
{
    sierp_point_list_size_set(sierp->points, size);
    return(0);
}

int sierp_vertex_num(SIERP *sierp)
{
    return sierp_point_list_size_get(sierp->vertex);
}

double sierp_divisor_set(SIERP *sierp, double divisor)
{
    if( divisor == 0 )
        return sierp->divisor;
    sierp->divisor = divisor;
    return(sierp->divisor);
}

double sierp_divisor_get(SIERP *sierp)
{
    return(sierp->divisor);
}

const SIERP_POINT *sierp_vertex_get(SIERP *sierp, int index)
{
    return sierp_point_list_get_index(sierp->vertex, index);
}

int sierp_flag_set(SIERP *sierp, int flags)
{
    sierp->flags |= flags;
    return(sierp->flags);
}

int sierp_flag_clear(SIERP *sierp, int flags)
{
    sierp->flags |= flags;
    sierp->flags ^= flags;

    return(sierp->flags);
}

int sierp_flag_isset(SIERP *sierp, int flags)
{
    return( sierp->flags & flags );
}

/*
 * Private Functions
 */

static double sierp_radian_align_bottom(SIERP *sierp, int num_vertices)
{
    double radians;
    radians = 3*M_PI/2 - M_PI / num_vertices;
    return(radians);
}

