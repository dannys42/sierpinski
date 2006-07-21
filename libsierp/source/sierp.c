#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sierp/sierp.h"
#include "sierp/sierp_point_list.h"

struct SIERP {
    SIERP_POINT_LIST *vertex;
    int radius;
    int iter_per_update;
};

SIERP *sierp_new(int num_vertices)
{
    SIERP *sierp;
    if( num_vertices <= 1 ) {
        return(NULL);
    }
    sierp = (SIERP *)malloc(sizeof(SIERP));

    /* Initialize values */
    sierp->vertex = NULL;
    sierp->radius = 100;
    sierp->iter_per_update = 100;

    sierp_vertex_set(sierp, num_vertices, 100);

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

    sierp->vertex = sierp_point_list_new();
    sierp_point_list_size_set(sierp->vertex, num_vertices);
    for(i=0; i<num_vertices; i++) {
        double radian;
        radian = 2*M_PI * i / num_vertices;
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
