/*
 * Author: youngtrips(youngtrips@163.com)
 * Created Time:  2011-02-10
 * File Name: dlist.h
 * Description: 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#ifndef __DLIST_H
#define __DLIST_H

/* modified from linux/list.h */
#include <stddef.h>

typedef struct dlist_t {
	struct dlist_t *prev;
	struct dlist_t *next;
}dlist_t;


static inline void init_dlist_node(dlist_t *node) {
	node->next = node;
	node->prev = node;
}

static inline void __dlist_add(dlist_t *node,
		dlist_t *prev, dlist_t *next) {
	next->prev = node;
	node->next = next;
	node->prev = prev;
	prev->next = node;
}

static inline void dlist_add(dlist_t *node, dlist_t *head) {
	__dlist_add(node, head, head->next);
}

static inline void dlist_add_tail(dlist_t *node, dlist_t *head) {
	__dlist_add(node, head->prev, head);
}

static inline void __dlist_del(dlist_t *prev, dlist_t *next) {
	next->prev = prev;
	prev->next = next;
}


static inline void dlist_del(dlist_t *node) {
	__dlist_del(node->prev, node->next);
	node->next = node;
	node->prev = node;
}


#define dlist_entry(ptr, type, memb) \
	(((char*)ptr) - offsetof(type, memb))

#define dlist_foreach(pos, head) \
	for(pos = (head)->next;pos != (head); pos = pos->next)

#define dlist_foreach_safe(pos, n, head) \
	for(pos = (head)->next, n = pos->next; pos != (head); pos = n, n = pos->next)


#endif
