#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

vertexT *create_vertex(int value) {
    vertexT *vertex = (vertexT *) malloc(sizeof(vertexT));
    vertex->value = value;
    return vertex;
}

int destroy_vertex(vertexT *vertex) {
    if (vertex == NULL) return -1;

    free(vertex);
    return 0;
}

edgeT *create_edge(vertexT *dest_vertex) {
    edgeT *edge = (edgeT *) malloc(sizeof(edgeT));
    if (edge == NULL) return NULL;

    edge->to_vertex = dest_vertex;
    edge->next = NULL;

    return edge;
}

int destroy_edge(edgeT *edge) {
    if (edge == NULL) return -1;

    free(edge);
    return 0;
}

graphT *create_graph() {
    graphT *graph = (graphT *) malloc(sizeof(graphT));
    graph->vertices = (vertexT **) malloc(MAX_VERTICES * sizeof(vertexT));
    if (graph->vertices == NULL) {
        free(graph);
        return NULL;
    }

    return graph;
}

int g_hash(int id) {
    return id % MAX_VERTICES;
}

vertexT *get_vertex(graphT *graph, int value) {
    if (graph == NULL || graph->vertices == NULL) return NULL;

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

int add_edge(graphT *graph, vertexT *origin_vertex, vertexT *dest_vertex) {
    if (graph == NULL) return -1;
    if (origin_vertex == NULL || dest_vertex == NULL) return -1;
    
    if (origin_vertex->frontier == NULL) {
        origin_vertex->frontier = create_edge(dest_vertex);
        return 1;
    }

    edgeT *prev_edge = NULL;
    edgeT *edge = origin_vertex->frontier;
    while (edge != NULL) {
        if (edge->to_vertex == dest_vertex) return 0;

        prev_edge = edge;
        edge = edge->next;
    }
    prev_edge->next = create_edge(dest_vertex);
    return 1;
}

void print_graph(graphT *graph) {
    if (graph == NULL || graph->vertices == NULL) return;

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

int check_for_cycles(graphT *graph) {
    return 0;
}

int empty_graph(graphT *graph) {
    if (graph == NULL || graph->vertices == NULL) return -1;

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

int destroy_graph(graphT *graph) {
    if (graph == NULL) return -1;
    empty_graph(graph);
    if (graph->vertices != NULL) {
        free(graph->vertices);
    }
    free(graph);
    return 0;
}