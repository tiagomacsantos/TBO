#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ponto.h"

struct ponto_t {
    char *id; 
    double *coords; // array de coordenadas
    int dimensao;
};

ponto_t *ponto_create(char *id, double *coords, int dimensao) {
    ponto_t *p = malloc(sizeof(ponto_t));
    if (p == NULL) {
        return NULL;
    }
    p->id = strdup(id);
    p->coords = malloc(dimensao * sizeof(double)); // Aloca a memoria das coords
    memcpy(p->coords, coords, dimensao * sizeof(double)); // copia os valores
    p->dimensao = dimensao;
    return p;
}

char *ponto_get_id(ponto_t *p) {
    return p->id;
}

double *ponto_get_coords(ponto_t *p) {
    return p->coords;
}

ponto_t **ler_pontos(FILE *in, int *n, int *m) {
    char *buffer = NULL;
    size_t len = 0;

    getline(&buffer, &len, in);  // le a primeira linha

    *m = 0;
    char *tmp = strdup(buffer); // primeira linha
    char *token = strtok(tmp, ",");
    while ((token = strtok(NULL, ",")) != NULL) { // conta a dimensao dos pontos
        (*m)++;
    }
    free(tmp);

    int capacidade = 10;
    ponto_t **pontos = malloc(capacidade * sizeof(ponto_t *));
    *n = 0;

    char *id = NULL;
    double *coords = malloc((*m) * sizeof(double));

    // isso e pro primeiro ponto, que esta em "buffer"
    token = strtok(buffer, ",");
    id = strdup(token);
    for (int i = 0; i < *m; i++) {
        token = strtok(NULL, ",");
        coords[i] = atof(token);
    }
    ponto_t *p = ponto_create(id, coords, *m);
    pontos[*n] = p;
    (*n)++;

    // le o resto
    while (getline(&buffer, &len, in) != -1) {
        if (*n >= capacidade) {
            capacidade *= 2;
            pontos = realloc(pontos, capacidade * sizeof(ponto_t *));
        }

        if (id) free(id);
        token = strtok(buffer, ",");
        id = strdup(token);

        for (int i = 0; i < *m; i++) {
            token = strtok(NULL, ",");
            coords[i] = atof(token);
        }

        p = ponto_create(id, coords, *m);
        pontos[*n] = p;
        (*n)++;
    }

    free(id);
    free(coords);
    free(buffer);
    return pontos;
}

void ponto_destroy(ponto_t *p) {
    free(p->id);
    free(p->coords);
    free(p);
}


