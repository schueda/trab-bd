#include <stdio.h>
#include <stdlib.h>
#include "query.h"

queryT *create_query(char *buffer) {
    queryT *query = (queryT *) malloc(sizeof(queryT));
    if (query == NULL) { return NULL; }

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

int destroy_query(queryT *query) {
    if (query == NULL) { return -1; }
    free(query);
    query = NULL; 
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

    if (query_table == NULL) { return NULL; }
    query_table->nodes = (query_nodeT **) malloc(QUERY_TABLE_SIZE * sizeof(query_nodeT));
    if (query_table->nodes == NULL) {
        free(query_table);
        return NULL;
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

int q_hash(char c) {
    return (int) c % QUERY_TABLE_SIZE; 
}

int compare(queryT *qi, queryT *qj, conflictsT *conflicts) {
    if (qi->transaction_id == qj->transaction_id) { return 0; }

    if (qj->operation == WRITE || (qj->operation == READ && qi->operation == WRITE)) {
        if (conflicts->count >= CONFLICTS_MAX_SIZE) { return -1; }
        
        conflicts->transactions[conflicts->count] = qi->transaction_id;
        conflicts->count++;
        return 1;
    }
    return 0;
}

int query_table_insert(queryT *query, query_tableT *table, conflictsT *conflicts) {
    if (query == NULL || table == NULL || table->nodes == NULL || conflicts == NULL || conflicts->transactions == NULL) { return -1; }

    for (int inc = 0; inc < QUERY_TABLE_SIZE; inc++) {
        int query_hash = q_hash(query->resource + inc);
        if (table->nodes[query_hash] == NULL) {
            query_nodeT *query_node = create_query_node(query);
            if (query_node == NULL) { return -1; }
    
            table->nodes[query_hash] = query_node;

            table->new_entries = 1;
            return 0;
        } else if (table->nodes[query_hash]->query->resource == query->resource) {
            query_nodeT *node = table->nodes[query_hash];
            while(node->next != NULL) {
                compare(node->query, query, conflicts);
                node = node->next;
            }

            compare(node->query, query, conflicts);
            node->next = create_query_node(query);

            table->new_entries = 1;
            return 1;
        } 
    }

    perror("Tabela de Query cheia.");
    return -1; 
}

int empty_query_table(query_tableT *table) {
    table->new_entries = 0;
    return -1;
}

int destroy_query_table(query_tableT *table) {
    return -1;
}
