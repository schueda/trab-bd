#ifndef __GRAPH__
#define __GRAPH__

typedef struct edge edgeT;
typedef struct vertex vertexT;
typedef struct graph graphT;

#define MAX_VERTEX 16

struct edge {
    vertexT *to;
    edgeT *next;
};

struct vertex {
    int value;
    edgeT *frontier;
};

struct graph {
    vertexT **vertices;
    int vertices_count;
};

graphT *create_graph();
int add_vertex(graphT *graph, int value);
void add_edge(graphT *graph, int from, int to);
int find_vertex(graphT *graph, int id);
void print_graph(graphT *graph);
int check_for_cycles(graphT *graph);
int empty_graph(graphT *graph);
int destroy_graph(graphT *graph);

#endif