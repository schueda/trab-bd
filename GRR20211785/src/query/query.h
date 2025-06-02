#ifndef __QUERY__
#define __QUERY__

#include "../conflict/conflict.h"

typedef enum query_op operationT; 
enum query_op {
    READ = 'R',
    WRITE = 'W',
    COMMIT = 'C'
};

typedef struct query queryT;
struct query {
    int timestamp;
    int transaction_id;
    operationT operation;
    char resource;
};

typedef struct query_node query_nodeT;
struct query_node {
    queryT *query;
    query_nodeT *next;
};

queryT *create_query(char *buffer);
void print_query(queryT *query);
int destroy_query(queryT *query);

query_nodeT *create_query_node(queryT *query);
int destroy_query_node(query_nodeT *node);

#endif