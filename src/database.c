#include "../include/database.h"

#include <stdio.h>
#include <stdlib.h>
#include "../include/errors.h"
#include "../include/utilities.h"
#include "../include/input_read.h"

#define MAX_STR 64
#define LINES_ACC 6
#define HT_CAPACITY 5

void getID(struct account *acc){
    struct HT_node *new_node = (struct HT_node*) malloc(sizeof(struct HT_node));
    new_node->acc = acc;

    if(DATABASE.count == 0){
        acc->id = 0;
        new_node->next = NULL;
        List_ids.head = new_node;
        List_ids.tail = new_node;
        return;
    }

    if(List_ids.tail->acc->id + 1 == DATABASE.count){
        acc->id = DATABASE.count;
        new_node->next = NULL;
        List_ids.tail->next = new_node;
        List_ids.tail = new_node;
        return;
    }

    if(List_ids.head->acc->id > 0){
        acc->id = 0;
        new_node->next = List_ids.head;
        List_ids.head = new_node;
        return;
    }

    if(DATABASE.count <= 1)
        DatabaseError(-1);

    struct HT_node *p = List_ids.head;
    while(p->acc->id + 1 == p->next->acc->id){
        p = p->next;
    }

    acc->id = p->acc->id + 1;
    new_node->next = p->next;
    p->next = new_node;
}

void deleteID(uint ID){
    if(ID == List_ids.head->acc->id){
        struct HT_node *tmp = List_ids.head;
        List_ids.head = List_ids.head->next;
        free(tmp);
        return;
    }

    struct HT_node *prev = List_ids.head, *curr = prev->next;
    while(curr){
        if(curr->acc->id == ID){
            prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

void getAccounts(struct HashTable *DATABASE){
    FILE *file = fopen("./files/conturi.txt", "r");
    if(file == NULL)
        DatabaseError(0);

    uint database_size = 0;

    size_t buffer_size = MAX_STR;
    char buffer[buffer_size];

    while(fgets(buffer, buffer_size, file)){
        if(buffer[0] != '\n')
            ++database_size;
    }

    if(database_size % LINES_ACC != 0)
        DatabaseError(1);

    initialize_DB(HT_CAPACITY);
    List_ids.head = NULL, List_ids.tail = NULL;

    if(DATABASE->buckets == NULL)
        DatabaseError(2);

    fseek(file, 0, SEEK_SET);

    /* big_ind = ind / LINES_ACC */
    /* small_ind = ind % LINES_ACC; */
    uint big_ind = 0, small_ind = 0;

    /* current account I'm storing in memory */
    struct account *curr_account = (struct account*) malloc(sizeof(struct account));
    while(fgets(buffer, buffer_size, file)){
        if(buffer[0] != '\n'){
            switch(small_ind){
                case 0:
                    if(convert_to_upper(buffer))
                        DatabaseError(3);

                    curr_account->name = (char*) malloc(strlen(buffer));
                    strcpy(curr_account->name, buffer);
                    break;

                case 1:
                    if(convert_to_upper(buffer))
                        DatabaseError(4);

                    curr_account->firstname = (char*) malloc(strlen(buffer));
                    strcpy(curr_account->firstname, buffer);
                    break;

                case 2:
                    if(strlen(buffer) != 4 || convert_to_upper(buffer))
                        DatabaseError(5);

                    strncpy(curr_account->currency, buffer, 3);
                    break;

                case 3:
                    /* buffer[i+1] because last element is new_line always */
                    for(uint i = (buffer[0] == '-') ? 1 : 0; buffer[i+1]; ++i){
                        if(buffer[i] < '0' || buffer[i] > '9')
                            DatabaseError(6);
                    }

                    curr_account->curr_sum = strtol(buffer, NULL, 10);
                    break;

                case 4:
                    if(strlen(buffer) != 5)
                        DatabaseError(7);

                    for(uint i = 0; i < 4; ++i){
                        if(buffer[i] < '0' || buffer[i] > '9')
                            DatabaseError(7);
                    }

                    strncpy(curr_account->pin, buffer, 4);
                    break;

                case 5:
                    if(strlen(buffer) != 9)
                        DatabaseError(8);

                    for(uint i = 0; i < 8; ++i){
                        if(buffer[i] < '0' || buffer[i] > '9')
                            DatabaseError(8);
                    }

                    strncpy(curr_account->created, buffer, 8);
                    break;

                default:
                    DatabaseError(-1);
            }

            ++small_ind;
            if(small_ind == LINES_ACC){
                getID(curr_account);
                DB_insert(curr_account->id, curr_account);
                curr_account = (struct account*) malloc(sizeof(struct account));
                ++big_ind;
                small_ind = 0;
            }
        }
    }

    fclose(file);
}

void update_DB(const struct HT_list *List_acc){
    FILE *tmp = fopen("./files/tmp.txt", "w");
    if(tmp == NULL)
        DatabaseError(0);

    for(struct HT_node *node = List_acc->head; node; node = node->next){
        write_account(tmp,node->acc);
    }

    fclose(tmp);
    remove("./files/conturi.txt");
    rename("./files/tmp.txt","./files/conturi.txt");
}

void initialize_DB(const uint size){
    DATABASE.size = size;
    DATABASE.count = 0;
    DATABASE.buckets = (struct HT_list*) calloc(size,sizeof(struct HT_list));
}

struct HT_node* create_node(struct account *ptr_acc){
    struct HT_node *node = (struct HT_node*) malloc(sizeof(struct HT_node));
    node->acc = ptr_acc;
    node->next = NULL;
    return node;
}

void HT_list_insert(struct HT_list *list, struct account *ptr_acc){
    struct HT_node *new_node = create_node(ptr_acc);

    if(list->head == NULL){
        list->head = new_node;
        list->tail = new_node;
        return;
    }

    list->tail->next = new_node;
    list->tail = new_node;
}

void free_account(struct account *acc){
    free(acc->name);
    free(acc->firstname);
    free(acc);
}

void free_HT_node(struct HT_node *node){
    free_account(node->acc);
    free(node);
}

void free_HT_list(struct HT_list *list){
    list->tail = NULL;
    while(list->head) {
        struct HT_node *tmp = list->head;
        list->head = list->head->next;
        free_HT_node(tmp);
    }
}

void free_DB(void){
    for (int i = 0; i < DATABASE.size; i++) {
        free_HT_list(DATABASE.buckets + i);
    }
}

void DB_insert(const uint key, struct account *ptr_acc){
    /* key = acc->id */
    HT_list_insert(DATABASE.buckets + key % DATABASE.size, ptr_acc);
    ++DATABASE.count;
}

struct account* DB_search(const uint key){
    struct HT_node *node = (DATABASE.buckets + key % DATABASE.size)->head;
    while(node){
        if(node->acc->id == key)
            return node->acc;

        node = node->next;
    }
    return NULL;
}

void HT_delete(const uint key){
    struct HT_node *prev = NULL, *node = (DATABASE.buckets + key % DATABASE.size)->head;
    while(node){
        if(node->acc->id == key){
            --DATABASE.count;
            deleteID(key);
            if(prev){
                prev->next = node->next;
                free_HT_node(node);
            }
            else{
                (DATABASE.buckets + key % DATABASE.size)->head = node->next;
                free_HT_node(node);
            }

            printf("\nCONTUL A FOST STERS CU SUCCES!\n");
            goBack(1);
            return;
        }
        node = node->next;
    }
    getError(2,1);
}
