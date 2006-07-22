#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include "sierp/sierp.h"
#include "sierp/sierp_point_list.h"

struct SIERP {
    SIERP_POINT_LIST *vertex;
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
    sierp->radius = 100;
    sierp->iter_per_update = 100;
    sierp->flags = 0;
    sierp_flag_set(sierp, SIERP_FLAG_ALIGN_BOTTOM);

    /* Default vertex type */
    sierp_vertex_set(sierp, 3, 100);

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
        rotate = 0;
    }

    for(i=0; i<num_vertices; i++) {
        double radian;
        radian = 2*M_PI * (double)i / num_vertices;
        radian += rotate;
/*        printf("radian: %f   %f\n", radian, radian / (2*M_PI));*/
        p.x = (int) (radius * cos(radian));
        p.y = (int) (radius * sin(radian));
        p.iter = 0;
        sierp_point_list_push_point(sierp->vertex, &p);
    }
    sierp->radius = radius;
    return NULL;
}

int sierp_vertex_num(SIERP *sierp)
{
    return sierp_point_list_size_get(sierp->vertex);
}

const SIERP_POINT *sierp_vertex_get(SIERP *sierp, int index)
{
    return sierp_point_get_index(sierp->vertex, index);
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

