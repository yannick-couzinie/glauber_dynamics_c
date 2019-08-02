#include <assert.h>
#include <stdlib.h> // malloc

#include "vertex.h"

vertex *vertex_new(){
        vertex *out = malloc(sizeof(vertex));
        *out = (vertex){.dim=0,
                        .local_weight=0}; // initalize with no standard value for v->edges
        return out;
}

void vertex_free(vertex *v){
        for(int i=0; i<v->dim; i++){
                edge_free(v->edges[i]);
        }
        free(v);
}

void vertex_add_edge_to_neighbourhood(vertex *v, edge *e){
        /* check that we are not trying to add a self-edge */
        assert(e->v1 != e->v2);        

        /* check that the edge is not already contained */
        for(int i=0; i<v->dim; i++){
                if (v->edges[i] == e){
                        return;
                }
        }
        v->dim++;
        v->local_weight += e->weight;
        v->edges = realloc(v->edges, sizeof(edge*)*v->dim);
        v->edges[v->dim-1] = e;
}

/**
 * make a new edges for the incoming vertex removing the i-th edge
 * 
 * Note that this is a pure utility function (hence static, i.e. limited to the
 * file) for vertex_rm_edge_from_neighbourhood.
 **/
edge static **rm_i_th_edge(vertex const *v, int to_remove){
        edge **out;
        out = malloc(sizeof(edge*)*(v->dim-1));
        for (int i=0; i<v->dim-1; i++){
                /* skip the i-th index in assigning */
                if (i<to_remove) { out[i] = v->edges[i]; }
                else if (i>=to_remove) { out[i] = v->edges[i+1]; }
        }
        return out;
}

void vertex_rm_edge_from_neighbourhood(vertex *v, edge *e){
        for(int i=0; i<v->dim; i++){
                /* compare memory adresses */
                if(v->edges[i] == e){
                        v->edges = rm_i_th_edge(v, i);
                        v->local_weight -= e->weight;
                        return;
                }
        }
}
