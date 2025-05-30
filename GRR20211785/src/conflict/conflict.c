#include <stdio.h>
#include <stdlib.h>
#include "conflict.h"
#include "../query/query.h"

conflictsT *create_conflicts() {
    conflictsT *conflicts = (conflictsT *) malloc(sizeof(conflictsT));
    if (conflicts == NULL) return NULL;

    conflicts->transactions = (int *) malloc(CONFLICTS_MAX_SIZE * sizeof(int));
    if (conflicts->transactions == NULL) {
        free(conflicts);
        return NULL;
    }

    return conflicts;
}

void conflict_process_query(graphT *graph, queryT *query, conflict_query_tableT *query_table, conflictsT *conflicts) {
    conflict_query_table_insert(query, query_table, conflicts);

    vertexT *dest_vertex = get_vertex(graph, query->transaction_id);
    for (int i = 0; i < conflicts->count; i++) {
        vertexT *origin_vertex = get_vertex(graph, conflicts->transactions[i]);
        add_edge(graph, origin_vertex, dest_vertex);
        printf("(%d, %d)\n", conflicts->transactions[i], query->transaction_id);
    }
    conflicts->count = 0;
}

int destroy_conflicts(conflictsT *conflicts) {
    if (conflicts == NULL) return -1;

    if (conflicts->transactions != NULL) {
        free(conflicts->transactions);
    }

    free(conflicts);
    return 0;
}