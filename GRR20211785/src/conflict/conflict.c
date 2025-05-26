#include <stdio.h>
#include "conflict.h"

void conflict_process_query(graphT *graph, queryT *query) {
    if (!find_vertex(graph, query->transaction_id)) {
        add_vertex(graph, query->transaction_id);
    }
    
}