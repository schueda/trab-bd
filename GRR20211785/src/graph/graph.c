
#include "graph.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Cria um novo vértice com o valor especificado
 *
 * @param value Valor do vértice
 * @return vertexT* Ponteiro para o vértice criado, ou NULL em caso de erro
 */
vertexT *create_vertex(int value) {
    vertexT *vertex = (vertexT *)malloc(sizeof(vertexT));
    vertex->value = value;

    vertex->visited = 0;
    return vertex;
}

/**
 * @brief Destrói um vértice, liberando sua memória
 *
 * @param vertex Ponteiro para o vértice a ser destruído
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_vertex(vertexT *vertex) {
    if (vertex == NULL)
        return -1;

    free(vertex);
    return 0;
}

/**
 * @brief Cria uma nova aresta apontando para o vértice de destino
 *
 * @param dest_vertex Ponteiro para o vértice de destino
 * @return edgeT* Ponteiro para a aresta criada, ou NULL em caso de erro
 */
edgeT *create_edge(vertexT *dest_vertex) {
    edgeT *edge = (edgeT *)malloc(sizeof(edgeT));
    if (edge == NULL)
        return NULL;

    edge->to_vertex = dest_vertex;
    edge->next = NULL;

    return edge;
}

/**
 * @brief Destrói uma aresta, liberando sua memória
 *
 * @param edge Ponteiro para a aresta a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_edge(edgeT *edge) {
    if (edge == NULL)
        return -1;

    free(edge);
    return 0;
}

/**
 * @brief Cria um novo grafo vazio
 *
 * @return graphT* Ponteiro para o grafo criado, ou NULL em caso de erro
 */
graphT *create_graph() {
    graphT *graph = (graphT *)malloc(sizeof(graphT));
    graph->vertices = (vertexT **)malloc(MAX_VERTICES * sizeof(vertexT *));
    if (graph->vertices == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}

/**
 * @brief Calcula o hash de um ID para a tabela de vértices
 *
 * @param id ID do vértice
 * @return int Índice na tabela hash
 */
int g_hash(int id) {
    return id % MAX_VERTICES;
}

/**
 * @brief Obtém ou cria um vértice com o valor especificado
 *
 * @param graph Ponteiro para o grafo
 * @param value Valor do vértice a ser obtido/criado
 * @return vertexT* Ponteiro para o vértice, ou NULL em caso de erro
 */
vertexT *get_vertex(graphT *graph, int value) {
    if (graph == NULL || graph->vertices == NULL)
        return NULL;

    for (int inc = 0; inc < MAX_VERTICES; inc++) {
        int vertex_hash = g_hash(value + inc);
        vertexT *vertex = graph->vertices[vertex_hash];
        if (vertex == NULL) {
            graph->vertices[vertex_hash] = create_vertex(value);
            return graph->vertices[vertex_hash];
        } else if (vertex->value == value) {
            return vertex;
        }
    }

    perror("Grafo não pode mais receber novos vértices...\n");
    return NULL;
}

/**
 * @brief Adiciona uma aresta ao grafo
 *
 * @param graph Ponteiro para o grafo
 * @param origin_vertex Ponteiro para o vértice de origem
 * @param dest_vertex Ponteiro para o vértice de destino
 * @return int 1 se a aresta foi adicionada, 0 se já existia, -1 em caso de erro
 */
int add_edge(graphT *graph, vertexT *origin_vertex, vertexT *dest_vertex) {
    if (graph == NULL)
        return -1;
    if (origin_vertex == NULL || dest_vertex == NULL)
        return -1;

    if (origin_vertex->frontier == NULL) {
        origin_vertex->frontier = create_edge(dest_vertex);
        return 1;
    }

    edgeT *prev_edge = NULL;
    edgeT *edge = origin_vertex->frontier;
    while (edge != NULL) {
        if (edge->to_vertex == dest_vertex)
            return 0;

        prev_edge = edge;
        edge = edge->next;
    }
    prev_edge->next = create_edge(dest_vertex);
    return 1;
}

/**
 * @brief Imprime o grafo no formato texto
 *
 * @param graph Ponteiro para o grafo a ser impresso
 */
void print_graph(graphT *graph) {
    if (graph == NULL || graph->vertices == NULL)
        return;

    for (int i = 0; i < MAX_VERTICES; i++) {
        if (graph->vertices[i] != NULL) {
            printf("%d:", graph->vertices[i]->value);

            edgeT *edge = graph->vertices[i]->frontier;
            while (edge != NULL) {
                printf(" %d", edge->to_vertex->value);
                edge = edge->next;
            }
            printf("\n");
        }
    }
}

/**
 * @brief Realiza uma busca em profundidade (DFS) para detectar ciclos
 *
 * @param r Ponteiro para o vértice raiz
 * @return int 1 se encontrar um ciclo, 0 caso contrário
 */
int dfs(vertexT *r) {
    r->visited = 1;

    edgeT *edge = r->frontier;
    while (edge != NULL) {
        vertexT *neighbor = edge->to_vertex;
        if (neighbor->visited == 1) {
            return 1;
        } else if (neighbor->visited == 0) {
            if (dfs(neighbor)) {
                return 1;
            }
        }
        edge = edge->next;
    }

    r->visited = 2;
    return 0;
}

/**
 * @brief Verifica se o grafo possui ciclos
 *
 * @param graph Ponteiro para o grafo
 * @return int 1 se houver ciclos, 0 se não houver, -1 em caso de erro
 */
int check_for_cycles(graphT *graph) {
    if (graph == NULL || graph->vertices == NULL)
        return -1;

    for (int i = 0; i < MAX_VERTICES; i++) {
        vertexT *cur_vertex = graph->vertices[i];
        if (cur_vertex != NULL && cur_vertex->visited == 0) {
            if (dfs(cur_vertex)) {
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Esvazia o grafo, removendo todos os vértices e arestas
 *
 * @param graph Ponteiro para o grafo a ser esvaziado
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_graph(graphT *graph) {
    if (graph == NULL || graph->vertices == NULL)
        return -1;

    for (int i = 0; i < MAX_VERTICES; i++) {
        vertexT *vertex = graph->vertices[i];
        if (vertex != NULL) {
            edgeT *edge = vertex->frontier;
            while (edge != NULL) {
                edgeT *next_edge = edge->next;
                destroy_edge(edge);
                edge = next_edge;
            }
        }
        destroy_vertex(vertex);
        graph->vertices[i] = NULL;
    }

    return 0;
}

/**
 * @brief Destrói o grafo, liberando toda a memória alocada
 *
 * @param graph Ponteiro para o grafo a ser destruído
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_graph(graphT *graph) {
    if (graph == NULL)
        return -1;
    empty_graph(graph);
    if (graph->vertices != NULL) {
        free(graph->vertices);
    }
    free(graph);
    return 0;
}