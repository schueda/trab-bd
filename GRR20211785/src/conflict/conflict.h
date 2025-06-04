#ifndef __CONFLICT__
#define __CONFLICT__

#include "../graph/graph.h"

typedef struct query queryT;
typedef struct query_node query_nodeT;

/** @brief Tamanho máximo da lista de conflitos */
#define CONFLICTS_MAX_SIZE 16
/** @brief Tamanho da tabela hash de operações de conflito */
#define CONFLICT_QUERY_TABLE_SIZE 16

/** @brief Estrutura que representa uma lista de conflitos */
typedef struct conflicts conflictsT;
struct conflicts {
    int *transactions; /**< Array de IDs de transações em conflito */
    int count;         /**< Número de transações em conflito */
};

/** @brief Estrutura que representa uma tabela hash de operações de conflito */
typedef struct conflict_query_table conflict_query_tableT;
struct conflict_query_table {
    query_nodeT **nodes; /**< Array de ponteiros para nós de operação */
};

/**
 * @brief Processa uma operação para detecção de conflitos
 *
 * @param query Ponteiro para a operação a ser processada
 * @param graph Ponteiro para o grafo de conflitos
 * @param query_table Ponteiro para a tabela de operações de conflito
 * @param conflicts Ponteiro para a lista de conflitos
 */
void conflict_process_query(queryT *query, graphT *graph, conflict_query_tableT *query_table, conflictsT *conflicts);

/**
 * @brief Cria uma nova lista de conflitos
 *
 * @return conflictsT* Ponteiro para a lista criada, ou NULL em caso de erro
 */
conflictsT *create_conflicts();

/**
 * @brief Destrói uma lista de conflitos
 *
 * @param conflicts Ponteiro para a lista a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_conflicts(conflictsT *conflicts);

/**
 * @brief Cria uma nova tabela de operações de conflito
 *
 * @return conflict_query_tableT* Ponteiro para a tabela criada, ou NULL em caso de erro
 */
conflict_query_tableT *create_conflict_query_table();

/**
 * @brief Esvazia uma tabela de operações de conflito
 *
 * @param table Ponteiro para a tabela a ser esvaziada
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_conflict_query_table(conflict_query_tableT *table);

/**
 * @brief Destrói uma tabela de operações de conflito
 *
 * @param table Ponteiro para a tabela a ser destruída
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_conflict_query_table(conflict_query_tableT *table);

#endif