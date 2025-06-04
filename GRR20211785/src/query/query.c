#include "query.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma nova operação a partir de um buffer de entrada
 *
 * A função lê os dados da operação do buffer no formato:
 * timestamp transaction_id operation resource
 *
 * @param buffer String contendo os dados da operação
 * @return queryT* Ponteiro para a operação criada, ou NULL em caso de erro
 */
queryT *create_query(char *buffer) {
    queryT *query = (queryT *)malloc(sizeof(queryT));
    if (query == NULL)
        return NULL;

    char temp_operation;
    if (sscanf(buffer, "%d %d %c %c", &query->timestamp, &query->transaction_id, &temp_operation, &query->resource)) {
        if (temp_operation != READ && temp_operation != WRITE && temp_operation != COMMIT) {
            free(query);
            return NULL;
        }

        if (query->transaction_id >= 10000) {
            free(query);
            return NULL;
        }

        query->operation = (operationT)temp_operation;
        return query;
    }

    free(query);
    return NULL;
}

/**
 * @brief Imprime os dados de uma operação no formato legível
 *
 * @param query Ponteiro para a operação a ser impressa
 */
void print_query(queryT *query) {
    printf("timestamp: %d\n", query->timestamp);
    printf("transaction_id: %d\n", query->transaction_id);
    printf("operation: %c\n", query->operation);
    printf("resource: %c\n", query->resource);
}

/**
 * @brief Destrói uma operação, liberando sua memória
 *
 * @param query Ponteiro para a operação a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_query(queryT *query) {
    if (query == NULL) return -1;

    free(query);
    return 0;
}

/**
 * @brief Cria uma cópia de uma operação
 *
 * @param query Ponteiro para a operação a ser copiada
 * @return queryT* Ponteiro para a cópia da operação, ou NULL em caso de erro
 */
queryT *copy_query(queryT *query) {
    queryT *new_query = (queryT *)malloc(sizeof(queryT));
    memcpy(new_query, query, sizeof(queryT));
    return new_query;
}

/**
 * @brief Cria um novo nó de lista encadeada para uma operação
 *
 * @param query Ponteiro para a operação a ser armazenada no nó
 * @return query_nodeT* Ponteiro para o nó criado, ou NULL em caso de erro
 */
query_nodeT *create_query_node(queryT *query) {
    query_nodeT *query_node = (query_nodeT *)malloc(sizeof(query_nodeT));
    if (query_node == NULL)
        return NULL;

    query_node->query = copy_query(query);

    query_node->next = NULL;
    return query_node;
}

/**
 * @brief Destrói um nó de lista encadeada, liberando sua memória
 *
 * @param node Ponteiro para o nó a ser destruído
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_query_node(query_nodeT *node) {
    if (node == NULL) return -1;

    if (node->query != NULL) {
        free(node->query);
        node->query = NULL;
    }

    free(node);
    return 0;
}