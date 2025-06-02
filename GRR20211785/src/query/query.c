#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "query.h"

queryT *create_query(char *buffer) {
    queryT *query = (queryT *) malloc(sizeof(queryT));
    if (query == NULL) return NULL;

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

        query->operation = (operationT) temp_operation;
        return query;
    }
    
    free(query);
    return NULL;
}

void print_query(queryT *query) {
    printf("timestamp: %d\n", query->timestamp);
    printf("transaction_id: %d\n", query->transaction_id);
    printf("operation: %c\n", query->operation);
    printf("resource: %c\n", query->resource);
}

int destroy_query(queryT *query) {
    if (query == NULL) return -1;

    free(query);
    return 0;
}

queryT *copy_query(queryT *query) {
    queryT *new_query = (queryT *) malloc(sizeof(queryT));
    memcpy(new_query, query, sizeof(queryT));
    return new_query;
}

query_nodeT *create_query_node(queryT *query) {
    query_nodeT *query_node = (query_nodeT *) malloc(sizeof(query_nodeT));
    if (query_node == NULL) return NULL;

    query_node->query = copy_query(query);

    query_node->next = NULL;
    return query_node;
}

int destroy_query_node(query_nodeT *node) {
    if (node == NULL) return -1;

    if (node->query != NULL) {
        free(node->query);
        node->query = NULL;
    }

    free(node);
    return 0;
}