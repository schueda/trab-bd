#include <stdio.h>
#include "query/query.h"
#include "conflict/conflict.h"
#include "graph/graph.h"
#include "transaction/transaction.h"

#define INPUT_BUFFER 64

int main(int argc, char const *argv[]) {
    graphT *conflict_graph = create_graph();
    if (conflict_graph == NULL) {
        perror("Não foi possível alocar o grafo.\n");
        return 1;
    }

    conflict_query_tableT *conflict_query_table = create_conflict_query_table();
    if (conflict_query_table == NULL) {
        perror("Não foi possível alocar a tabela de query de conflitos.\n");
        return 1;
    }

    transaction_tableT *transaction_table = create_transaction_table();
    if (transaction_table == NULL) {
        perror("Não foi possível alocar a tabela de transações.\n");
        return 1;
    }

    conflictsT *conflicts = create_conflicts();
    if (conflicts == NULL) {
        perror("Não foi possível alocar a lista de conflitos.\n");
        return 1;
    }

    int schedule = 1;

    char buffer[INPUT_BUFFER];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        queryT *query = create_query(buffer);
        if (query == NULL) {
            printf("operação inválida: %s", buffer);
            continue;
        }

        if (query->operation == COMMIT) {
            commit_transaction(query->transaction_id, transaction_table);
            destroy_query(query);
        } else if (open_transaction(query->transaction_id, transaction_table) != -1) {
            conflict_process_query(conflict_graph, query, conflict_query_table, conflicts);
        } else {
            return 1;
        }
    
        if (transaction_table->open_count <= 0 && conflict_query_table->new_entries == 1) {
            char tran_str[TRANSACTION_LIST_BUFFER] = "";
            list_transactions(tran_str, transaction_table);

            char *result_serial = check_for_cycles(conflict_graph) ? "NS" : "SS";

            printf("%d %s %s\n", schedule, tran_str, result_serial);

            empty_graph(conflict_graph);
            empty_conflict_query_table(conflict_query_table);
            empty_transaction_table(transaction_table);
            schedule++;
        }
    }
    
    destroy_graph(conflict_graph);
    destroy_conflict_query_table(conflict_query_table);
    destroy_transaction_table(transaction_table);
    destroy_conflicts(conflicts);
    return 0;
}
