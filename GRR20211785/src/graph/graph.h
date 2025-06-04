#ifndef __GRAPH__
#define __GRAPH__

typedef struct edge edgeT;
typedef struct vertex vertexT;
typedef struct graphS graphT;

/** @brief Número máximo de vértices no grafo */
#define MAX_VERTICES 16

/** @brief Estrutura que representa uma aresta do grafo */
struct edge {
    vertexT *to_vertex; /**< Ponteiro para o vértice de destino */
    edgeT *next;        /**< Ponteiro para a próxima aresta */
};

/** @brief Estrutura que representa um vértice do grafo */
struct vertex {
    int value;       /**< Valor do vértice (ID da transação) */
    edgeT *frontier; /**< Lista de arestas saindo do vértice */
    int visited;     /**< Flag de visitação para DFS */
};

/** @brief Estrutura que representa o grafo */
struct graphS {
    vertexT **vertices; /**< Array de ponteiros para vértices */
};

/**
 * @brief Cria um novo grafo vazio
 *
 * @return graphT* Ponteiro para o grafo criado, ou NULL em caso de erro
 */
graphT *create_graph();

/**
 * @brief Obtém ou cria um vértice com o valor especificado
 *
 * @param graph Ponteiro para o grafo
 * @param value Valor do vértice a ser obtido/criado
 * @return vertexT* Ponteiro para o vértice, ou NULL em caso de erro
 */
vertexT *get_vertex(graphT *graph, int value);

/**
 * @brief Adiciona uma aresta ao grafo
 *
 * @param graph Ponteiro para o grafo
 * @param origin_vertex Ponteiro para o vértice de origem
 * @param dest_vertex Ponteiro para o vértice de destino
 * @return int 1 se a aresta foi adicionada, 0 se já existia, -1 em caso de erro
 */
int add_edge(graphT *graph, vertexT *origin_vertex, vertexT *dest_vertex);

/**
 * @brief Imprime o grafo no formato texto
 *
 * @param graph Ponteiro para o grafo a ser impresso
 */
void print_graph(graphT *graph);

/**
 * @brief Verifica se o grafo possui ciclos
 *
 * @param graph Ponteiro para o grafo
 * @return int 1 se houver ciclos, 0 se não houver, -1 em caso de erro
 */
int check_for_cycles(graphT *graph);

/**
 * @brief Esvazia o grafo, removendo todos os vértices e arestas
 *
 * @param graph Ponteiro para o grafo a ser esvaziado
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int empty_graph(graphT *graph);

/**
 * @brief Destrói o grafo, liberando toda a memória alocada
 *
 * @param graph Ponteiro para o grafo a ser destruído
 * @return int 0 em caso de sucesso, -1 em caso de erro
 */
int destroy_graph(graphT *graph);

#endif