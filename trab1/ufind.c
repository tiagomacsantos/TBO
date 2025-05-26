#include "ufind.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ufind_t {
    int *parent;
    int *size;
    int count;
};

ufind_t *uf_create(int n) {
    ufind_t *uf = malloc(sizeof(ufind_t));
    uf->parent = malloc(n * sizeof(int));
    uf->size = malloc(n * sizeof(int));
    uf->count = n;
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
        uf->size[i] = 1;
    }
    return uf;
}

int uf_find(ufind_t *uf, int p) {
    int root = p;
    // Find the root
    while (uf->parent[root] != root) {
        root = uf->parent[root];
    }
    // Path compression: make every node on the path point to the root
    int curr = p;
    while (uf->parent[curr] != root) {
        int next = uf->parent[curr];
        uf->parent[curr] = root;
        curr = next;
    }
    return root;
}

void uf_union(ufind_t *uf, int p, int q) {
    int rootP = uf_find(uf, p);
    int rootQ = uf_find(uf, q);
    if (rootP == rootQ) return;
    // Weighted union
    if (uf->size[rootP] < uf->size[rootQ]) {
        uf->parent[rootP] = rootQ;
        uf->size[rootQ] += uf->size[rootP];
    } else {
        uf->parent[rootQ] = rootP;
        uf->size[rootP] += uf->size[rootQ];
    }
    uf->count--;
}

int uf_connected(ufind_t *uf, int p, int q) {
    return uf_find(uf, p) == uf_find(uf, q);
}

void uf_destroy(ufind_t *uf) {
    free(uf->parent);
    free(uf->size);
    free(uf);
}