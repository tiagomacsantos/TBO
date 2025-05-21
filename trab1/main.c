#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ponto.h"
#include "aresta.h"
#include "ufind.h"

int cmp_str(const void *a, const void *b) {
    return strcmp(*(char **)a, *(char **)b);
}

typedef struct {
    char **ids;     // vetor de strings (nomes dos pontos)
    int tamanho;    // quantos nomes já tem
    int capacidade; // capacidade atual do vetor
} cluster_t;

void cluster_destroy(cluster_t *clusters_array, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        for (int j = 0; j < clusters_array[i].tamanho; j++) {
            free(clusters_array[i].ids[j]);
        }
        free(clusters_array[i].ids);
    }
    free(clusters_array);
}

int cmp_clusters(const void *a, const void *b) {
    cluster_t *c1 = (cluster_t *)a;
    cluster_t *c2 = (cluster_t *)b;
    return strcmp(c1->ids[0], c2->ids[0]);
}

int main(int argc, char *argv[]) {

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[3], "w");

    int k = atoi(argv[2]);

    // PONTOS
    ponto_t **pontos = NULL;
    int n = 0, m = 0;
    pontos = ler_pontos(in, &n, &m); // le todos os pontos do arquivo de entrada e salva para calcular as arestas

    // ARESTAS
    aresta_t **arestas = NULL;
    int num_arestas = 0;
    arestas = gerar_arestas(pontos, n, m, &num_arestas); // gera as arestas entre os pontos (calculo das distancias)
    qsort(arestas, num_arestas, sizeof(aresta_t *), comparar_arestas); // ordena as arestas pela distancia (crescente)

    // AQUI VAI O ALGORITMO DA MST DO KRUSKAL
    ufind_t *uf = uf_create(n); // cria a estrutura de dados para o algoritmo de Kruskal
    aresta_t **mst = malloc((n-1) * sizeof(aresta_t*)); // inicializa o vetor de arestas da MST
    int count = 0; // idx da aresta atual

    for(int i = 0; i < num_arestas && count < n - 1; i++) { //importante a condicao count < n - 1 pra ele nao precisar passar por todas as arestas
        int from = aresta_get_from(arestas[i]);
        int to = aresta_get_to(arestas[i]);
        double dist = aresta_get_dist(arestas[i]);

        if(!uf_connected(uf, from, to)) { // se os pontos nao estao na mesma componente
            uf_union(uf, from, to); // une os dois pontos
            mst[count] = arestas[i]; // adiciona a aresta na MST
            count++;
        }
    }

    qsort(mst, n-1, sizeof(aresta_t *), comparar_arestas_desc); // ordena as arestas da MST pela distancia (decrescente)

    ufind_t *uf_clusters = uf_create(n);
    for (int i = k - 1; i < n - 1; i++) {
        int u = aresta_get_from(mst[i]);      // AGRUPANDO OS K-1 MENORES
        int v = aresta_get_to(mst[i]);
        uf_union(uf_clusters, u, v);
    }

    int clusters[n];
    for (int i = 0; i < n; i++) {
        clusters[i] = uf_find(uf_clusters, i); // mapeia os pontos para os clusters
    }

    int raizes[n];
    int num_clusters = 0;

    // vendo quais sao os clusters unicos 
    for(int i = 0; i < n; i++) {
        int raiz = clusters[i];
        int existe = 0;
        for(int j = 0; j < num_clusters; j++) {
            if(raizes[j] == raiz) {
                existe = 1; 
                break; 
            }
        }
        if(!existe) {
            raizes[num_clusters] = raiz;
            num_clusters++;
        }
    }

    // alocando os clusters
    cluster_t *clusters_array = malloc(num_clusters * sizeof(cluster_t));
    for(int i = 0; i < num_clusters; i++) {
        clusters_array[i].ids = malloc(10 * sizeof(char *));
        clusters_array[i].tamanho = 0;
        clusters_array[i].capacidade = 10;
    }

    // preenchendo os clusters
    for(int i = 0; i < n; i++) {
        int raiz = clusters[i];
        int idx = -1;
        for(int j = 0; j < num_clusters; j++) {
            if(raizes[j] == raiz) {
                idx = j;
                break;
            }
        }
        // garantir capacidade
        if (idx != -1) {
            if (clusters_array[idx].tamanho == clusters_array[idx].capacidade) {
                clusters_array[idx].capacidade *= 2;
                clusters_array[idx].ids = realloc(clusters_array[idx].ids, clusters_array[idx].capacidade * sizeof(char *));
            }

            clusters_array[idx].ids[clusters_array[idx].tamanho++] = strdup(ponto_get_id(pontos[i]));
        }
    }

    // ordenando os clusters para a saida  
    for (int i = 0; i < num_clusters; i++) {
        qsort(clusters_array[i].ids, clusters_array[i].tamanho, sizeof(char *), cmp_str);
    }
    qsort(clusters_array, num_clusters, sizeof(cluster_t), cmp_clusters);

    // IMPRESSAO NO ARQUIVO DE SAIDA
    for (int i = 0; i < num_clusters; i++) {
        fprintf(out, "%s", clusters_array[i].ids[0]);
        for (int j = 1; j < clusters_array[i].tamanho; j++) {
            fprintf(out, ",%s", clusters_array[i].ids[j]);
        }
        fprintf(out, "\n");
    }
    
    /////////////////// DEBUG //////////////////
    // for(int i = 0; i < n; i++) {
    //     fprintf(out, "Ponto %s: ", ponto_get_id(pontos[i]));
    //     for(int j = 0; j < m; j++) {
    //         fprintf(out, "%.2lf ", ponto_get_coords(pontos[i])[j]);
    //     }
    //     fprintf(out, "\n");
    // }
    // for(int i = 0; i < num_arestas; i++) {
    //     fprintf(out, "Aresta %d -> %d: %.2lf\n", aresta_get_from(arestas[i]), aresta_get_to(arestas[i]), aresta_get_dist(arestas[i]));
    // }
    // double *coords = malloc(3 * sizeof(double));
    // coords[0] = 1.0;
    // coords[1] = 2.0;
    // coords[2] = 3.0;
    // ponto_t *p = ponto_create("A", coords, 3);
    // aresta_t *a = aresta_create(1, 2, 3.0);
    // printf("Ponto ID: %s\n", ponto_get_id(p));
    // printf("Ponto Coords: %lf %lf %lf\n", ponto_get_coords(p)[0], ponto_get_coords(p)[1], ponto_get_coords(p)[2]);
    // printf("Aresta From: %d\n", aresta_get_from(a));
    // printf("Aresta To: %d\n", aresta_get_to(a));
    // printf("Aresta Dist: %lf\n", aresta_get_dist(a));
    // printf("Valor de k: %d\n", k);
    // size_t i = 256;
    // char *buffer = malloc(i * sizeof(char));
    // getline(&buffer, &i, in);
    // printf("Linha lida do arquivo: %s\n", buffer);
    // fprintf(out, "Linha lida do arquivo: %s\n", buffer);
    // free(coords);
    // free(buffer);
    // ponto_destroy(p);
    // aresta_destroy(a);
    //////////////////////////////////////////

    for(int i = 0; i < n; i++) {
        ponto_destroy(pontos[i]);
    }
    free(pontos);
    for(int i = 0; i < num_arestas; i++) {
        aresta_destroy(arestas[i]);
    }
    free(arestas);
    free(mst); // nao precisa varrer tudo pra varrer pq o que destroi as arestas do vetor de arestas tbm destroi aqui
    uf_destroy(uf);
    uf_destroy(uf_clusters);
    cluster_destroy(clusters_array, num_clusters); 

    fclose(in);
    fclose(out);
    return 0;
}