/*
 * Author: youngtrips
 * Created Time:  Thu 22 Dec 2011 03:21:57 PM CST
 * File Name: mesh.c
 * Description: 
 */

#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "mesh.h"

static vertex_t *mesh_alloc_vertex(mesh_t *self)
{
    return NULL;
}

static half_edge_t *mesh_alloc_half_edge(mesh_t *self)
{
    return NULL;
}

static triangle_t *mesh_alloc_triangle(mesh_t *self)
{
    return NULL;
}

static void mesh_free_half_edge(mesh_t *self, half_edge_t *he)
{
}

static void mesh_free_triangle(mesh_t *self, triangle_t *tri)
{
}


static vertex_t *mesh_get_vertex(mesh_t *self, double x, double y)
{
    return NULL;
}

static vertex_t *mesh_add_vertex(mesh_t *self, double x, double y)
{
    vertex_t *vert;

    vert = mesh_get_vertex(self, x, y);
    if (vert == NULL) {
        vert = mesh_alloc_vertex(self);
        if (vert == NULL) {
            return NULL;
        }
        INIT_VERTEX(vert, x, y);
        MESH_ADD_VERTEX(self, vert);
    }

    return vert;
}

static half_edge_t *mesh_add_half_edge(mesh_t *self, vertex_t *st, vertex_ed)
{
    half_edge_t *he;
    /* find if existed such half_edge */
    he = st->edge;

    do {
        /* the pair edge must be exist, since edge is inserted or deleted by pair*/
        pair = he->pair;
        if (vertex_equal(he->vert, st) && vertex_equal(pair->vert, ed)) {
            return he;
        }
        he = pair->next;
    } while (he && he != st->edge);

    he = mesh_alloc_half_edge(self);
    INIT_HALF_EDGE(he, st);
    return he;
}

static half_edge_t *mesh_add_edge(mesh_t *self, vertex_t *st, vertex_t *ed)
{
    half_edge_t *he;
    half_edge_t *he_pair;

    he = mesh_add_half_edge(self, st, ed);
    he_pair = mesh_add_half_edge(self, ed, st);

    he->pair = he_pair;
    he_pair->pair = he;

    return he;
}

static void mesh_del_half_edge(mesh_t *self, half_edge_t *he)
{
    half_edge_t *cur;

    cur = he->vert->edge;
    do {
        if (cur != he) {
            he->vert->edge = cur;
            break;
        }
        cur = cur->pair->next;
    } while (cur && cur != he->vert->edge);
}

static void mesh_del_edge(mesh_t *self, half_edge_t *he)
{
    half_edge_t *pair;

    pair = he->pair;
    mesh_del_half_edge(self, he);
    mesh_del_half_edge(self, pair);

    dlist_del(&(he->node));
    dlist_del(&(pair->node));

    mesh_free_half_edge(self, he);
    mesh_free_half_edge(self, pair);
}

#define makeup_edge_loop(he) \
    do { \
        he[0]->next = he[1]; \
        he[1]->next = he[2]; \
        he[2]->next = he[0]; \
    } while(0)

static int mesh_add_triangle(mesh_t *self,
        double x0, double y0,
        double x1, double y1,
        double x2, double y2)
{
    half_edge_t *he[3];
    triangle_t *tri;
    vertex_t *v[3];

    v[0] = mesh_add_vertex(self, x0, y0);
    v[1] = mesh_add_vertex(self, x1, y1);
    v[2] = mesh_add_vertex(self, x2, y2);

    makeup_ccw_order(v);

    he[0] = mesh_add_edge(self, v[0], v[1]);
    he[1] = mesh_add_edge(self, v[1], v[2]);
    he[2] = mesh_add_edge(self, v[2], v[0]);
    makeup_edge_loop(he);

    tri = mesh_alloc_triangle(self);
    tri->edge =  he[0];

    he[0]->face = tri;
    he[1]->face = tri;
    he[2]->face = tri;

    MESH_ADD_TRIANGLE(self, tri);
    return 0;
}

static void mesh_del_triangle(mesh_t *self, triangle_t *tri)
{
    /* dlist triange from mesh's tlist */

    dlist_del(&(tri->node));
    mesh_free_triangle(self, tri);
}



