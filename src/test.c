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

static const int VERTEX_BUCKET_COUNT = (1<<12);

inline int computeVertexHash(int x, int y, int z)
{
	const unsigned int h1 = 0x8da6b343; // Large multiplicative constants;
	const unsigned int h2 = 0xd8163841; // here arbitrarily chosen primes
	const unsigned int h3 = 0xcb1ab31f;
	unsigned int n = h1 * x + h2 * y + h3 * z;
	return (int)(n & (VERTEX_BUCKET_COUNT-1));
}


