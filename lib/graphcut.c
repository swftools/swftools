/*
    graphcut- a graphcut implementation based on the Boykov Kolmogorov algorithm 

    Part of the swftools package.

    Copyright (c) 2007,2008,2009 Matthias Kramm <kramm@quiss.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <memory.h>
#include "graphcut.h"
#include "mem.h"

//#define DEBUG

//#define CHECKS

#ifdef DEBUG
#define DBG
#include <assert.h>
#else
#define DBG if(0)
#define assert(x) (x)
#endif

#define ACTIVE 0x10
#define IN_TREE 0x20

#define TWOTREES

typedef struct _posqueue_entry {
    node_t*pos;
    struct _posqueue_entry*next;
} posqueue_entry_t;

typedef struct _posqueue {
    posqueue_entry_t*list;
} posqueue_t;

typedef struct _graphcut_workspace {
    unsigned char*flags1;
    unsigned char*flags2;
    halfedge_t**back;
    graph_t*graph;
    node_t*pos1;
    node_t*pos2;
    posqueue_t*queue1;
    posqueue_t*queue2;
    posqueue_t*tmpqueue;
} graphcut_workspace_t;

static posqueue_t*posqueue_new() 
{
    posqueue_t*m = (posqueue_t*)malloc(sizeof(posqueue_t));
    memset(m, 0, sizeof(posqueue_t));
    return m;
}
static void posqueue_delete(posqueue_t*q)
{
    posqueue_entry_t*l = q->list;
    while(l) {
	posqueue_entry_t*next = l->next;
	free(l);
	l = next;
    }
    free(q);
}
static inline void posqueue_addpos(posqueue_t*queue, node_t*pos)
{
    posqueue_entry_t*old = queue->list;
    queue->list = malloc(sizeof(posqueue_entry_t));
    queue->list->pos = pos;
    queue->list->next = old;
}
static inline node_t* posqueue_extract(posqueue_t*queue)
{
    posqueue_entry_t*item = queue->list;
    node_t*pos;
    if(!item)
	return 0;
    pos = item->pos;
    queue->list = queue->list->next;
    free(item);
    return pos;
}
static inline int posqueue_notempty(posqueue_t*queue)
{
    return (int)queue->list;
}

#define NR(p) ((p)->nr)

static void posqueue_print(graphcut_workspace_t*w, posqueue_t*queue)
{
    posqueue_entry_t*e = queue->list;
    while(e) {
	halfedge_t*back = w->back[NR(e->pos)];
	printf("%d(%d) ", NR(e->pos), back?NR(back->fwd->node):-1);
	e = e->next;
    }
    printf("\n");
}
static void posqueue_purge(posqueue_t*queue)
{
    posqueue_entry_t*e = queue->list;
    while(e) {
	posqueue_entry_t*next = e->next;
	e->next = 0;free(e);
	e = next;
    }
    queue->list = 0;
}

graph_t* graph_new(int num_nodes)
{
    graph_t*graph = rfx_calloc(sizeof(graph_t));
    graph->num_nodes = num_nodes;
    graph->nodes = rfx_calloc(sizeof(node_t)*num_nodes);
    int t;
    for(t=0;t<num_nodes;t++) {
	graph->nodes[t].nr = t;
    }
    return graph;
}

void graph_delete(graph_t*graph)
{
    int t;
    for(t=0;t<graph->num_nodes;t++) {
	halfedge_t*e = graph->nodes[t].edges;
	while(e) {
	    halfedge_t*next = e->next;
	    free(e);
	    e = next;
	}
    }
    free(graph->nodes);graph->nodes=0;
    free(graph);
}

static graphcut_workspace_t*graphcut_workspace_new(graph_t*graph, node_t*pos1, node_t*pos2)
{
    graphcut_workspace_t*workspace = malloc(sizeof(graphcut_workspace_t));
    workspace->flags1 = rfx_calloc(graph->num_nodes);
    workspace->flags2 = rfx_calloc(graph->num_nodes);
    workspace->back = rfx_calloc(graph->num_nodes*sizeof(halfedge_t*));
    workspace->pos1 = pos1;
    workspace->pos2 = pos2;
    workspace->graph = graph;
    workspace->queue1 = posqueue_new();
    workspace->queue2 = posqueue_new();
    workspace->tmpqueue = posqueue_new();
    return workspace;
}
static void graphcut_workspace_delete(graphcut_workspace_t*w) 
{
    posqueue_delete(w->queue1);w->queue1=0;
    posqueue_delete(w->queue2);w->queue2=0;
    posqueue_delete(w->tmpqueue);w->tmpqueue=0;
    if(w->flags1) free(w->flags1);w->flags1=0;
    if(w->flags2) free(w->flags2);w->flags2=0;
    if(w->back) free(w->back);w->back=0;
    free(w);
}

typedef struct _path {
    node_t**pos;
    halfedge_t**dir;
    unsigned char*firsthalf;
    int length;
} path_t;

static path_t*path_new(int len)
{
    path_t*p = malloc(sizeof(path_t));
    p->pos = malloc(sizeof(node_t*)*len);
    p->dir = malloc(sizeof(halfedge_t*)*len);
    p->firsthalf = malloc(sizeof(unsigned char)*len);
    p->length = len;
    return p;
}
static void path_delete(path_t*path)
{
    free(path->pos);path->pos = 0;
    free(path->dir);path->dir = 0;
    free(path->firsthalf);path->firsthalf = 0;
    free(path);
}

static path_t*extract_path(graphcut_workspace_t*work, unsigned char*mytree, unsigned char*othertree, node_t*pos, node_t*newpos, halfedge_t*dir)
{
    int t;
    node_t*p = pos;
    node_t*nodes = work->graph->nodes;
    int len1 = 0;
    /* walk up tree1 */
    DBG printf("walk back up (1) to %d\n", NR(work->pos1));
    while(p != work->pos1) {
	halfedge_t*back = work->back[NR(p)];
	DBG printf("walk backward (1): %d %d\n", NR(p), back?NR(back->fwd->node):-1);
	node_t*old = p;
	p = work->back[NR(p)]->fwd->node;
	assert(p!=old);
	len1++;
    }
    p = newpos;
    int len2 = 0;
    DBG printf("walk back up (2) to %d\n", NR(work->pos2));
    /* walk up tree2 */
    while(p != work->pos2) {
	DBG printf("walk backward (2): %d\n", NR(p));
	p = work->back[NR(p)]->fwd->node;
	len2++;
    }
    path_t*path = path_new(len1+len2+2);

    t = len1;
    path->pos[t] = p = pos;
    path->dir[t] = dir;
    path->firsthalf[t] = 1;
    while(p != work->pos1) {
	assert(mytree[NR(p)]&IN_TREE);
	halfedge_t*dir = work->back[NR(p)];
	assert(dir->node == p);
	p = dir->fwd->node;
	t--;
	path->pos[t] = p;
	path->dir[t] = dir->fwd;
	path->firsthalf[t] = 1;
    }
    assert(!t);

    t = len1+1;

    p = newpos;
    while(p != work->pos2) {
	assert(othertree[NR(p)]&IN_TREE);
	halfedge_t*dir = work->back[NR(p)];
	path->pos[t] = p;
	path->dir[t] = dir;
	path->firsthalf[t] = 0;
	p = dir->fwd->node;
	t++;
    }

    /* terminator */
    path->pos[t] = p;
    path->dir[t] = 0; // last node
    path->firsthalf[t] = 0;

    assert(t == len1+len2+1);
    return path;
}

static void path_print(path_t*path)
{
    int t;
    for(t=0;t<path->length;t++) {
	node_t*n = path->pos[t];
	printf("%d (firsthalf: %d)", NR(n), path->firsthalf[t]);
	if(t<path->length-1) {
	    printf(" -(%d/%d)-> \n", 
		    path->dir[t]->used,
		    path->dir[t]->fwd->used);
	} else {
	    printf("\n");
	}
    }

    for(t=0;t<path->length-1;t++) {
	if(path->firsthalf[t]==path->firsthalf[t+1]) {
	    assert(( path->firsthalf[t] && path->dir[t]->used) || 
		   (!path->firsthalf[t] && path->dir[t]->fwd->used));
	}
    }
    printf("\n");
}


static void workspace_print(graphcut_workspace_t*w)
{
    printf("queue1: ");posqueue_print(w, w->queue1);
    printf("queue2: ");posqueue_print(w, w->queue2);
}

static void myassert(graphcut_workspace_t*w, char assertion, const char*file, int line, const char*func)
{
    if(!assertion) {
	printf("Assertion %s:%d (%s) failed:\n", file, line, func);
	workspace_print(w);
	exit(0);
    }
}

#define ASSERT(w,c) {myassert(w,c,__FILE__,__LINE__,__func__);}

static path_t* expand_pos(graphcut_workspace_t*w, posqueue_t*queue, node_t*pos, char reverse, unsigned char*mytree, unsigned char*othertree)
{
    graph_t*graph = w->graph;
    int dir;
    if((mytree[NR(pos)]&(IN_TREE|ACTIVE)) != (IN_TREE|ACTIVE)) {
	/* this node got deleted or marked inactive in the meantime. ignore it */
	DBG printf("node %d is deleted or inactive\n", NR(pos));
	return 0;
    }

    halfedge_t*e = pos->edges;
    for(;e;e=e->next) {
	node_t*newpos = e->fwd->node;
	weight_t weight = reverse?e->fwd->weight:e->weight;
	if(mytree[NR(newpos)]) continue; // already known

	if(weight) {
	    if(othertree[NR(newpos)]) {
		DBG printf("found connection: %d connects to %d\n", NR(pos), NR(newpos));
                posqueue_addpos(queue, pos); mytree[NR(pos)] |= ACTIVE; // re-add, this vertex might have other connections

		path_t*path;
		if(reverse) {
		    path = extract_path(w, othertree, mytree, newpos, pos, e->fwd);
		} else {
		    path = extract_path(w, mytree, othertree, pos, newpos, e);
		}
		return path;
	    } else {
		DBG printf("advance from %d to new pos %d\n", NR(pos), NR(newpos));
		w->back[NR(newpos)] = e->fwd;
		e->used = 1;
		posqueue_addpos(queue, newpos); mytree[NR(newpos)] |= ACTIVE|IN_TREE; // add
	    }
	}
    }
    /* if we can't expand this node anymore, it's now an inactive node */
    mytree[NR(pos)] &= ~ACTIVE;
    return 0;
}

static int node_count_edges(node_t*node)
{
    halfedge_t*e = node->edges;
    int num = 0;
    while(e) {
	num++;
	e = e->next;
    }
    return num;
}

static void bool_op(graphcut_workspace_t*w, unsigned char*flags, node_t*pos, unsigned char and, unsigned char or)
{
    posqueue_t*q = w->tmpqueue;
    posqueue_purge(q);
    posqueue_addpos(q, pos);

    while(posqueue_notempty(q)) {
        node_t*p = posqueue_extract(q);
        flags[NR(p)] = (flags[NR(p)]&and)|or;
	halfedge_t*e = p->edges;
	while(e) {
            if(e->used) {
                posqueue_addpos(q, e->fwd->node);
	    }
	    e = e->next;
	}
    }
}

static weight_t decrease_weights(graph_t*map, path_t*path)
{
    int t;
    assert(path->length);

    weight_t min = path->dir[0]->weight;
    for(t=0;t<path->length-1;t++) {
	int w = path->dir[t]->weight;
	DBG printf("%d->%d (%d)\n", NR(path->dir[t]->node), NR(path->dir[t]->fwd->node), w);
	if(t==0 || w < min) min = w;
    }
    assert(min);
    if(min<=0) 
        return 0;

    for(t=0;t<path->length-1;t++) {
	path->dir[t]->weight-=min;
	path->dir[t]->fwd->weight+=min;
    }
    return min;
}

static int reconnect(graphcut_workspace_t*w, unsigned char*flags, node_t*pos, char reverse)
{
    graph_t*graph = w->graph;

    halfedge_t*e = pos->edges;
    for(;e;e=e->next) {
        node_t*newpos = e->fwd->node;
        int weight;
        if(!reverse) {
	    weight = e->fwd->weight;
	} else {
	    weight = e->weight;
	}
        if(weight && (flags[NR(newpos)]&IN_TREE)) {
            DBG printf("successfully reconnected node %d to %d (%d->%d) (reverse:%d)\n", 
                    NR(pos), NR(newpos), NR(e->node), NR(e->fwd->node), reverse);

	    w->back[NR(pos)] = e;
	    e->fwd->used = 1;
            return 1;
        }
    }
    return 0;
}

static void clear_node(graphcut_workspace_t*w, node_t*n)
{
    w->flags1[NR(n)] = 0;
    w->flags2[NR(n)] = 0;
    w->back[NR(n)] = 0;
    halfedge_t*e = n->edges;
    while(e) {e->used = 0;e=e->next;}
}

static void destroy_subtree(graphcut_workspace_t*w, unsigned char*flags, node_t*pos, posqueue_t*posqueue)
{
    DBG printf("destroying subtree starting with %d\n", NR(pos));

    posqueue_t*q = w->tmpqueue;
    posqueue_purge(q);
    posqueue_addpos(q, pos);

    while(posqueue_notempty(q)) {
        node_t*p = posqueue_extract(q);
	halfedge_t*e = p->edges;
	while(e) {
	    node_t*newpos = e->fwd->node;	
            if(e->used) {
                posqueue_addpos(q, newpos);
            } else if((flags[NR(newpos)]&(ACTIVE|IN_TREE)) == IN_TREE) {
		// re-activate all nodes that surround our subtree.
                // TODO: we should check the weight of the edge from that other
		// node to our node. if it's zero, we don't need to activate that node.
                posqueue_addpos(posqueue, newpos);
                flags[NR(newpos)]|=ACTIVE;
            }
	    e = e->next;
        }
       
	clear_node(w, p);
	DBG printf("removed pos %d\n", NR(p));
    }
}

static void combust_tree(graphcut_workspace_t*w, posqueue_t*q1, posqueue_t*q2, path_t*path)
{
    graph_t*graph = w->graph;
    int t;
    for(t=0;t<path->length-1 && path->firsthalf[t+1];t++) {
        node_t*pos = path->pos[t];
	halfedge_t*dir = path->dir[t];
        node_t*newpos = dir->fwd->node;
        if(!dir->weight) {
            /* disconnect node */
            DBG printf("remove link %d -> %d from tree 1\n", NR(pos), NR(newpos));
	   
	    dir->used = 0;
            w->flags1[NR(newpos)] &= ACTIVE;
            bool_op(w, w->flags1, newpos, ~IN_TREE, 0);

            /* try to reconnect the path to some other tree part */
            if(reconnect(w, w->flags1, newpos, 0)) {
                bool_op(w, w->flags1, newpos, ~0, IN_TREE);
            } else {
                destroy_subtree(w, w->flags1, newpos, q1);
                break;
            }
        }
    }

    for(t=path->length-1;t>0 && !path->firsthalf[t-1];t--) {
        node_t*pos = path->pos[t];
        node_t*newpos = path->pos[t-1];
        halfedge_t*dir = path->dir[t-1]->fwd;
        node_t*newpos2 = dir->fwd->node;
        assert(newpos == newpos2);
        if(!dir->fwd->weight) {
            /* disconnect node */
            DBG printf("remove link %d->%d from tree 2\n", NR(pos), NR(newpos));

	    dir->used = 0;
            w->flags2[NR(newpos)] &= ACTIVE;
            bool_op(w, w->flags2, newpos, ~IN_TREE, 0);

            /* try to reconnect the path to some other tree part */
            if(reconnect(w, w->flags2, newpos, 1)) {
                bool_op(w, w->flags2, newpos, ~0, IN_TREE);
            } else {
                destroy_subtree(w, w->flags2, newpos, q2);
                break;
            }
        }
    }
}

static void check_graph(graph_t*g)
{
    int t;
    for(t=0;t<g->num_nodes;t++) {
	assert(g->nodes[t].nr==t);
	halfedge_t*e = g->nodes[t].edges;
	while(e) {
	    assert(!e->used || !e->fwd->used);
	    e = e->next;
	}
    }
}

void graph_reset(graph_t*g)
{
    int t;
    for(t=0;t<g->num_nodes;t++) {
	g->nodes[t].nr = t;
	assert(g->nodes[t].nr==t);
	halfedge_t*e = g->nodes[t].edges;
	while(e) {
	    e->used = 0;
	    e->weight = e->init_weight;
	    e = e->next;
	}
    }
}

weight_t graph_maxflow(graph_t*graph, node_t*pos1, node_t*pos2)
{
    int max_flow = 0;
    graphcut_workspace_t* w = graphcut_workspace_new(graph, pos1, pos2);

    graph_reset(graph);
    DBG check_graph(graph);
   
    posqueue_addpos(w->queue1, pos1); w->flags1[pos1->nr] |= ACTIVE|IN_TREE; 
    posqueue_addpos(w->queue2, pos2); w->flags2[pos2->nr] |= ACTIVE|IN_TREE; 
    DBG workspace_print(w);
  
    while(1) {
	path_t*path;
	while(1) {
	    char done1=0,done2=0;
	    node_t* p1 = posqueue_extract(w->queue1);
	    if(!p1) {
		graphcut_workspace_delete(w);
		return max_flow;
	    }
	    DBG printf("extend 1 from %d (%d edges)\n", NR(p1), node_count_edges(p1));
	    path = expand_pos(w, w->queue1, p1, 0, w->flags1, w->flags2);
	    if(path)
		break;
	    DBG workspace_print(w);
	   
#ifdef TWOTREES
	    node_t* p2 = posqueue_extract(w->queue2);
	    if(!p2) {
		graphcut_workspace_delete(w);
		return max_flow;
	    }
	    DBG printf("extend 2 from %d (%d edges)\n", NR(p2), node_count_edges(p2));
	    path = expand_pos(w, w->queue2, p2, 1, w->flags2, w->flags1);
	    if(path)
		break;
	    DBG workspace_print(w);
#endif

	}
	DBG printf("found connection between tree1 and tree2\n");
	DBG path_print(path);

	DBG printf("decreasing weights\n");
	max_flow += decrease_weights(graph, path);
	DBG workspace_print(w);

	DBG printf("destroying trees\n");
	combust_tree(w, w->queue1, w->queue2, path);
	DBG workspace_print(w);

	DBG check_graph(w->graph);

	path_delete(path);
    }
    graphcut_workspace_delete(w);
    return max_flow;
}

halfedge_t*graph_add_edge(node_t*from, node_t*to, weight_t forward_weight, weight_t backward_weight)
{
    halfedge_t*e1 = (halfedge_t*)rfx_calloc(sizeof(halfedge_t));
    halfedge_t*e2 = (halfedge_t*)rfx_calloc(sizeof(halfedge_t));
    e1->fwd = e2;
    e2->fwd = e1;
    e1->node = from;
    e2->node = to;
    e1->init_weight = forward_weight;
    e2->init_weight = backward_weight;
    e1->weight = forward_weight;
    e2->weight = backward_weight;

    e1->next = from->edges;
    from->edges = e1;
    e2->next = to->edges;
    to->edges = e2;
    return e1;
}

static void do_dfs(node_t*n, int color)
{
    int t;
    n->tmp = color;
    halfedge_t*e = n->edges;
    while(e) {
	if(e->fwd->node->tmp<0)
	    do_dfs(e->fwd->node, color);
	e = e->next;
    }
}

int graph_find_components(graph_t*g)
{
    int t;
    int count = 0;
    for(t=0;t<g->num_nodes;t++) {
	g->nodes[t].tmp = -1;
    }
    for(t=0;t<g->num_nodes;t++) {
	if(g->nodes[t].tmp<0) {
	    do_dfs(&g->nodes[t], count++);
	}
    }
    return count;
}

#ifdef MAIN
int main()
{
    int t;
    int s;
    for(s=0;s<10;s++) {
	int width = (lrand48()%8)+1;
	graph_t*g = graph_new(width*width);
	for(t=0;t<width*width;t++) {
	    int x = t%width;
	    int y = t/width;
	    int w = 1;
#define R (lrand48()%32)
	    if(x>0) graph_add_edge(&g->nodes[t], &g->nodes[t-1], R, R);
	    if(x<width-1) graph_add_edge(&g->nodes[t], &g->nodes[t+1], R, R);
	    if(y>0) graph_add_edge(&g->nodes[t], &g->nodes[t-width], R, R);
	    if(y<width-1) graph_add_edge(&g->nodes[t], &g->nodes[t+width], R, R);
	}
	
	int x = graph_maxflow(g, &g->nodes[0], &g->nodes[width*width-1]);
	printf("max flow: %d\n", x);
	graph_delete(g);
    }
}
#endif
