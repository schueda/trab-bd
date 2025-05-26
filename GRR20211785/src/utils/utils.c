#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

queryT *make_query(char *buffer) {
    queryT *query = (queryT *) malloc(sizeof(queryT));
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