#include <stdio.h>
#include "query/query.h"
#include "conflict/conflict.h"
#include "graph/graph.h"

int main(int argc, char const *argv[]) {
    graphT *conflict_graph = create_graph();
    if (conflict_graph == NULL) {
        perror("Não foi possível alocar o grafo.\n");
        return 1;
    }

    query_tableT *query_table = create_query_table();
    if (query_table == NULL) {
        perror("Não foi possível alocar a tabela de query.\n");
        return 1;
    }

    char buffer[64];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        queryT *query = create_query(buffer); 
        if (query == NULL) {
            printf("operação inválida: %s\n", buffer);
            continue;
        }

        conflict_process_query(conflict_graph, query, query_table);
    }

    destroy_graph(conflict_graph);
    destroy_query_table(query_table);
    return 0;
}
