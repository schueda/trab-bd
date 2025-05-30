#ifndef __CONFLICT__
#define __CONFLICT__

#include "../graph/graph.h"

typedef struct query queryT;
typedef struct query_node query_nodeT;

#define CONFLICTS_MAX_SIZE 16
#define CONFLICT_QUERY_TABLE_SIZE 16

typedef struct conflicts conflictsT;
struct conflicts {
    int *transactions;
    int count;
};

typedef struct conflict_query_table conflict_query_tableT;
struct conflict_query_table {
    query_nodeT **nodes;
    int new_entries;
};

conflictsT *create_conflicts();
void conflict_process_query(graphT *graph, queryT *query, conflict_query_tableT *conflict_query_table, conflictsT *conflicts);
int destroy_conflicts(conflictsT *conflicts);

conflict_query_tableT *create_conflict_query_table();
int conflict_query_table_insert(queryT *query, conflict_query_tableT *table, conflictsT *conflicts);
int empty_conflict_query_table(conflict_query_tableT *table);
int destroy_conflict_query_table(conflict_query_tableT *table);


#endif