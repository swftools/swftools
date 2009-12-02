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
#ifndef __graphcut_h__
#define __graphcut_h__

typedef signed int weight_t;

typedef struct _halfedge halfedge_t;
typedef struct _node node_t;
typedef struct _graph graph_t;

struct _halfedge {
    node_t*node;
    struct _halfedge*fwd;
    weight_t weight;
    weight_t init_weight;
    char used;
    halfedge_t*next;
};

struct _node {
    halfedge_t*edges;
    union {
	int tmp;
	int component;
	int color;
    };
    int nr;
};

struct _graph {
    node_t* nodes;
    int num_nodes;
};

graph_t* graph_new(int num_nodes);
halfedge_t*graph_add_edge(node_t*from, node_t*to, weight_t forward_weight, weight_t backward_weight);
weight_t graph_maxflow(graph_t*graph, node_t*pos1, node_t*pos2);
int graph_find_components(graph_t*g);
void graph_delete(graph_t*);

#endif
