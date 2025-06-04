#include "conflict.h"

#include <stdio.h>
#include <stdlib.h>

#include "../query/query.h"

/**
 * @brief Insere uma operação na tabela de operações de conflito
 *
 * @param query Ponteiro para a operação a ser inserida
 * @param table Ponteiro para a tabela de operações
 * @param conflicts Ponteiro para a lista de conflitos
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int conflict_query_table_insert(queryT *query, conflict_query_tableT *table, conflictsT *conflicts);

/**
 * @brief Processa uma operação para detecção de conflitos
 *
 * Esta função insere a operação na tabela de operações e verifica
 * se há conflitos com operações anteriores. Se houver conflitos,
 * adiciona arestas ao grafo de conflitos.
 *
 * @param query Ponteiro para a operação a ser processada
 * @param graph Ponteiro para o grafo de conflitos
 * @param query_table Ponteiro para a tabela de operações de conflito
 * @param conflicts Ponteiro para a lista de conflitos
 */
void conflict_process_query(queryT *query, graphT *graph, conflict_query_tableT *query_table, conflictsT *conflicts) {
    conflict_query_table_insert(query, query_table, conflicts);

    vertexT *dest_vertex = get_vertex(graph, query->transaction_id);
    for (int i = 0; i < conflicts->count; i++) {
        vertexT *origin_vertex = get_vertex(graph, conflicts->transactions[i]);
        add_edge(graph, origin_vertex, dest_vertex);
    }
    conflicts->count = 0;
}

/**
 * @brief Cria uma nova lista de conflitos
 *
 * @return conflictsT* Ponteiro para a lista criada, ou NULL em caso de erro
 */
conflictsT *create_conflicts() {
    conflictsT *conflicts = (conflictsT *)malloc(sizeof(conflictsT));
    if (conflicts == NULL)
        return NULL;

    conflicts->transactions = (int *)malloc(CONFLICTS_MAX_SIZE * sizeof(int));
    if (conflicts->transactions == NULL) {
        free(conflicts);
        return NULL;
    }

    return conflicts;
}

/**
 * @brief Destrói uma lista de conflitos
 *
 * @param conflicts Ponteiro para a lista a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_conflicts(conflictsT *conflicts) {
    if (conflicts == NULL)
        return -1;

    if (conflicts->transactions != NULL) {
        free(conflicts->transactions);
    }

    free(conflicts);
    return 0;
}

/**
 * @brief Cria uma nova tabela de operações de conflito
 *
 * @return conflict_query_tableT* Ponteiro para a tabela criada, ou NULL em caso de erro
 */
conflict_query_tableT *create_conflict_query_table() {
    conflict_query_tableT *table = (conflict_query_tableT *)malloc(sizeof(conflict_query_tableT));
    if (table == NULL)
        return NULL;

    table->nodes = (query_nodeT **)malloc(CONFLICT_QUERY_TABLE_SIZE * sizeof(query_nodeT *));
    if (table->nodes == NULL) {
        free(table);
        return NULL;
    }

    return table;
}

/**
 * @brief Calcula o hash de um recurso para a tabela de operações
 *
 * @param c Caractere representando o recurso
 * @return int Índice na tabela hash
 */
int q_hash(char c) {
    return (int)c % CONFLICT_QUERY_TABLE_SIZE;
}

/**
 * @brief Compara duas operações para detectar conflitos
 *
 * @param qi Ponteiro para a primeira operação
 * @param qj Ponteiro para a segunda operação
 * @param conflicts Ponteiro para a lista de conflitos
 * @return int 1 se houver conflito, 0 se não houver, -1 em caso de erro
 */
int compare(queryT *qi, queryT *qj, conflictsT *conflicts) {
    if (qi->transaction_id == qj->transaction_id)
        return 0;

    if (qj->operation == WRITE || (qj->operation == READ && qi->operation == WRITE)) {
        if (conflicts->count >= CONFLICTS_MAX_SIZE)
            return -1;

        conflicts->transactions[conflicts->count++] = qi->transaction_id;
        return 1;
    }
    return 0;
}

/**
 * @brief Insere uma operação na tabela de operações de conflito
 *
 * @param query Ponteiro para a operação a ser inserida
 * @param table Ponteiro para a tabela de operações
 * @param conflicts Ponteiro para a lista de conflitos
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int conflict_query_table_insert(queryT *query, conflict_query_tableT *table, conflictsT *conflicts) {
    if (query == NULL || table == NULL || table->nodes == NULL || conflicts == NULL || conflicts->transactions == NULL)
        return -1;

    for (int inc = 0; inc < CONFLICT_QUERY_TABLE_SIZE; inc++) {
        int query_hash = q_hash(query->resource + inc);
        if (table->nodes[query_hash] == NULL) {
            query_nodeT *query_node = create_query_node(query);
            if (query_node == NULL)
                return -1;

            table->nodes[query_hash] = query_node;
            return 0;
        } else if (table->nodes[query_hash]->query->resource == query->resource) {
            query_nodeT *node = table->nodes[query_hash];
            while (node->next != NULL) {
                compare(node->query, query, conflicts);
                node = node->next;
            }

            compare(node->query, query, conflicts);
            node->next = create_query_node(query);
            return 1;
        }
    }

    perror("Tabela query de conflito cheia.");
    return -1;
}

/**
 * @brief Esvazia uma tabela de operações de conflito
 *
 * @param table Ponteiro para a tabela a ser esvaziada
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_conflict_query_table(conflict_query_tableT *table) {
    if (table == NULL || table->nodes == NULL)
        return -1;

    for (int i = 0; i < CONFLICT_QUERY_TABLE_SIZE; i++) {
        query_nodeT *node = table->nodes[i];
        while (node != NULL) {
            query_nodeT *next_node = node->next;
            destroy_query_node(node);
            node = next_node;
        }
        table->nodes[i] = NULL;
    }

    return 0;
}

/**
 * @brief Destrói uma tabela de operações de conflito
 *
 * @param table Ponteiro para a tabela a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_conflict_query_table(conflict_query_tableT *table) {
    if (table == NULL)
        return -1;

    empty_conflict_query_table(table);

    if (table->nodes != NULL) {
        free(table->nodes);
    }

    free(table);
    return 0;
}