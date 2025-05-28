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

queryT *create_query(char *buffer);
void print_query(queryT *query);
int destroy_query(queryT *query);

//--------------------------------------------------------------
#define QUERY_TABLE_SIZE 16

typedef struct query_node query_nodeT;
struct query_node {
    queryT *query;
    query_nodeT *next;
};

typedef struct query_table query_tableT;
struct query_table {
    query_nodeT **nodes;
    int new_entries;
};

query_tableT *create_query_table();
int query_table_insert(queryT *query, query_tableT *table, conflictsT *conflicts);
int empty_query_table(query_tableT *table);
int destroy_query_table(query_tableT *table);

#endif