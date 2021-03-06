/*
 * Author: youngtrips
 * Created Time:  Thu 22 Dec 2011 03:07:02 PM CST
 * File Name: mesh.h
 * Description: 
 */

#ifndef __MESH_H
#define __MESH_H

#include "dlist.h"

typedef struct vertex_t vertex_t;
typedef struct half_edge_t half_edge_t;
typedef struct triangle_t triangle_t;

struct vertex_t {
    double x;
    double y;
    half_edge_t *edge;
    dlist_t node;
};

#define INIT_VERTEX(v, _x, _y) \
    do { \
        (v)->x = (_x); \
        (v)->y = (_y); \
        (v)->edge = NULL; \
        init_dlist_node(&((v)->node)); \
    } while(0)

struct half_edge_t {
    half_edge_t *pair;
    half_edge_t *next;
    triangle_t *face;
    vertex_t *vert;
    dlist_t node;
};

#define INIT_HALF_EDGE(he, v) \
    do { \
        (he)->pair = NULL; \
        (he)->next = NULL; \
        (he)->face = NULL; \
        (he)->vert = (v); \
        init_dlist_node(&((he)->node)); \
    } while(0)

struct triangle_t {
    half_edge_t *edge;
    dlist_t node;
};

typedef struct mesh_t {
    dlist_t vlist;
    dlist_t elist;
    dlist_t tlist;

    dlist_t gc_elist;
    dlist_t gc_tlist;

    vertex_t super_tri[3];

    int num_v;
    int num_e;
    int num_t;
} mesh_t;


#define MESH_ADD_VERTEX(mesh, v) \
        dlist_add(&((v)->node), &((mesh)->vlist))

#define MESH_ADD_EDGE(mesh, e) \
        dlist_add(&((e)->node), &((mesh)->elist))

#define MESH_ADD_TRIANGLE(mesh, t) \
        dlist_add(&((t)->node), &((mesh)->tlist))



mesh_t *mesh_init(const vertex_t *verties, int num);
int mesh_set_box(mesh_t *self, double left, double top, int width, int height);
int mesh_add_vertex(mesh_t *self, double x, double y);
int mesh_add_constrained_poly(mesh_t *self, vertex_t *verties, int num);
int mesh_add_constrained_line(mesh_t *self, vertex_t *st, vertex_t *ed);
void mesh_destroy(mesh_t *self);

/* for debug */
void mesh_dump_vertex(mesh_t *self);
void mesh_dump_edge(mesh_t *self);
void mesh_dump_triangle(mesh_t *self);

#endif

