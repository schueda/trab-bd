#ifndef __VISION__
#define __VISION__

#include "../query/query.h"

#define VISION_QUERY_TABLE_SIZE 16
#define VISION_RESOURCE_TABLE_SIZE 16

/**
 * @brief Estrutura que representa um timestamp de escrita
 */
typedef struct w_timestamp w_timestampT;
struct w_timestamp {
    char resource; /**< Recurso associado ao timestamp */
    int timestamp; /**< Timestamp da operação de escrita */
};

/**
 * @brief Estrutura que representa uma tabela de queries para verificação de equivalência por visão
 */
typedef struct vision_query_table vision_query_tableT;
struct vision_query_table {
    query_nodeT **nodes;         /**< Array de nós de query */
    int number_of_transactions;  /**< Número de transações na tabela */
    w_timestampT **w_timestamps; /**< Array de timestamps de escrita */
};

/**
 * @brief Processa uma query na tabela de visão
 *
 * @param query Query a ser processada
 * @param table Tabela de visão
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int vision_process_query(queryT *query, vision_query_tableT *table);

/**
 * @brief Verifica se o escalonamento é equivalente por visão
 *
 * @param table Tabela de visão
 * @return int 1 se for equivalente, 0 se não for, -1 em caso de erro
 */
int check_equivalent_vision(vision_query_tableT *table);

/**
 * @brief Cria uma nova tabela de visão
 *
 * @return vision_query_tableT* Ponteiro para a tabela criada, ou NULL em caso de erro
 */
vision_query_tableT *create_vision_query_table();

/**
 * @brief Esvazia a tabela de visão
 *
 * @param table Tabela a ser esvaziada
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_vision_query_table(vision_query_tableT *table);

/**
 * @brief Destrói a tabela de visão
 *
 * @param table Tabela a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_vision_query_table(vision_query_tableT *table);

#endif