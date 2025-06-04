#ifndef __TRANSACTION__
#define __TRANSACTION__

/** @brief Tamanho da tabela hash de transações */
#define TRANSACTION_TABLE_SIZE 16
/** @brief Tamanho do buffer para lista de transações */
#define TRANSACTION_LIST_BUFFER 64

/** @brief Estrutura que representa uma transação */
typedef struct transaction transactionT;
struct transaction {
    int id;   /**< ID da transação */
    int open; /**< Flag indicando se a transação está aberta */
};

/** @brief Estrutura que representa uma tabela de transações */
typedef struct transaction_table transaction_tableT;
struct transaction_table {
    transactionT **transactions; /**< Array de ponteiros para transações */
    int open_count;              /**< Número de transações abertas */
};

/**
 * @brief Cria uma nova tabela de transações
 *
 * @return transaction_tableT* Ponteiro para a tabela criada, ou NULL em caso de erro
 */
transaction_tableT *create_transaction_table();

/**
 * @brief Abre uma nova transação
 *
 * @param id ID da transação a ser aberta
 * @param table Ponteiro para a tabela de transações
 * @return int 0 se a transação foi aberta, 1 se já existia, -1 em caso de erro
 */
int open_transaction(int id, transaction_tableT *table);

/**
 * @brief Realiza o commit de uma transação
 *
 * @param id ID da transação a ser commitada
 * @param table Ponteiro para a tabela de transações
 * @return int 1 se o commit foi realizado, -1 em caso de erro
 */
int commit_transaction(int id, transaction_tableT *table);

/**
 * @brief Lista as transações em ordem crescente de ID
 *
 * @param str Buffer para armazenar a lista de transações
 * @param table Ponteiro para a tabela de transações
 * @return int 1 em caso de sucesso, -1 em caso de erro
 */
int list_transactions(char *str, transaction_tableT *table);

/**
 * @brief Esvazia a tabela de transações
 *
 * @param table Ponteiro para a tabela a ser esvaziada
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_transaction_table(transaction_tableT *table);

/**
 * @brief Destrói a tabela de transações
 *
 * @param table Ponteiro para a tabela a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_transaction_table(transaction_tableT *table);

#endif