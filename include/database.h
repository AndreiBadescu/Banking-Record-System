#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED

#include <stdlib.h>

typedef unsigned int uint;

struct account{
    uint id;
    long long curr_sum;
    /// created = date when the account was created
    char *name,*firstname,currency[3],pin[4],created[8];
};

struct HT_node{
    struct account *acc;
    struct HT_node *next;
};

struct HT_list{
    struct HT_node *head,*tail;
}List_ids;

struct HashTable{
    struct HT_list *buckets;
    uint size,count;
}DATABASE;

/* List_ids */
void getID(struct account *acc);
void deleteID(uint ID);

/* DATABASE */
void getAccounts(struct HashTable *DATABASE);
void update_DB(const struct HT_list *List_acc);
struct HT_node* create_node(struct account *ptr_acc);
void HT_list_insert(struct HT_list *list, struct account *ptr_acc);
void free_account(struct account *acc);
void free_HT_node(struct HT_node *node);
void free_HT_list(struct HT_list *list);
void initialize_DB(const uint size);
void free_DB(void);
void DB_insert(const uint key, struct account *ptr_acc);
struct account* DB_search(const uint key);
void HT_delete(const uint key);


#endif // DATABASE_H_INCLUDED
