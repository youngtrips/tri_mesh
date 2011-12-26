#!/usr/bin/env python
#-*- coding: utf-8 -*-

# Author: youngtrips(youngtrips@gmail.com)
# Created Time:  2011-12-08
# File Name: draw.py
# Description: 
#

import pygame
import math
import mesh

def draw_points(screen, points):
    c = pygame.Color('red')
    for p in points:
        screen.set_at(p, c)

def draw_polygons(screen, polygons):
    c = pygame.Color('blue')
    for e in polygons:
        pygame.draw.polygon(screen, c, e, 1)

if __name__ == "__main__":
    w = 1280 
    h = 900
    screen = pygame.display.set_mode((w, h), pygame.DOUBLEBUF | pygame.HWSURFACE, 32)
    pygame.display.set_caption("Triangle Mesh PyBinding Demo")
    bg = pygame.Surface(screen.get_size())
    bg.fill((0, 0, 0))
    screen.blit(bg, (0, 0))
    pygame.display.flip()
    clock = pygame.time.Clock()
    running = True
    polygons = []
    points = []
    mobj = mesh.MeshObject([], 0)
    mobj.set_box(0, 0, w, h)
    while True:
        clock.tick(50)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
                break
            elif event.type == pygame.MOUSEBUTTONUP:
                mobj.add_vertex(event.pos[0], event.pos[1])
                points.append(event.pos)
                polygons = mobj.triangles()

        if not running: break
        screen.blit(bg, (0, 0))
        draw_polygons(screen, polygons)
        draw_points(screen, points)
        pygame.display.flip()


