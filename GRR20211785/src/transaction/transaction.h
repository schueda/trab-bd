#ifndef __TRANSACTION__
#define __TRANSACTION__

#define TRANSACTION_TABLE_SIZE 16
#define TRANSACTION_LIST_BUFFER 64

typedef struct transaction transactionT;
struct transaction {
    int id;
    int open;
};

typedef struct transaction_table transaction_tableT;
struct transaction_table {
    transactionT **transactions;
    int open_count;
};

transaction_tableT *create_transaction_table();
int open_transaction(int id, transaction_tableT *table);
int commit_transaction(int id, transaction_tableT *table);
int list_transactions(char *str, transaction_tableT *table);
int empty_transaction_table(transaction_tableT *table);
int destroy_transaction_table(transaction_tableT *table);

#endif