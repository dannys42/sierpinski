#include <stdlib.h>
#include <stdio.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include <sierp/sierp.h>

int main(int argc, char *argv[])
{
    SIERP *sierp;
    int i, n;

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

    sierp_delete(sierp);
    return(EXIT_SUCCESS);
}
