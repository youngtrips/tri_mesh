#include <stdio.h>

#include "mesh.h"

vertex_t verties[10];
int main()
{
    mesh_t *mesh;
    int cnt;

    cnt = 0;
    INIT_VERTEX(&verties[cnt], 283, 185); cnt++;
    INIT_VERTEX(&verties[cnt], 156, 307); cnt++;
    INIT_VERTEX(&verties[cnt], 367, 430); cnt++;
    INIT_VERTEX(&verties[cnt], 567, 299); cnt++;
    INIT_VERTEX(&verties[cnt], 493, 133); cnt++;

    printf("cnt=%d\n", cnt);
    mesh = mesh_init(verties, cnt);

    mesh_dump_vertex(mesh);
    mesh_dump_edge(mesh);
    mesh_dump_triangle(mesh);

    mesh_destroy(mesh);
    return 0;
}

