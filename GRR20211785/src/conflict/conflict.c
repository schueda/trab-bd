#include <stdio.h>
#include "conflict.h"

void conflict_process_query(graphT *graph, queryT *query, query_tableT *query_table) {
    if (!find_vertex(graph, query->transaction_id)) {
        if (add_vertex(graph, query->transaction_id) == -1) {
            perror("Não foi possível adicionar a aresta ao grafo\n");
            return;
        }
    }
    query_table_insert(query, query_table);
}