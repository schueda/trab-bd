#ifndef __UTILS__
#define __UTILS__

typedef enum query_op operationT; 
enum query_op {
    READ = 'R',
    WRITE = 'W',
    COMMIT = 'C'
};

typedef struct query_type queryT;
struct query_type {
    int timestamp;
    int transaction_id;
    operationT operation;
    char resource;
};

queryT *make_query(char *buffer);
void print_query(queryT *query);
int destroy_query(queryT *query);

#endif