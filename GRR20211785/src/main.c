#include <stdio.h>

#include "conflict/conflict.h"
#include "graph/graph.h"
#include "query/query.h"
#include "transaction/transaction.h"
#include "vision/vision.h"

/** @brief Tamanho do buffer para leitura de entrada */
#define INPUT_BUFFER 64

int main(int argc, char const *argv[]) {
    transaction_tableT *transaction_table = create_transaction_table();
    if (transaction_table == NULL) {
        perror("Não foi possível alocar a tabela de transações.\n");
        return 1;
    }

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

    conflictsT *conflicts = create_conflicts();
    if (conflicts == NULL) {
        perror("Não foi possível alocar a lista de conflitos.\n");
        return 1;
    }

    vision_query_tableT *vision_query_table = create_vision_query_table();
    if (vision_query_table == NULL) {
        perror("Não foi possível alocar a tabela de query de visão.\n");
        return 1;
    }

    int schedule = 1;
    int present = 0;

    char buffer[INPUT_BUFFER];
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        queryT *query = create_query(buffer);
        if (query == NULL) {
            printf("operação inválida: %s", buffer);
            continue;
        }

        if (query->operation == COMMIT) {
            commit_transaction(query->transaction_id, transaction_table);
        } else if (open_transaction(query->transaction_id, transaction_table) != -1) {
            conflict_process_query(query, conflict_graph, conflict_query_table, conflicts);
            vision_process_query(query, vision_query_table);
            present = 1;
        } else {
            return 1;
        }

        if (transaction_table->open_count <= 0 && present) {
            char tran_str[TRANSACTION_LIST_BUFFER] = "";
            list_transactions(tran_str, transaction_table);

            char *result_serial = check_for_cycles(conflict_graph) ? "NS" : "SS";
            char *result_vision = check_equivalent_vision(vision_query_table) ? "SV" : "NV";

            printf("%d %s %s %s\n", schedule, tran_str, result_serial, result_vision);

            empty_graph(conflict_graph);
            empty_conflict_query_table(conflict_query_table);
            empty_transaction_table(transaction_table);

            empty_vision_query_table(vision_query_table);
            schedule++;
            present = 0;
        }

        destroy_query(query);
    }

    destroy_transaction_table(transaction_table);

    destroy_graph(conflict_graph);
    destroy_conflict_query_table(conflict_query_table);
    destroy_conflicts(conflicts);

    destroy_vision_query_table(vision_query_table);

    return 0;
}
