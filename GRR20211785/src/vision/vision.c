#include <stdio.h>
#include <stdlib.h>
#include "vision.h"

int vision_query_table_insert(queryT *query, vision_query_tableT *table);
int empty_w_timestamps(w_timestampT **w_timestamps);

int r_hash(char resource) {
    return (int) resource % VISION_RESOURCE_TABLE_SIZE;
}

w_timestampT *create_w_timestamp(char resource, int timestamp) {
    w_timestampT *w_timestamp = (w_timestampT *) malloc(sizeof(w_timestamp));
    w_timestamp->resource = resource;
    w_timestamp->timestamp = timestamp;
    return w_timestamp;
}

int w_timestamps_insert(queryT *query, w_timestampT **w_timestamps) {
    if (query == NULL || w_timestamps == NULL) return -1;

    for (int inc = 0; inc < VISION_RESOURCE_TABLE_SIZE; inc++) {
        int resource_hash = r_hash(query->resource + inc);
        if (w_timestamps[resource_hash] == NULL) {
            w_timestamps[resource_hash] = create_w_timestamp(query->resource, query->timestamp);
            return 0;
        }
        if (w_timestamps[resource_hash]->resource == query->resource) {
            w_timestamps[resource_hash]->timestamp = query->timestamp;
            return 1;
        }
    }

    perror("Tabela query de últimas escritas cheia.");
    return -1; 
}


int vision_process_query(queryT *query, vision_query_tableT *table) {
    if (query == NULL) return -1;

    vision_query_table_insert(query, table);

    if (query->operation == WRITE)
        w_timestamps_insert(query, table->w_timestamps);

    return 0;
}

void swap(query_nodeT **n1, query_nodeT **n2) {
    query_nodeT *aux = *n2;
    *n2 = *n1;
    *n1 = aux;
}

int get_timestamp(char resource, w_timestampT **w_timestamps) {
    if (w_timestamps == NULL) return -1;

    for (int inc = 0; inc < VISION_RESOURCE_TABLE_SIZE; inc++) {
        int resource_hash = r_hash(resource + inc);
        if (w_timestamps[resource_hash] == NULL) return -1;

        if (w_timestamps[resource_hash]->resource == resource) 
            return w_timestamps[resource_hash]->timestamp;
    }

    return -1;
}

int serial_equivalent(query_nodeT **transactions, vision_query_tableT *table) {
    int n = table->number_of_transactions;
 
    w_timestampT **w_timestamps = (w_timestampT **) malloc(VISION_RESOURCE_TABLE_SIZE * sizeof(w_timestampT *));
    for (int i = 0; i < VISION_RESOURCE_TABLE_SIZE; i++) {
        query_nodeT *node_i = table->nodes[i];
        while (node_i != NULL) {
            if (node_i->query->operation == WRITE) {
                w_timestamps_insert(node_i->query, w_timestamps);
            }
            node_i = node_i->next;
        }
    }

    for (int i = 0; i < VISION_RESOURCE_TABLE_SIZE; i++) {
        if (w_timestamps[i] != NULL) {
            int og_timestamp = get_timestamp(w_timestamps[i]->resource, table->w_timestamps);
            if (og_timestamp != w_timestamps[i]->timestamp) {
                return 0;
            }
        }
    }

    empty_w_timestamps(w_timestamps);
    free(w_timestamps);

    for (int i = 0; i < n-1; i++) {
        for (int j = i + 1; j < n; j++) {
            query_nodeT *node_a = transactions[i];
            while (node_a != NULL) {
                query_nodeT *node_b = transactions[j];
                while (node_b != NULL) {
                    if (node_a->query->resource == node_b->query->resource)
                        if ((node_a->query->operation == READ && node_b->query->operation == WRITE) ||
                            (node_a->query->operation == WRITE && node_b->query->operation == READ))
                                if (node_a->query->timestamp > node_b->query->timestamp)
                                    return 0;
                    
                    node_b = node_b->next;
                }
                node_a = node_a->next;
            }
        }
    }

    return 1;
}

int permute(query_nodeT **transactions, int n, vision_query_tableT *table) {
    if (n == 1) {
        return serial_equivalent(transactions, table);
    }
    
    for (int i = 0; i < n; i++) {
        swap(&transactions[i], &transactions[n-1]);
        if (permute(transactions, n-1, table)) {
            return 1;
        }
        swap(&transactions[i], &transactions[n-1]);
    }

    return 0;
}

int check_equivalent_vision(vision_query_tableT *table) {
    if (table == NULL || table->nodes == NULL) return -1;
    if (table->number_of_transactions == 0) return 1;

    query_nodeT **transactions = (query_nodeT **) malloc(table->number_of_transactions * sizeof(query_nodeT *));
    int t_pos = 0;
    for (int i = 0; i < VISION_QUERY_TABLE_SIZE; i ++) {
        if (table->nodes[i] != NULL)
            transactions[t_pos++] = table->nodes[i];
    }

    int result = permute(transactions, table->number_of_transactions, table);
    free(transactions);
    return result;
}

vision_query_tableT *create_vision_query_table() {
    vision_query_tableT *table = (vision_query_tableT *) malloc(sizeof(vision_query_tableT));
    if (table == NULL) return NULL;
    
    table->nodes = (query_nodeT **) malloc(VISION_QUERY_TABLE_SIZE * sizeof(query_nodeT *));
    if (table->nodes == NULL) {
        free(table);
        return NULL;
    }
    table->w_timestamps = (w_timestampT **) malloc(VISION_RESOURCE_TABLE_SIZE * sizeof(w_timestampT *));
    if (table->w_timestamps == NULL) {
        free(table->nodes);
        free(table);
        return NULL;
    }

    table->number_of_transactions = 0;
    return table;
}

int v_hash(int id) {
    return id % VISION_QUERY_TABLE_SIZE;
}

int vision_query_table_insert(queryT *query, vision_query_tableT *table) {
    if (query == NULL || table == NULL || table->nodes == NULL) return -1;

    for (int inc = 0; inc < VISION_QUERY_TABLE_SIZE; inc++) {
        int query_hash = v_hash(query->transaction_id + inc);
        if (table->nodes[query_hash] == NULL) {
            query_nodeT *query_node = create_query_node(query);
            if (query_node == NULL) return -1;
    
            table->nodes[query_hash] = query_node;
            table->number_of_transactions++;
            return 0;
        }  else if (table->nodes[query_hash]->query->transaction_id == query->transaction_id) {
            query_nodeT *node = table->nodes[query_hash];
            while(node->next != NULL) 
                node = node->next;
            node->next = create_query_node(query);
            return 1;
        }
    }

    perror("Tabela query de visão cheia.");
    return -1; 
}

int destroy_w_timestamp(w_timestampT *w_timestamp) {
    if (w_timestamp == NULL) return -1;
    
    free(w_timestamp);
    return 0;
}

int empty_w_timestamps(w_timestampT **w_timestamps) {
    if (w_timestamps == NULL) return -1;

    for (int i = 0; i < VISION_RESOURCE_TABLE_SIZE; i++) {
        if(w_timestamps[i] != NULL) {
            destroy_w_timestamp(w_timestamps[i]);
            w_timestamps[i] = NULL;
        }
    }

    return 0;
}

int empty_vision_query_table(vision_query_tableT *table) {
    if (table == NULL || table->nodes == NULL || table->w_timestamps == NULL) return -1; 
    
    for (int i = 0; i < VISION_QUERY_TABLE_SIZE; i++) {
        query_nodeT *node = table->nodes[i];
        while (node != NULL) {
            query_nodeT *next_node = node->next;
            destroy_query_node(node);
            node = next_node;
        }
        table->nodes[i] = NULL;
    }

    empty_w_timestamps(table->w_timestamps);

    table->number_of_transactions = 0;

    return 0;
}

int destroy_vision_query_table(vision_query_tableT *table) {
    if (table == NULL) return -1;

    empty_vision_query_table(table);

    if (table->nodes != NULL) {
        free(table->nodes);
    }

    if (table->w_timestamps != NULL) {
        free(table->w_timestamps);
    }

    free(table);
    return 0;
}
