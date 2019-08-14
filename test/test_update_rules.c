#include <glib.h>
#include <math.h>

#include "update_rules.h"

#undef ALPHA /* reset ALPHA to a fixred value for testing */
#define ALPHA 0.25

typedef struct{
        graph *g;
        pcg32_random_t rng;
        double first_elements[10]; /* first ten elements spawned by rng unif on [0,1)
                                      (using ldexp(random number, -32) as per the docs of pcg)*/
} ufixture;

void update_rule_setup(ufixture *uf, gconstpointer test_data){
        /* for testing take the
         *   |   |   |
         * - 0 - 1 - 2 - 
         *   |   |   |
         * - 3 - 4 - 5 - 
         *   |   |   |
         * - 6 - 7 - 8 -
         *   |   |   |
         * torus.*/
        uf->g = graph_construct_torus(3, 2, 1);

        pcg32_srandom_r(&uf->rng, (uint64_t) 1, (uint64_t) 1);
        /* first ten results of ldexp(pcg32_random_r(&uf->rng), 32) i.e.
         * uniform on [0,1) variable generation */
        memcpy(uf->first_elements, (double [10]){0.787148, 0.084273,
                                                 0.750582, 0.647502,
                                                 0.685563, 0.422305,
                                                 0.187791, 0.612337,
                                                 0.817260, 0.564490},
               10*sizeof(double));
}

void update_rule_teardown(ufixture *uf, gconstpointer test_data){
        graph_free(uf->g);
}

/* check 4 updates */
void test_polya_update(ufixture *uf, gconstpointer ignored){
        polya_update(uf->g, 4, &uf->rng);
        /* for the first element in first_uniform_elements the incremented edge
         * should be the fourth in the edges list (since
         * 0.75<first_unifrom_elements[0]<1*/

        edge *edge0 = uf->g->vertices[4]->edges[0];
        edge *edge1 = uf->g->vertices[4]->edges[1];
        edge *edge2 = uf->g->vertices[4]->edges[2];
        edge *edge3 = uf->g->vertices[4]->edges[3];

        g_assert_cmpint(edge0->weight, ==, 1);
        g_assert_cmpint(edge1->weight, ==, 1);
        g_assert_cmpint(edge2->weight, ==, 1);
        g_assert_cmpint(edge3->weight, ==, 2);

        /* for the second update it should be the first one that is updated */
        polya_update(uf->g, 4, &uf->rng);
        g_assert_cmpint(edge0->weight, ==, 2);
        g_assert_cmpint(edge1->weight, ==, 1);
        g_assert_cmpint(edge2->weight, ==, 1);
        g_assert_cmpint(edge3->weight, ==, 2);

        /* for the third update it should be the third one that is updated */
        polya_update(uf->g, 4, &uf->rng);
        g_assert_cmpint(edge0->weight, ==, 2);
        g_assert_cmpint(edge1->weight, ==, 1);
        g_assert_cmpint(edge2->weight, ==, 1);
        g_assert_cmpint(edge3->weight, ==, 3);

        /* finallly check that all the edges have been updated accordingly */
        for(int i=0; i<uf->g->m; i++){
                if (uf->g->edges[i] == edge0){
                        g_assert_cmpint(uf->g->edges[i]->weight, ==, 2);
                }
                else if (uf->g->edges[i] == edge3){
                        g_assert_cmpint(uf->g->edges[i]->weight, ==, 3);
                }
                else{
                        g_assert_cmpint(uf->g->edges[i]->weight, ==, 1);
                }
        }
}

/* test that giving n too large fails the code */
void test_polya_invalid_vertex(ufixture *uf, gconstpointer ignored){
         if (g_test_subprocess()){
                 polya_update(uf->g, 9, &uf->rng);
         }
         g_test_trap_subprocess(NULL, 0, 0);
         g_test_trap_assert_failed();
}

int main(int argc, char **argv){
        g_test_init(&argc, &argv, NULL);
        /* Tests for polya_update */
        g_test_add("/polya/test polya updates", ufixture, NULL,
                   update_rule_setup, test_polya_update, update_rule_teardown);

        g_test_add("/polya/test polya invalid vertex", ufixture, NULL,
                   update_rule_setup, test_polya_invalid_vertex, update_rule_teardown);
        return g_test_run();
}
