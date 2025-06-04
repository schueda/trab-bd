#include "transaction.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma nova tabela de transações
 *
 * @return transaction_tableT* Ponteiro para a tabela criada, ou NULL em caso de erro
 */
transaction_tableT *create_transaction_table() {
    transaction_tableT *table = (transaction_tableT *)malloc(sizeof(transaction_tableT));
    if (table != NULL) {
        table->transactions = (transactionT **)malloc(TRANSACTION_TABLE_SIZE * sizeof(transactionT));
        if (table->transactions == NULL) {
            free(table);
            return NULL;
        }

        table->open_count = 0;
    }
    return table;
}

/**
 * @brief Cria uma nova transação
 *
 * @param id ID da transação
 * @return transactionT* Ponteiro para a transação criada, ou NULL em caso de erro
 */
transactionT *create_transaction(int id) {
    transactionT *transaction = (transactionT *)malloc(sizeof(transactionT));
    if (transaction == NULL)
        return NULL;

    transaction->id = id;
    transaction->open = 1;
    return transaction;
}

/**
 * @brief Calcula o hash de um ID para a tabela de transações
 *
 * @param id ID da transação
 * @return int Índice na tabela hash
 */
int t_hash(int id) {
    return id % TRANSACTION_TABLE_SIZE;
}

/**
 * @brief Abre uma nova transação
 *
 * @param id ID da transação a ser aberta
 * @param table Ponteiro para a tabela de transações
 * @return int 0 se a transação foi aberta, 1 se já existia, -1 em caso de erro
 */
int open_transaction(int id, transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL)
        return -1;

    for (int inc = 0; inc < TRANSACTION_TABLE_SIZE; inc++) {
        int transaction_hash = t_hash(id + inc);
        if (table->transactions[transaction_hash] == NULL) {
            transactionT *transaction = create_transaction(id);
            if (transaction == NULL)
                return -1;

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

/**
 * @brief Realiza o commit de uma transação
 *
 * @param id ID da transação a ser commitada
 * @param table Ponteiro para a tabela de transações
 * @return int 1 se o commit foi realizado, -1 em caso de erro
 */
int commit_transaction(int id, transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL)
        return -1;

    for (int inc = 0; inc < TRANSACTION_TABLE_SIZE; inc++) {
        int transaction_hash = t_hash(id + inc);
        transactionT *transaction = table->transactions[transaction_hash];
        if (transaction == NULL)
            break;

        if (transaction->id == id) {
            if (transaction->open == 1) {
                transaction->open = 0;
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

/**
 * @brief Função de comparação para ordenação de IDs
 *
 * @param a Ponteiro para o primeiro ID
 * @param b Ponteiro para o segundo ID
 * @return int Diferença entre os IDs
 */
int compare_ints(const void *a, const void *b) {
    const int *ia = (const int *)a;
    const int *ib = (const int *)b;

    return *ia - *ib;
}

/**
 * @brief Lista as transações em ordem crescente de ID
 *
 * @param str Buffer para armazenar a lista de transações
 * @param table Ponteiro para a tabela de transações
 * @return int 1 em caso de sucesso, -1 em caso de erro
 */
int list_transactions(char *str, transaction_tableT *table) {
    if (str == NULL || table == NULL || table->transactions == NULL)
        return -1;

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
        strncat(str, number, TRANSACTION_LIST_BUFFER);
    }

    str[strlen(str) - 1] = '\0';

    return 1;
}

/**
 * @brief Esvazia a tabela de transações
 *
 * @param table Ponteiro para a tabela a ser esvaziada
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_transaction_table(transaction_tableT *table) {
    if (table == NULL || table->transactions == NULL)
        return -1;

    for (int i = 0; i < TRANSACTION_TABLE_SIZE; i++) {
        if (table->transactions[i] != NULL) {
            free(table->transactions[i]);
            table->transactions[i] = NULL;
        }
    }

    table->open_count = 0;
    return 0;
}

/**
 * @brief Destrói a tabela de transações
 *
 * @param table Ponteiro para a tabela a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_transaction_table(transaction_tableT *table) {
    if (table == NULL)
        return -1;

    empty_transaction_table(table);

    if (table->transactions != NULL) {
        free(table->transactions);
    }

    free(table);

    return -1;
}
