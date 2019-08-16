#include <stdio.h>
#include "vertex.h"

/* define the graph structure */

typedef struct graph {
        int n;                  /* number of vertices */
        int m;                  /* number of edges */
        vertex **vertices;
        edge **edges;
} graph;

/* create a new graph */
graph *graph_new();

/* free all space used by graph
 * 
 *  NOTE: Make sure that nothing is malloced while this is
 *  running, since every edge memory location will be freed
 *  twice (once for every end-point) and hence if something is
 *  malloced on the already freed space it would be freed
 *  again.
 **/
void graph_free(graph *g);

/* add n new vertices to the graph labelled 0..n-1*/
void graph_add_n_vertices(graph *g, int n);

/**
 * add an edge to an existing graph between two vertices if the connection does
 * not already exist.
 **/
void graph_add_edge(graph *g, int v1, int v2, int weight);

/* remove an edge between two vertices if it exists */
void graph_rm_edge(graph *g, int v1, int v2);

/* return the edge connecting vertex v to vertex of index dst */
edge *graph_find_connecting_edge(vertex *v, int dst);

/* construct a d-dimensional corresponding to \Z^d \setminus n\Z */
graph *graph_construct_torus(int n, int d, int init_weight);

/* draw the torus */
void draw_torus2png(graph *draw_torus, int n, int d, unsigned int duration,
                    int max_width, int max_height, int max_dpi, FILE *out_stream);
