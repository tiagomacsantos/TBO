#ifndef _ARESTA_H_  
#define _ARESTA_H_

#include "ponto.h"

typedef struct aresta_t aresta_t;

aresta_t *aresta_create(int from, int to, double dist);
int aresta_get_from(aresta_t *a);
int aresta_get_to(aresta_t *a);
double aresta_get_dist(aresta_t *a);
aresta_t **gerar_arestas(ponto_t **pontos, int n, int m, int *num_arestas);
int comparar_arestas_desc(const void *a, const void *b);
int comparar_arestas(const void *a, const void *b);
void aresta_destroy(aresta_t *a);

#endif