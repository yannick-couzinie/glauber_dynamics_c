#include <stdlib.h> // malloc

#include "weightedgraph.h"

/**
 * Basic undirected graph struct implementation with edge weights using
 * adjacency lists represented as  variable length arrays.
 *
 * The arrays are not sorted!
 **/

graph *graph_new(){
        graph *g = malloc(sizeof(graph));
        *g = (graph) {.n=0, .m=0};
        return g;
}

void graph_free(graph *g){
        for(int i=0; i<g->n; i++){
                /**
                 * use the fact that vertex_free frees the memory location for all
                 * edges contained in it
                 **/
                vertex_free(g->vertices[i]);
        }
        free(g);
}

void graph_add_n_vertices(graph *g, int to_add){
        /* update g->n only at the end and use that it contains the old value */
        g->vertices = realloc(g->vertices, sizeof(g->vertices) + to_add*sizeof(vertex));
        for (int i=g->n; i<to_add; i++){
                g->vertices[i] = vertex_new();
        }
        g->n += to_add;
}

void graph_add_edge(graph *g, int v1, int v2, int weight){
        /* check first that the connection does not exist */
        for (int i=0; i<g->vertices[v1]->dim; i++){
                if (g->vertices[v1]->edges[i]->v1 == v2 ||
                    g->vertices[v1]->edges[i]->v2 == v2){
                        return;
                }

        /* update g->m only at the end and use that it contains the old value */
        g->edges = realloc(g->edges, sizeof(g->edges) + sizeof(edge));
        edge *new_edge = edge_new(v1, v2, weight);
        g->edges[g->m] = new_edge;

        vertex_add_edge_to_neighbourhood(g->vertices[v1], new_edge);
        vertex_add_edge_to_neighbourhood(g->vertices[v2], new_edge);
        g->m += 1;
        }
}

/**
 * utility function to find edge connecting v to vertex with label dst
 *
 * Returns NULL pointer if no edge present.
 **/
edge static *find_connecting_edge(vertex *v, int dst){
        for (int i=0; i<v->dim; i++){
                if (v->edges[i]->v1 == dst ||
                    v->edges[i]->v2 == dst){
                        return v->edges[i];
                }
        }
        return NULL;
}

void graph_rm_edge(graph *g, int v1, int v2){
        /**
         * Since we need the correct memory location we cannot just create a
         * new edge instance, but need to loop through the edges and find the
         * actual edge.
         **/
        edge *connecting_edge = find_connecting_edge(g->vertices[v1], v2); 
        if (connecting_edge){
                /* only if the connecting edge is not NULL */
                vertex_rm_edge_from_neighbourhood(g->vertices[v1], connecting_edge);
                vertex_rm_edge_from_neighbourhood(g->vertices[v2], connecting_edge);
        }
}
