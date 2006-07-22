#include <stdlib.h>
#include <stdio.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include <sierp/sierp.h>
#include <sierp/sierp_point_list.h>

int main(int argc, char *argv[])
{
    SIERP *sierp;
    int i, n;
    const SIERP_POINT_LIST *points;
    const int iter = 10000;

    sierp = sierp_new();

    /* Try changing number of vertices */
    sierp_vertex_set(sierp, 3, 100);

    /* loop through and print vertices */
    n = sierp_vertex_num(sierp);
    for(i=0; i<n; i++) {
        const SIERP_POINT *p;
        p = sierp_vertex_get(sierp, i);
        printf("%d %d\n", p->x, p->y);
    }

    sierp_points_size_set(sierp, iter);
    sierp_update(sierp, iter);
    /* loop through and print points */
    points = sierp_points(sierp);
    for(i=0; i<iter; i++) {
        const SIERP_POINT *p;
        p = sierp_point_list_get_index(points, i);
        if( p == NULL ) {
            fprintf(stderr, "Error: ran out of values (%d)\n", i);
            break;
        }
        printf("%d %d\n", p->x, p->y);
    }

    sierp_delete(sierp);
    return(EXIT_SUCCESS);
}
