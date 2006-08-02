/**
  * @defgroup SIERP SIERP
  * C-style class library for generating 2D Sierpinski polygons
  * @{
  */
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

#ifndef min
#define min(a,b)    ((a)<(b) ? (a) : (b))
#endif

#ifndef max
#define max(a,b)    ((a)>(b) ? (a) : (b))
#endif

#define SIERP_DEFAULT_VERTICES      3
#define SIERP_DEFAULT_DIVISOR       2
#define SIERP_DEFAULT_RADIUS        100
#define SIERP_DEFAULT_POINT_SIZE    1000

/** SIERP object handle
  *
  * Control points are the "actual" vertices used in the math
  * calculations.
  *
  * The 'vertex' variable, however, holds the adjusted values for the
  * points that coincide with the calculated data points.
  */
struct SIERP {
    SIERP_POINT_LIST *vertex;   /**< List of vertices */
    SIERP_POINT_LIST *control;  /**< List of control points */
    SIERP_POINT_LIST *points;   /**< List of fill points */
    SIERP_POINT control_last;   /**< Last point drawn */
    double divisor;
    int flags;
    int radius;
    int iter_per_update;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

/*
 * Private Function Prototypes
 */ 
static double sierp_radian_align_bottom(SIERP *sierp, int num_vertices);
static void sierp_vertex_update(SIERP *sierp);

/*
 * Public Function definitions
 */

/**
  * Create a new SIERP object.
  * 
  * @return (SIERP *) to new object
  * @retval NULL if insufficient memory
  */
SIERP *sierp_new(void)
{
    SIERP *sierp;
    sierp = (SIERP *)malloc(sizeof(SIERP));
    if( sierp == NULL )
        return NULL;

    /* Initialize values */
    sierp->vertex = NULL;
    sierp->control = NULL;
    sierp->points = NULL;
    sierp->radius = 100;
    sierp->iter_per_update = 100;
    sierp->flags = 0;
    sierp->divisor = SIERP_DEFAULT_DIVISOR;
    sierp->x_min = 0;
    sierp->x_max = 0;
    sierp->y_min = 0;
    sierp->y_max = 0;
    sierp_flag_set(sierp, SIERP_FLAG_ALIGN_BOTTOM);

    /* Default vertex type */
    sierp_vertex_set(sierp, SIERP_DEFAULT_VERTICES, SIERP_DEFAULT_RADIUS);

    /* Default point size */
    sierp->points = sierp_point_list_new();
    sierp_point_list_size_set(sierp->points, SIERP_DEFAULT_POINT_SIZE);

    return sierp;
}

/**
  * Destroy SIERP handle
  *
  * @param sierp pointer to object
  * @retval NULL
  */
SIERP *sierp_delete(SIERP *sierp)
{
    if( sierp == NULL )
        return NULL;
    if( sierp->control != NULL ) {
        sierp_point_list_delete(sierp->control);
        sierp->control = NULL;
    }
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

/**
  * Clear all points currently accumulated.
  *
  * @note This does not actually free any memory.
  *
  * @param sierp handle to SIERP object
  */
void sierp_clear(SIERP *sierp)
{
    if( sierp == NULL )
        return;
    if( sierp->points == NULL )
        return;
    sierp_point_list_clear(sierp->points);
}

/**
  * Set the number of vertices and size of the Sierpinski polygon.
  *
  * This function uses the circle of a given radius and creates a new
  * equilateral polygon with the specified number of vertices.
  * 
  * @note All accumulated fill points will be destroyed
  * 
  * @param sierp handle to SIERP object
  * @param num_vertices number of vertices.  This must be > 0
  * @param radius the size of the circle along whose path the shape is
  *             created
  * @retval NULL
  */
SIERP *sierp_vertex_set(SIERP *sierp, int num_vertices, int radius)
{
    int i;
    SIERP_POINT p;
    double rotate = 0;

    if( sierp->control != NULL ) {
        sierp_point_list_delete(sierp->control);
    }
    sierp->control = sierp_point_list_new();
    sierp_point_list_size_set(sierp->control, num_vertices);

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
        sierp_point_list_push_point(sierp->control, &p);
    }
    sierp->radius = radius;

    /* Now update the vertices */
    sierp_vertex_update(sierp);

    /* Just pick an arbitrary vertex to start with */
    sierp->control_last = p;
    return NULL;
}

/**
  * Calculate the next batch of fill points.
  *
  * @param sierp SIERP handle
  * @param steps number of points to calculate this iteration
  * @retval 0
  */
int sierp_update(SIERP *sierp, int steps)
{
    int vertex_idx;
    const SIERP_POINT *vertex;
    SIERP_POINT point;

    for( ; steps > 0; steps-- ) {
        /* pick a random vertex to go to */
        float rval;
        rval = (float)rand();
        rval = rval / RAND_MAX * sierp_vertex_num(sierp);

        vertex_idx = (int)rval;
        vertex = sierp_control_get(sierp, vertex_idx);

        point.x = sierp->control_last.x + vertex->x;
        point.x /= sierp->divisor;

        point.y = sierp->control_last.y + vertex->y;
        point.y /= sierp->divisor;

        sierp_point_list_push_point(sierp->points, &point);
        sierp->control_last = point;
    }
    return(0);
}

/**
  * Get the fill points for the current polygon.
  *
  * @param sierp handle to SIERP object
  * @return pointer to SIERP_POINT_LIST containing all fill points
  */
const SIERP_POINT_LIST *sierp_points(SIERP *sierp)
{
    return(sierp->points);
}

/**
  * Set the number of fill points.
  *
  * @param sierp handle to SIERP object
  * @param size maximum number of fill points
  * @retval 0
  */
int sierp_points_size_set(SIERP *sierp, int size)
{
    sierp_point_list_size_set(sierp->points, size);
    return(0);
}

/**
  * Get the number of vertices
  *
  * @param sierp handle to SIERP object
  * @return number of vertices in current polygon
  */
int sierp_vertex_num(SIERP *sierp)
{
    return sierp_point_list_size_get(sierp->control);
}

/**
  * Set the divisor used in point generation
  * 
  * @note All values except 0 are allowed.
  * 
  * @param sierp handle to SIERP object
  * @param divisor divisor to use in calculations
  * @return new divisor value
  */
double sierp_divisor_set(SIERP *sierp, double divisor)
{
    if( divisor == 0 )
        return sierp->divisor;
    sierp->divisor = divisor;

    sierp_vertex_update(sierp);

    return(sierp->divisor);
}

/**
  * Retrieve the current divisor
  *
  * @param sierp handle to SIERP object
  * @reutrn current divisor value
  */
double sierp_divisor_get(SIERP *sierp)
{
    return(sierp->divisor);
}

/**
  * Get a vertex coordinate
  *
  * @param sierp handle to SIERP object
  * @param index index of vertex to get
  * @return SIERP_POINT handle of vertex
  */
const SIERP_POINT *sierp_control_get(SIERP *sierp, int index)
{
    return sierp_point_list_get_index(sierp->control, index);
}

/**
  * Get a vertex coordinate
  *
  * @param sierp handle to SIERP object
  * @param index index of vertex to get
  * @return SIERP_POINT handle of vertex
  */
const SIERP_POINT *sierp_vertex_get(SIERP *sierp, int index)
{
    return sierp_point_list_get_index(sierp->vertex, index);
}

/**
  * Set any number of option flags
  *
  * Currently supported flags:
  * SIERP_FLAG_ALIGN_BOTTOM - align bottom horizontally
  *
  * @param sierp handle to SIERP object
  * @param flags OR'd flags to set
  * @return new state of all flags
  */
int sierp_flag_set(SIERP *sierp, int flags)
{
    sierp->flags |= flags;
    return(sierp->flags);
}

/**
  * Clear option flags
  *
  * @param sierp handle to SIERP object
  * @param flags OR'd flags to clear
  * @return new state of all flags
  */
int sierp_flag_clear(SIERP *sierp, int flags)
{
    sierp->flags |= flags;
    sierp->flags ^= flags;

    return(sierp->flags);
}

/**
  * Determine if the given flags are set
  * @param sierp handle to SIERP object
  * @param flags OR'd flags to test
  * @return non-zero if all given flags are set
  */
int sierp_flag_isset(SIERP *sierp, int flags)
{
    return( (sierp->flags & flags) == flags );
}

/**
  * Get minimum x-coordinate based on vertices.
  *
  * This is useful for getting a bounding box around the polygon.
  *
  * @param sierp handle to SIERP object
  * @return minimum x-coordinate
  */
double sierp_x_min(SIERP *sierp)
{
    return( sierp->x_min );
}

/**
  * Get minimum x-coordinate based on vertices.
  *
  * This is useful for getting a bounding box around the polygon.
  *
  * @param sierp handle to SIERP object
  * @return minimum x-coordinate
  */
double sierp_x_max(SIERP *sierp)
{
    return( sierp->x_max );
}

/**
  * Get minimum y-coordinate based on vertices.
  *
  * This is useful for getting a bounding box around the polygon.
  *
  * @param sierp handle to SIERP object
  * @return minimum x-coordinate
  */
double sierp_y_min(SIERP *sierp)
{
    return( sierp->y_min );
}

/**
  * Get maximum y-coordinate based on vertices.
  *
  * This is useful for getting a bounding box around the polygon.
  *
  * @param sierp handle to SIERP object
  * @return maximum y-coordinate
  */
double sierp_y_max(SIERP *sierp)
{
    return( sierp->y_max );
}

/*
 * Private Functions
 */

/**
  * Calculate radian offset to ensure the polygon has the bottom most
  * segment horizontal.
  *
  * @param sierp handle to SIERP object
  * @apram num_vertices number of vertices in polygon
  * @return radian offset
  */
static double sierp_radian_align_bottom(SIERP *sierp, int num_vertices)
{
    double radians;
    radians = 3*M_PI/2 - M_PI / num_vertices;
    return(radians);
}

static void sierp_vertex_update(SIERP *sierp)
{
    int i;
    int num_vertices;

    if( sierp == NULL )
        return;

    num_vertices = sierp_point_list_size_get(sierp->control);
    if( sierp->vertex != NULL ) {
        sierp_point_list_delete(sierp->vertex);
    }
    sierp->vertex = sierp_point_list_new();
    sierp_point_list_size_set(sierp->vertex, num_vertices);
    for(i=0; i<num_vertices; i++) {
        const SIERP_POINT *point;
        SIERP_POINT vertex;
        point = sierp_point_list_get_index(sierp->control, i);
        vertex.x = point->x;
        vertex.y = point->y;
        if( sierp->divisor > 2 ) {
            vertex.x /= sierp->divisor - 1;
            vertex.y /= sierp->divisor - 1;
        }
        sierp_point_list_push_point(sierp->vertex, &vertex);

        if( i == 0 ) {
            sierp->x_min = vertex.x;
            sierp->x_max = vertex.x;
            sierp->y_min = vertex.y;
            sierp->y_max = vertex.y;
        } else {
            sierp->x_min = min(sierp->x_min, vertex.x);
            sierp->x_max = max(sierp->x_max, vertex.x);
            sierp->y_min = min(sierp->y_min, vertex.y);
            sierp->y_max = max(sierp->y_max, vertex.y);
        }
    }
}


/** @} */
