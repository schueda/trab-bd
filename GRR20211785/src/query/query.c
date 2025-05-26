#include <stdio.h>
#include <stdlib.h>
#include "query.h"

queryT *create_query(char *buffer) {
    queryT *query = (queryT *) malloc(sizeof(queryT));
    if (query == NULL) {
        return NULL;
    }

    char temp_operation;
    if (sscanf(buffer, "%d %d %c %c", &query->timestamp, &query->transaction_id, &temp_operation, &query->resource)) {
        if (temp_operation != READ && temp_operation != WRITE && temp_operation != COMMIT) {
            free(query);
            return NULL;        
        } 

        query->operation = (operationT) temp_operation;
        return query;
    }
    
    free(query);
    return NULL;
}

int destroy_query(queryT *query) {
    if (query != NULL) {
        free(query);    
        return 1;
    } 
    return 0;
}

void print_query(queryT *query) {
    printf("timestamp: %d\n", query->timestamp);
    printf("transaction_id: %d\n", query->transaction_id);
    printf("operation: %c\n", query->operation);
    printf("resource: %c\n", query->resource);
}

query_tableT *create_query_table() {
    query_tableT *query_table = (query_tableT *) malloc(sizeof(query_tableT));
    if (query_table != NULL) {
        query_table->nodes = (query_nodeT **) malloc(TABLE_SIZE * sizeof(query_nodeT));
        if (query_table->nodes == NULL) {
            return NULL;
        }
    }

    return query_table;
}

query_nodeT *create_query_node(queryT *query) {
    query_nodeT *query_node = (query_nodeT *) malloc(sizeof(query_nodeT));
    if (query_node != NULL) {
        query_node->query = query;
        query_node->next = NULL;
    }
    return query_node;
}

int hash(char c) {
    return (int) c % TABLE_SIZE; 
}

int query_table_insert(queryT *query, query_tableT *table) {
    int query_hash = hash(query->resource);
    if (table->nodes[query_hash] == NULL) {
        query_nodeT *query_node = create_query_node(query);
        if (query_node != NULL) {
            table->nodes[query_hash] = query_node;
        } else {
            return -1;
        }
    } else {

    }

    return -1;
}

int destroy_query_table(query_tableT *table) {
    return -1;
}
