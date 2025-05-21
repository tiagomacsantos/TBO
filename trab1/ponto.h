#ifndef _PONTO_H_
#define _PONTO_H_

typedef struct ponto_t ponto_t;

ponto_t *ponto_create(char *id, double *coords, int dimensao);
char *ponto_get_id(ponto_t *p);
double *ponto_get_coords(ponto_t *p);
void ponto_destroy(ponto_t *p);
ponto_t **ler_pontos(FILE *in, int *n, int *m);

#endif