#ifndef __QUERY__
#define __QUERY__

#include "../conflict/conflict.h"


/** @brief Enumeração dos tipos de operações */
typedef enum query_op operationT;
enum query_op {
    READ = 'R',  /**< Operação de leitura */
    WRITE = 'W', /**< Operação de escrita */
    COMMIT = 'C' /**< Operação de commit */
};

/** @brief Estrutura que representa uma operação */
typedef struct query queryT;
struct query {
    int timestamp;        /**< Timestamp da operação */
    int transaction_id;   /**< ID da transação */
    operationT operation; /**< Tipo de operação */
    char resource;        /**< Recurso acessado */
};

/** @brief Estrutura para nó de lista encadeada de operações */
typedef struct query_node query_nodeT;
struct query_node {
    queryT *query;     /**< Ponteiro para a operação */
    query_nodeT *next; /**< Ponteiro para o próximo nó */
};

/**
 * @brief Cria uma nova operação a partir de um buffer de entrada
 *
 * @param buffer String contendo os dados da operação
 * @return queryT* Ponteiro para a operação criada, ou NULL em caso de erro
 */
queryT *create_query(char *buffer);

/**
 * @brief Imprime os dados de uma operação
 *
 * @param query Ponteiro para a operação a ser impressa
 */
void print_query(queryT *query);

/**
 * @brief Destrói uma operação, liberando sua memória
 *
 * @param query Ponteiro para a operação a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_query(queryT *query);

/**
 * @brief Cria um novo nó de lista encadeada para uma operação
 *
 * @param query Ponteiro para a operação a ser armazenada no nó
 * @return query_nodeT* Ponteiro para o nó criado, ou NULL em caso de erro
 */
query_nodeT *create_query_node(queryT *query);

/**
 * @brief Destrói um nó de lista encadeada, liberando sua memória
 *
 * @param node Ponteiro para o nó a ser destruído
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_query_node(query_nodeT *node);

#endif