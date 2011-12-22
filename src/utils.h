/*
 * Author: youngtrips
 * Created Time:  Mon 12 Dec 2011 12:35:25 PM CST
 * File Name: utils.h
 * Description: 
 */

#ifndef __UTILS_H
#define __UTILS_H

#include <math.h>

#include "mesh.h"

#define SQR(x) ((x) * (x))
#define log_err(fmt, args...) fprintf(stderr, fmt, ##args)
#define eps 1e-12

int in_circumcircle(const vertex_t *a, const vertex_t *b, const vertex_t *c,
        const vertex_t *d);


int create_bounding_triangle(const vertex_t *vertices, int num,
        vertex_t *out);

#define DET(x1, y1, x2, y2) \
    ((x1) * (y1) - (x2) * (y1))

double cross(const vertex_t *a, const vertex_t *b, const vertex_t *c);

void print_vertex(const vertex_t *v);

#define makeup_ccw_order(v) \
        do { \
            vertex_t *a = v[0]; \
            vertex_t *b = v[1]; \
            vertex_t *c = v[2]; \
            if (cross(a, b, c) > 0) { \
                /* vector c is on left side of line ab */ \
                v[0] = a; \
                v[1] = b; \
                v[2] = c; \
            } else { \
                /* vector c is on right side of line ab */ \
                v[0] = b; \
                v[1] = a; \
                v[2] = c; \
            } \
        } while(0)


#define vertex_equal(a, b) \
    (fabs((a)->x - (b)->x) < eps && fabs((a)->y - (b)->y) < eps)

int in_triangle(const triangle_t *tri, const vertex_t *p);
int in_tri_circumcircle(const triangle_t *tri, const vertex_t *p);

#endif

