#ifndef __VISION__
#define __VISION__

#include "../query/query.h"

#define VISION_QUERY_TABLE_SIZE 16

typedef struct vision_query_table vision_query_tableT;
struct vision_query_table {
    query_nodeT **nodes;
    int number_of_transactions;
    int last_write_timestamp;
};

int vision_process_query(queryT *query, vision_query_tableT *table);
int check_equivalent_vision(vision_query_tableT *table);

vision_query_tableT *create_vision_query_table();
int empty_vision_query_table(vision_query_tableT *table);
int destroy_vision_query_table(vision_query_tableT *table);


#endif