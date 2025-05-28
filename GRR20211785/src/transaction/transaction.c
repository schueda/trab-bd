#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "transaction.h"

transaction_tableT *create_transaction_table() {
    transaction_tableT *table = (transaction_tableT *) malloc(sizeof(table));
    if (table != NULL) {
        table->transactions = (transactionT **) malloc(TRANSACTION_TABLE_SIZE * sizeof(transactionT));
        if (table->transactions == NULL) { 
            free(table);
            return NULL; 
        }

        table->open_count = 0;        
    }
    return table;
}

transactionT *create_transaction(int id) {
    transactionT *transaction = (transactionT *) malloc(sizeof(transactionT));
    if (transaction == NULL) { return NULL; }

    transaction->id = id;
    transaction->open = 1;
    return transaction;
}

int t_hash(int id) {
    return id % TRANSACTION_TABLE_SIZE;
}

int open_transaction(int id, transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL) { return -1; }

    for (int inc = 0; inc < TRANSACTION_TABLE_SIZE; inc++) {
        int transaction_hash = t_hash(id + inc);
        if (table->transactions[transaction_hash] == NULL) {
            transactionT *transaction = create_transaction(id);
            if (transaction == NULL) { return -1; }
            
            table->transactions[transaction_hash] = transaction;
            table->open_count++;
            return 0;
        } else if (table->transactions[transaction_hash]->id == id) {
            return 1;
        } 
    }
    printf("Tabela de transações cheia. Não foi possível registrar a transação T%d.\n", id);
    return -1;
}

int commit_transaction(int id, transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL) { return -1; }

    for (int inc = 0; inc < TRANSACTION_TABLE_SIZE; inc++) {
        int transaction_hash = t_hash(id + inc);
        if (table->transactions[transaction_hash] == NULL) { break; }
        if (table->transactions[transaction_hash]->id == id) {
            if (table->transactions[transaction_hash]->open == 1) {
                table->transactions[transaction_hash]->open = 0;
                table->open_count--;
                return 1;
            } else {
                break;
            }
        }
    }    
    printf("Commit em T%d sem operações\n", id);
    return -1;
}

int compare_ints(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;
    
    return *ia - *ib;
}

int list_transactions(char *str, transaction_tableT *table) {
    if (str == NULL || table == NULL || table->transactions == NULL) { return -1; }

    int transactions[TRANSACTION_TABLE_SIZE];
    int j = 0;
    for (int i = 0; i < TRANSACTION_TABLE_SIZE; i++) {
        if (table->transactions[i] != NULL) {
            transactions[j++] = table->transactions[i]->id;
        }
    }
    qsort(transactions, j, sizeof(int), compare_ints);

    for (int i = 0; i < j; i++) {
        char number[8] = "";
        sprintf(number, "%d,", transactions[i]);
        strlcat(str, number, TRANSACTION_LIST_BUFFER);
    }

    str[strlen(str)-1] = '\0';

    return 1;
}

int empty_transaction_table(transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL) { return -1; }

    for (int i = 0; i < TRANSACTION_TABLE_SIZE; i++) {
        if (table->transactions[i] !=  NULL) {
            free(table->transactions[i]);
            table->transactions[i] = NULL;
        }
    }

    table->open_count = 0;
    return 0;
}

int destroy_transaction_table(transaction_tableT *table) {
    empty_transaction_table(table);
    return -1;
}
