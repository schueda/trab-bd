#include <stdio.h>
#include <stdlib.h>
#include "conflict.h"
#include "../query/query.h"

conflictsT *create_conflicts() {
    conflictsT *conflicts = (conflictsT *) malloc(sizeof(conflictsT));
    if (conflicts == NULL) { return NULL; }

    conflicts->transactions = (int *) malloc(CONFLICTS_MAX_SIZE * sizeof(int));
    if (conflicts->transactions == NULL) {
        free(conflicts);
        return NULL;
    }

    return conflicts;
}

void conflict_process_query(graphT *graph, queryT *query, query_tableT *query_table, conflictsT *conflicts) {
    query_table_insert(query, query_table, conflicts);

    if (!find_vertex(graph, query->transaction_id)) {
        for (int i = 0; i < conflicts->count; i++) {
            printf("(%d, %d)\n", conflicts->transactions[i], query->transaction_id);
            // if (add_vertex(graph, query->transaction_id) == -1) {
            //     perror("Não foi possível adicionar a aresta ao grafo\n");
            //     return;
            // }
        }
        conflicts->count = 0;
    }
}

int destroy_conflicts(conflictsT *conflicts) {
    return -1;
}