#ifndef __VISION__
#define __VISION__

#include "../query/query.h"



typedef struct vision_transaction vision_transactionT;
struct vision_transaction {
    int id;
    query_nodeT *query_list;
};

typedef struct vision_table vision_tableT;
struct vision_table {
    vision_transactionT **transactions;
};

vision_tableT *create_vision_table()

#endif