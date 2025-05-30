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

    query_tableT *query_table = create_query_table();
    if (query_table == NULL) {
        perror("Não foi possível alocar a tabela de query.\n");
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
            conflict_process_query(conflict_graph, query, query_table, conflicts);
        } else {
            return 1;
        }
    
        if (transaction_table->open_count <= 0 && query_table->new_entries == 1) {
            char tran_str[TRANSACTION_LIST_BUFFER] = "";
            list_transactions(tran_str, transaction_table);

            print_graph(conflict_graph);

            char *result_serial = check_for_cycles(conflict_graph) ? "SS" : "NS";

            printf("%d %s %s\n", schedule, tran_str, result_serial);

            empty_graph(conflict_graph);
            empty_query_table(query_table);
            empty_transaction_table(transaction_table);
            schedule++;
        }
    }
    
    destroy_graph(conflict_graph);
    destroy_query_table(query_table);
    destroy_transaction_table(transaction_table);
    destroy_conflicts(conflicts);
    return 0;
}
