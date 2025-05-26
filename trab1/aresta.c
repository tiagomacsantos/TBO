#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "aresta.h"

struct aresta_t {
    int from;
    int to;
    double dist;
};

aresta_t *aresta_create(int from, int to, double dist) {
    aresta_t *a = malloc(sizeof(aresta_t));
    if (a == NULL) {
        return NULL;
    }
    a->from = from;
    a->to = to;
    a->dist = dist;
    return a;
}

int aresta_get_from(aresta_t *a) {
    return a->from;
}

int aresta_get_to(aresta_t *a) {
    return a->to;
}

double aresta_get_dist(aresta_t *a) {
    return a->dist;
}

aresta_t **gerar_arestas(ponto_t **pontos, int n, int m, int *num_arestas) {
    *num_arestas = 0;
    int capacidade = n * (n - 1) / 2; // max de arestas sem repetir
    aresta_t **arestas = malloc(capacidade * sizeof(aresta_t *));
    
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = 0;
            for (int k = 0; k < m; k++) {
                double diff = ponto_get_coords(pontos[i])[k] - ponto_get_coords(pontos[j])[k];
                dist += diff * diff;
            }
            dist = sqrt(dist);
            arestas[*num_arestas] = aresta_create(i, j, dist);
            (*num_arestas)++;
        }
    }
    
    return arestas;
}

//func pra usar com o qsort
int comparar_arestas_desc(const void *a, const void *b) {
    aresta_t *e1 = *(aresta_t **)a;
    aresta_t *e2 = *(aresta_t **)b;
    double d1 = aresta_get_dist(e1);
    double d2 = aresta_get_dist(e2);

    if (d1 < d2) return 1;
    if (d1 > d2) return -1;
    return 0;
}

//func pra usar com qsort
int comparar_arestas(const void *a, const void *b) {
    aresta_t *e1 = *(aresta_t **)a;
    aresta_t *e2 = *(aresta_t **)b;

    double d1 = aresta_get_dist(e1);
    double d2 = aresta_get_dist(e2);

    if (d1 < d2) return -1;
    if (d1 > d2) return 1;
    return 0;
}

void aresta_destroy(aresta_t *a) {
    free(a);
}