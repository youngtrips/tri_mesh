#!/usr/bin/env python
#-*- coding: utf-8 -*-

# Author: youngtrips(youngtrips@gmail.com)
# Created Time:  2011-12-23
# File Name: test.py
# Description: 
#

import mesh

verties = []
verties.append((283, 185))
verties.append((156, 307))
verties.append((367, 430))
verties.append((567, 299))
verties.append((493, 133))


h = mesh.MeshObject(verties, len(verties))

"""
h.dump_vertex()
h.dump_edge()
h.dump_triangle()
"""
tlist = h.triangles()

for t in tlist:
    print list(t)
