#ifndef __CONFLICT__
#define __CONFLICT__

#include "../graph/graph.h"

typedef struct query queryT;
typedef struct query_table query_tableT;

#define CONFLICTS_MAX_SIZE 16

typedef struct conflicts conflictsT;
struct conflicts {
    int *transactions;
    int count;
};

conflictsT *create_conflicts();
void conflict_process_query(graphT *graph, queryT *query, query_tableT *query_table, conflictsT *conflicts);
int destroy_conflicts(conflictsT *conflicts);

#endif