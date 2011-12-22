/*
 * Author: youngtrips
 * Created Time:  Mon 12 Dec 2011 12:36:09 PM CST
 * File Name: utils.c
 * Description: 
 */

#include <math.h>
#include <stdio.h>

#include "utils.h"

int calc_circumcircle(const vertex_t *a, const vertex_t *b, const vertex_t *c,
        circle_t *circle)
{
    real_t A = b->x - a->x;
    real_t B = b->y - a->y;
    real_t C = c->x - a->x;
    real_t D = c->y - a->y;

    real_t E = A * (a->x + b->x) + B * (a->y + b->y);
    real_t F = C * (a->x + c->x) + D * (a->y + c->y);

    real_t G = 2.0 * (A * (c->y - b->y) - B * (c->x - b->x));
    if (FABS(G) < eps) {
        return -1;
    }

    if (FABS(G) < eps) {
        return -1;
    }

    circle->x = (D * E - B * F) / G;
    circle->y = (A * F - C * E) / G;

    circle->r_sqr = SQR(a->x - circle->x) + SQR(a->y - circle->y);

    return 0;
}

int in_circle(const circle_t *c, const vertex_t *p)
{
    return SQR(p->x - c->x) + SQR(p->y - c->y) + eps <= c->r_sqr;
}

// This function is based on an article by Jonathan Richard Shewchuk
// "Robust Adaptive Floating-Point Geometric Predicates"
// | ax-dx  ay-dy  (ax-dx)^2+(ay-dy)^2 |
// | bx-dx  by-dy  (bx-dx)^2+(by-dy)^2 |
// | cx-dx  cy-dy  (cx-dx)^2+(cy-dy)^2 |
/* a, b and c must be in CCW order */
/* 
 * inside: 1
 * outside: -1
 * oncircle: 0
 */
int in_circle2(const vertex_t *a, const vertex_t *b, const vertex_t *c,
        const vertex_t *d)
{
  real_t ad_dx = a->x - d->x;
  real_t ad_dy = a->y - d->y;
  real_t ad_dsq = ad_dx * ad_dx + ad_dy * ad_dy;

  real_t bd_dx = b->x - d->x;
  real_t bd_dy = b->y - d->y;
  real_t bd_dsq = bd_dx * bd_dx + bd_dy * bd_dy;

  real_t cd_dx = c->x - d->x;
  real_t cd_dy = c->y - d->y;
  real_t cd_dsq = cd_dx * cd_dx + cd_dy * cd_dy;

  real_t calc = +ad_dx * (bd_dy * cd_dsq - cd_dy * bd_dsq)
          - ad_dy * (bd_dx * cd_dsq - cd_dx * bd_dsq)
          + ad_dsq * (bd_dx * cd_dy - cd_dx * bd_dy);

  return (calc > eps) ? 1
          : ((calc < eps) ? -1
             : 0);
}

int create_bounding_triangle(const vertex_t *vertices, int num, vertex_t **out)
{
    double minx;
    double miny;
    double maxx;
    double maxy;
    double dx;
    double dy;
    int i;
    vertex_t *v;

    minx = -1;
    miny = -1;
    maxx = -1;
    maxy = -1;
	for (i = 0;i < num; i++) {
        v = (vertex_t*)&vertices[i];
        if (minx == -1 || v->x < minx) minx = v->x;
        if (miny == -1 || v->y < miny) miny = v->y;
        if (maxx == -1 || v->x > maxx) maxx = v->x;
        if (maxy == -1 || v->y > maxy) maxy = v->y;
    }

    dx = (maxx - minx) * 10;
    dy = (maxy - miny) * 10;
	
    out[0]->x = minx - dx;
    out[0]->y = miny - dy * 3;

    out[1]->x = minx - dx;
    out[1]->y = maxy + dy;

    out[2]->x = maxx + dx * 3;
    out[2]->y = maxy + dy;

    return 0;
}


/* vector(P0P1) X vector(P0P2) */
double cross(const vertex_t *p0, const vertex_t *p1, const vertex_t *p2)
{
    return (p1->x - p0->x) * (p2->y - p0->y)  - (p2->x - p0->x) * (p1->y - p0->y);
}

void print_vertex(const vertex_t *v)
{
    if (v) {
        printf(REAL_FMT" "REAL_FMT, v->x, v->y);
    } else {
        printf(" ");
    }
}


