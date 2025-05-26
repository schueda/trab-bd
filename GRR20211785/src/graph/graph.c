#include <stdlib.h>
#include <stdio.h>
#include "graph.h"

vertexT *create_vertex(int value) {
    vertexT *vertex = (vertexT *) malloc(sizeof(vertexT));
    vertex->value = value;
    return vertex;
}

void destroy_vertex(vertexT *vertex) {

}

edgeT *create_edge(int destination) {
    return NULL;
}

void destroy_edge(edgeT *edge) {

}

graphT *create_graph() {
    graphT *graph = (graphT *) malloc(sizeof(graphT));
    graph->vertices = (vertexT **) malloc(MAX_VERTEX * sizeof(vertexT));
    graph->vertices_count = 0;

    return graph;
}

void add_vertex(graphT *graph, int value) {
    vertexT *vertex = create_vertex(value);
    graph->vertices[graph->vertices_count] = vertex;
    graph->vertices_count++;
}

void add_edge(graphT *graph, int from, int to) {

}

int find_vertex(graphT *graph, int id) {
    for (int i = 0; i < graph->vertices_count; i++) {
        if (graph->vertices[i]->value == id) {
            return 1;
        }
    }
    return 0;
}

void print_graph(graphT *graph) {
}

int check_for_cycles(graphT *graph) {
    return 0;
}

int destroy_graph(graphT *graph) {
    printf("bfs to free everything.\n");
    return 1;
}