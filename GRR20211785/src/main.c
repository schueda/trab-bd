#include <stdio.h>
#include "utils/utils.h"
#include "conflict/conflict.h"
#include "graph/graph.h"

int main(int argc, char const *argv[]) {
    graphT *conflict_graph = create_graph();

    char buffer[64];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        queryT *query = make_query(buffer); 
        if (query == NULL) {
            printf("operação inválida: %s\n", buffer);
            destroy_query(query);
            continue;
        }

        conflict_process_query(conflict_graph, query);
        destroy_query(query);
    }

    destroy_graph(conflict_graph);
    return 0;
}
