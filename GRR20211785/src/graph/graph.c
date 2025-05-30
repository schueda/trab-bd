#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

vertexT *create_vertex(int value) {
    vertexT *vertex = (vertexT *) malloc(sizeof(vertexT));
    vertex->value = value;
    return vertex;
}

int destroy_vertex(vertexT *vertex) {

}

edgeT *create_edge(vertexT *dest_vertex) {
    edgeT *edge = (edgeT *) malloc(sizeof(edgeT));
    if (edge == NULL) return NULL;

    edge->to_vertex = dest_vertex;
    edge->next = NULL;

    return edge;
}

int destroy_edge(edgeT *edge) {

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
    if (graph == NULL) return NULL;

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

int add_edge(graphT *graph, int from, int to) {
    if (graph == NULL) return -1;

    vertexT *origin_vertex = get_vertex(graph, from);
    vertexT *dest_vertex = get_vertex(graph, to);
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
    if (graph == NULL) return;

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
}

int destroy_graph(graphT *graph) {
}