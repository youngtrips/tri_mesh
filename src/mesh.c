/*
 * Author: youngtrips
 * Created Time:  Thu 22 Dec 2011 03:21:57 PM CST
 * File Name: mesh.c
 * Description: 
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"
#include "utils.h"
#include "mesh.h"

#define MESH_ALLOC_OBJECT(type, gclist, addr) \
    do { \
        dlist_t *i = (gclist)->next; \
        if (i == gclist) { \
            addr = (type*)malloc(sizeof(type)); \
            if (addr == NULL) { \
                log_err("%s:%s:%d : malloc() failed.", \
                        __FILE__, __FUNCTION__, __LINE__); \
                return NULL; \
            } \
        } else { \
            addr = (type*)dlist_entry(i, type, node); \
            dlist_del(i); \
        } \
    } while(0)

#define MESH_FREE_OBJECT(addr, gclist) \
    do { \
        dlist_del(&(addr->node)); \
        init_dlist_node(&(addr->node)); \
        dlist_add_tail(&(addr->node), gclist); \
    } while(0)

static vertex_t *mesh_alloc_vertex(mesh_t *self)
{
    vertex_t *vert;
    vert = (vertex_t*)malloc(sizeof(vertex_t));
    return vert;
}

static half_edge_t *mesh_alloc_half_edge(mesh_t *self)
{
    half_edge_t *he;
    MESH_ALLOC_OBJECT(half_edge_t, &(self->gc_elist), he);
    return he;
}

static void mesh_free_half_edge(mesh_t *self, half_edge_t *he)
{
    MESH_FREE_OBJECT(he, &(self->gc_elist));
}

static triangle_t *mesh_alloc_triangle(mesh_t *self)
{
    triangle_t *tri = NULL;
    MESH_ALLOC_OBJECT(triangle_t, &(self->gc_tlist), tri);
    return tri;
}

static void mesh_free_triangle(mesh_t *self, triangle_t *tri)
{
    MESH_FREE_OBJECT(tri, &(self->gc_tlist));
}

static vertex_t *mesh_get_vertex(mesh_t *self, double x, double y)
{
    vertex_t tmp;
    vertex_t *vert;
    dlist_t *i;

    INIT_VERTEX(&tmp, x, y);
    dlist_foreach(i, &(self->vlist)) {
        vert = (vertex_t*)dlist_entry(i, vertex_t, node);
//        print_vertex(vert); printf("<--->");
//        print_vertex(&tmp);
//        printf("\n");
        if (vertex_equal(vert, &tmp)) {
//            printf("same vertex...\n");
            return vert;
        }
    }
    return NULL;
}

static vertex_t *mesh_add_new_vertex(mesh_t *self, double x, double y)
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

static half_edge_t *mesh_add_half_edge(mesh_t *self,
        vertex_t *st, vertex_t *ed)
{
    half_edge_t *pair;
    half_edge_t *he;
    /* find if existed such half_edge */
    he = st->edge;

#ifdef __DEBUG
    if (he) {
        do {
            // the pair edge must be exist, 
            // since edge is inserted or deleted by pair 
            pair = he->pair;
            if (vertex_equal(he->vert, st) && vertex_equal(pair->vert, ed)) {
                printf("1same half edge...\n");
                return he;
            } else if (vertex_equal(pair->vert, st) && 
                    vertex_equal(he->vert, ed)) {
                printf("2same half edge...\n");
                return pair;
            }
            he = pair->next;
        } while (he && he != st->edge);
    }
    
#else

    dlist_t *i;

    dlist_foreach(i, &(self->elist)) {
        he = (half_edge_t*)dlist_entry(i, half_edge_t, node);
        pair = he->pair;
        if (vertex_equal(he->vert, st) && vertex_equal(pair->vert, ed)) {
            return he;
        }
    }
#endif

    he = mesh_alloc_half_edge(self);
    if (he == NULL) {
        printf("alloc half_edge failed...\n");
        return NULL;
    }
    INIT_HALF_EDGE(he, st);
    dlist_add(&(he->node), &(self->elist));
    st->edge = he;
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

//    printf("del half_edge: addr=%p\n", he);
//    printf("del half_edge: addr=%p\n", pair);

    //dlist_del(&(he->node));
    //dlist_del(&(pair->node));

    mesh_free_half_edge(self, he);
    mesh_free_half_edge(self, pair);
}

#define makeup_edge_loop(he) \
    do { \
        he[0]->next = he[1]; \
        he[1]->next = he[2]; \
        he[2]->next = he[0]; \
    } while(0)

static triangle_t *mesh_add_triangle_with_verties(mesh_t *self,
        double x0, double y0,
        double x1, double y1,
        double x2, double y2)
{
    half_edge_t *he[3];
    triangle_t *tri;
    vertex_t *v[3];

    v[0] = mesh_add_new_vertex(self, x0, y0);
    v[1] = mesh_add_new_vertex(self, x1, y1);
    v[2] = mesh_add_new_vertex(self, x2, y2);

    makeup_ccw_order(v);

//    print_vertex(v[0]); printf("\n");
//    print_vertex(v[1]); printf("\n");
//    print_vertex(v[2]); printf("\n");

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
    ++self->num_t;
    return tri;
}

static triangle_t *mesh_add_triangle(mesh_t *self,
        half_edge_t *he, double x, double y)
{
    vertex_t *v0 = he->vert;
    vertex_t *v1 = he->pair->vert;
    return mesh_add_triangle_with_verties(self,
            v0->x, v0->y,
            v1->x, v1->y,
            x, y);
}

static void mesh_del_triangle(mesh_t *self, triangle_t *tri)
{
    /* dlist triange from mesh's tlist */

    dlist_del(&(tri->node));
    mesh_free_triangle(self, tri);
    --self->num_t;
}

static void mesh_del_bad_triangle(mesh_t *self, triangle_t *from, vertex_t *p,
        queue_t *valid_edge)
{
    triangle_t *adj_tri;
    half_edge_t *tmp;
    half_edge_t *he;

    he = from->edge;
    do {
        tmp = he->next;
        if (he->pair->face && in_tri_circumcircle(he->pair->face, p)) {
            // mark bad edge
            adj_tri = he->pair->face;
            he->face = NULL;
            he->pair->face = NULL;

#ifdef _DEBUG
            printf("bad edge: addr=%p, pair=%p, ", he, he->pair);
            print_vertex(he->vert);
            printf("--"); print_vertex(he->pair->vert);
            printf("\n");
#endif

            mesh_del_bad_triangle(self, adj_tri, p, valid_edge);
            // delete current bad edge he
            mesh_del_edge(self, he);

        }
        if (he->face) {
            // vaild edge
            queue_push(valid_edge, he);
        }
        he = tmp;
    } while(he != from->edge);
    mesh_del_triangle(self, from);
}

static void mesh_create_super_triangle(mesh_t *self,
        const vertex_t *verties, int num)
{
    half_edge_t *pair[3];
    half_edge_t *he[3];
    triangle_t *tri;
    vertex_t v[3];
    create_bounding_triangle(verties, num, v);
    tri = mesh_add_triangle_with_verties(self,
            v[0].x, v[0].y,
            v[1].x, v[1].y,
            v[2].x, v[2].y);
    
    INIT_VERTEX(&(self->super_tri[0]), v[0].x, v[0].y);
    INIT_VERTEX(&(self->super_tri[1]), v[1].x, v[1].y);
    INIT_VERTEX(&(self->super_tri[2]), v[2].x, v[2].y);


    /*
    he[0] = tri->edge;
    he[1] = he[0]->next;
    he[2] = he[1]->next;

    pair[0] = he[0]->pair;
    pair[1] = he[1]->pair;
    pair[2] = he[2]->pair;

    pair[0]->next = pair[2];
    pair[1]->next = pair[0];
    pair[2]->next = pair[1];
    */
}

static triangle_t *mesh_locate(mesh_t *self, const vertex_t *v)
{
    triangle_t *tri;
    dlist_t *i;
    int ret;

    /* TODO: use search methd with O(logN) */
    dlist_foreach(i, &(self->tlist)) {
        tri = (triangle_t*)dlist_entry(i, triangle_t, node);
        ret = in_triangle(tri, v);
        if (ret == -1) {
            return NULL;
        }
        if (ret == 1) {
            return tri;
        }
    }
    return NULL;
}

mesh_t *mesh_init(const vertex_t *verties, int num)
{
    mesh_t *self;
    int i;

    self = (mesh_t*)malloc(sizeof(mesh_t));
    if (self == NULL) {
        return NULL;
    }

    init_dlist_node(&(self->vlist));
    init_dlist_node(&(self->elist));
    init_dlist_node(&(self->tlist));
    init_dlist_node(&(self->gc_elist));
    init_dlist_node(&(self->gc_tlist));
    self->num_v = 0;
    self->num_e = 0;
    self->num_t = 0;

    if (num > 0) {
        /* create super triangle */
        mesh_create_super_triangle(self, verties, num);
        for (i = 0;i < num; i++) {
            mesh_add_vertex(self, verties[i].x, verties[i].y);
        }
    }
    return self;
}

int mesh_set_box(mesh_t *self, double left, double top, int width, int height)
{
    vertex_t v[4];

    if (self == NULL) {
        return -1;
    }

    v[0].x = left;
    v[0].y = top;

    v[1].x = left + width;
    v[1].y = top;

    v[2].x = left + width;
    v[2].y = top + height;

    v[3].x = left;
    v[3].y = top + height;

    mesh_create_super_triangle(self, v, 4);
    return 0;
}

int mesh_add_vertex(mesh_t *self, double x, double y)
{
    half_edge_t *he;
    triangle_t *tri;
    vertex_t vert;
    queue_t *edges;
    
    INIT_VERTEX(&vert, x, y);
    tri = mesh_locate(self, &vert);
    if (tri == NULL) {
        printf("can't find a triangle to \
                contain the vertex:(%0.0lf, %0.0lf)\n", x, y);
        return -1;
    }
#ifdef _DEBUG
    printf("tri = %p\n", tri);
#endif

    edges = queue_init();
    mesh_del_bad_triangle(self, tri, &vert, edges);

#ifdef _DEBUG
    mesh_dump_edge(self);
    mesh_dump_triangle(self);
#endif

    while (!queue_empty(edges)) {
        he = queue_pop(edges);
#ifdef _DEBUG
        printf("Valid edge: addr=%p, ", he);
        print_vertex(he->vert); printf("----");
        print_vertex(he->pair->vert);
        printf("\n");
#endif
        mesh_add_triangle(self, he, x, y);
    }
    queue_destroy(edges);
    return 0;
}

int mesh_add_constrained_poly(mesh_t *self, vertex_t *verties, int num)
{
    return 0;
}

int mesh_add_constrained_line(mesh_t *self, vertex_t *st, vertex_t *ed)
{
    return 0;
}

#define MESH_FREE_OBJ_MEM(list, type) \
    do { \
        void *addr; \
        dlist_t *i; \
        dlist_t *j; \
        dlist_foreach_safe(i, j, list) { \
            addr = dlist_entry(i, type, node); \
            free(addr); \
        } \
    } while(0)
 
void mesh_destroy(mesh_t *self)
{
    if (self == NULL) {
        return;
    }
    MESH_FREE_OBJ_MEM(&(self->vlist), vertex_t);
    MESH_FREE_OBJ_MEM(&(self->elist), half_edge_t);
    MESH_FREE_OBJ_MEM(&(self->tlist), triangle_t);
    MESH_FREE_OBJ_MEM(&(self->gc_elist), half_edge_t);
    MESH_FREE_OBJ_MEM(&(self->gc_tlist), triangle_t);
    free(self);
}

void mesh_dump_vertex(mesh_t *self)
{
    dlist_t *i;
    vertex_t *vert;
    int cnt;

    printf("------------------verties----------------------\n");
    cnt = 0;
    dlist_foreach(i, &(self->vlist)) {
        vert = (vertex_t*)dlist_entry(i, vertex_t, node);
        printf("NO.%02d, addr=%p, edge=%p, ", ++cnt, vert, vert->edge);
        print_vertex(vert);
        printf("\n");
    }
}

void mesh_dump_edge(mesh_t *self)
{    
    dlist_t *i;
    half_edge_t *he;
    int cnt = 0;

    printf("------------------half_edge----------------------\n");
    dlist_foreach(i, &(self->elist)) {
        he = (half_edge_t*)dlist_entry(i, half_edge_t, node);
        if (he && he->pair) {
            printf("NO.%02d, addr=%p, pair=%p, next=%p, face=%p, vert=%p ,",
                    ++cnt, he, he->pair, he->next, he->face, he->vert);
            print_vertex(he->vert);
            printf("-");
            print_vertex(he->pair->vert);
            printf("\n");
        }
    }

}

void mesh_dump_triangle(mesh_t *self)
{
    dlist_t *i;
    half_edge_t *he;
    triangle_t *tri;
    int cnt;

    printf("------------------triangle----------------------\n");
    printf("triangle num: %d\n", self->num_t);
    cnt = 0;
    dlist_foreach(i, &(self->tlist)) {
        tri = (triangle_t*)dlist_entry(i, triangle_t, node);
        he = tri->edge;
        printf("NO.%02d, addr=%p, edge=%p, ", ++cnt, tri, tri->edge);
        do {
            if (he) {
                print_vertex(he->vert);
                printf(" ");
                he = he->next;
            }
        } while(he && he != tri->edge);
        printf("\n");
    }
}

