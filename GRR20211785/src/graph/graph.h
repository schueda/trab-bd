#ifndef __GRAPH__
#define __GRAPH__

typedef struct edge edgeT;
typedef struct vertex vertexT;
typedef struct graph graphT;

#define MAX_VERTICES 16

struct edge {
    vertexT *to_vertex;
    edgeT *next;
};

struct vertex {
    int value;
    edgeT *frontier;
};

struct graph {
    vertexT **vertices;
};

graphT *create_graph();
vertexT *get_vertex(graphT *graph, int value);
int add_edge(graphT *graph, vertexT *origin_vertex, vertexT *dest_vertex);
void print_graph(graphT *graph);
int check_for_cycles(graphT *graph);
int empty_graph(graphT *graph);
int destroy_graph(graphT *graph);

#endif