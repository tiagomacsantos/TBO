#ifndef _UFIND_H_
#define _UFIND_H_

typedef struct ufind_t ufind_t;

ufind_t *uf_create(int n);
void uf_destroy(ufind_t *uf);
int uf_find(ufind_t *uf, int x);
void uf_union(ufind_t *uf, int x, int y);
int uf_connected(ufind_t *uf, int x, int y);
int uf_size(ufind_t *uf, int x);

#endif