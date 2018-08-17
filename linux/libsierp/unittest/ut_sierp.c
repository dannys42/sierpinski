#include <sierp/sierp.h>
#include <stdlib.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char *argv[])
{
    SIERP *sierp;
    int i, j, n;

    sierp = sierp_new();

    /* Try changing number of vertices */
    sierp_vertex_set(sierp, 5, 100);

    /* loop through vertices a few times */
    n = sierp_vertex_num(sierp);
    for(i=0; i<10; i++) {
        for(j=0; j<n; j++) {
            sierp_vertex_get(sierp, j);
        }
    }

    sierp_delete(sierp);
    return(EXIT_SUCCESS);
}
