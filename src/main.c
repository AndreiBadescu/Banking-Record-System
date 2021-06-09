#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include<time.h>
#include "../include/database.h"
#include "../include/input_read.h"
#include "../include/utilities.h"
#include "../include/errors.h"

#define NR_MENUS 3
#define MAX_LEN_ID 8
#define PIN_LEN 4

typedef unsigned int uint;

char *MENU[NR_MENUS];

void search_user(void){
    show_menu(1);
    char option = getch();
    system("cls");

    if(option == '1'){
        /* ERROR CHECKING FOR INPUT */
        int check = read_account_id();
        if(check < 0){
            getError(0,1);
            return;
        }
        struct account *acc = DB_search((uint)check);
        if(acc){
            system("cls");
            print_account(acc);
            goBack(1);
            return;
        }
    }
    else if(option == '2'){
        /* ERROR CHECKING FOR INPUT */
        char *name = (char*) read_name();
        if(name == NULL){
            return;
        }

        /* ERROR CHECKING FOR INPUT */
        char *firstname = read_firstname();
        if(firstname == NULL){
            free(name);
            return;
        }

        uint console_ok = 1;
        for(uint i = 0; i < DATABASE.size; ++i){
            for(struct HT_node *node = DATABASE.buckets[i].head; node; node = node->next){
                if(!strcmp(name,node->acc->name) && !strcmp(firstname,node->acc->firstname)){
                    if(console_ok){
                        system("cls");
                        console_ok = 0;
                    }
                    else{
                        printf("\n");
                    }
                    print_account(node->acc);
                }
            }
        }

        free(name);
        free(firstname);

        if(!console_ok){
            goBack(1);
            return;
        }
    }
    else{
        return;
    }
    getError(2,1);
}

void show_all_users(void){
    system("cls");
    for(struct HT_node *node = List_ids.head; node; node = node->next){
        print_account(node->acc);
        printf("\n");
    }
    goBack(1);
}

void acces_account(void){
    system("cls");

    /* ERROR CHECKING FOR INPUT */
    int searched_id = read_account_id();
    if(searched_id < 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char* account_pin = read_account_pin(0);
    if(account_pin == NULL){
        system("cls");
        getError(3,1);
        return;
    }

    struct account *acc = DB_search((uint)searched_id);
    if(acc){
        /* curr_account->pin needs a null terminator for strcmp */
        char curr_pin[5];
        strncpy(curr_pin, acc->pin, 4);
        curr_pin[4] = '\0';

        if(!strcmp(account_pin,curr_pin)){
            system("cls");
            print_privateInfo_account(acc);
            goBack(1);
        }
        else{
            getError(4,1);
        }
        free(account_pin);
        return;
    }

    free(account_pin);
    getError(5,1);
}

void add_new_account(void){
    system("cls");

    /* ERROR CHECKING FOR INPUT */
    char *name = read_name();
    if(name == NULL)
        return;

    /* ERROR CHECKING FOR INPUT */
    char *firstname = read_firstname();
    if(firstname == NULL){
        free(name);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *currency = read_currency();
    if(currency == NULL){
        free(name);
        free(firstname);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *pin1 = read_account_pin(0);
    if(pin1 == NULL){
        free(name);
        free(firstname);
        free(currency);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *pin2 = read_account_pin(1);
    if(pin2 == NULL){
        free(name);
        free(firstname);
        free(currency);
        free(pin1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    if(strcmp(pin1,pin2)){
        free(name);
        free(firstname);
        free(currency);
        free(pin1);
        free(pin2);
        getError(12,1);
        return;
    }
    free(pin2);

    struct account *acc = (struct account*) malloc(sizeof(struct account));
    getID(acc);
    acc->curr_sum = 0;
    acc->name = name;
    acc->firstname = firstname;
    strncpy(acc->currency,currency,3);
    free(currency);
    strncpy(acc->pin,pin1,4);
    free(pin1);
    time_t t = time(NULL);
    struct tm *local = localtime(&t);
    addDate(&acc->created,local->tm_mday,local->tm_mon,local->tm_year);

    DB_insert(acc->id,acc);

    printf("\nCONTUL A FOST ADAUGAT CU SUCCES!\n");
    goBack(1);
}

void delete_existing_account(void){
    system("cls");
    /* ERROR CHECKING FOR INPUT */
    int searched_id = read_account_id();
    if(searched_id < 0){
        getError(0,1);
        return;
    }
    HT_delete((uint)searched_id);
}

void withdraw(void){
    system("cls");

    /* ERROR CHECKING FOR INPUT */
    int searched_id = read_account_id();
    if(searched_id < 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    long long sum = read_account_sum();
    if(sum == 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *pin = read_account_pin(0);
    if(pin == NULL)
        return;

    struct account *acc = DB_search((uint)searched_id);

    char acc_pin[5];
    strncpy(acc_pin, acc->pin, 4);
    acc_pin[4] = '\0';

    if(!strcmp(pin,acc_pin)){
        if(sum > acc->curr_sum){
            getError(11,1);
            return;
        }
        acc->curr_sum -= sum;
        printf("\nSUMA A FOST RETRASA DIN CONT CU SUCCES!\n");
        goBack(1);
    }
    else{
        getError(4,1);
    }

    free(pin);
}

void deposit(void){
    system("cls");

    /* ERROR CHECKING FOR INPUT */
    int searched_id = read_account_id();
    if(searched_id < 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    long long sum = read_account_sum();
    if(sum == 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *pin = read_account_pin(0);
    if(pin == NULL)
        return;

    /* ERROR CHECKING FOR INPUT */
    struct account *acc = DB_search((uint)searched_id);
    if(acc == NULL){
        getError(2,1);
        return;
    }

    char acc_pin[5];
    strncpy(acc_pin, acc->pin, 4);
    acc_pin[4] = '\0';

    /* ERROR CHECKING FOR INPUT */
    if(!strcmp(pin,acc_pin)){
        acc->curr_sum += sum;
        printf("\nSUMA A FOST DEPUSA IN CONT CU SUCCES!\n");
        goBack(1);
    }
    else{
        getError(4,1);
    }

    free(pin);
}

void modify_account(void){
    system("cls");

    /* ERROR CHECKING FOR INPUT */
    int searched_id = read_account_id();
    if(searched_id < 0){
        getError(0,1);
        return;
    }

    /* ERROR CHECKING FOR INPUT */
    char *pin = read_account_pin(0);
    if(pin == NULL)
        return;

    /* ERROR CHECKING FOR INPUT */
    struct account *acc = DB_search((uint)searched_id);
    if(acc == NULL){
        free(pin);
        getError(2,1);
        return;
    }

    char acc_pin[5];
    strncpy(acc_pin, acc->pin, 4);
    acc_pin[4] = '\0';

    if(!strcmp(pin,acc_pin)){
        show_menu(2);
        char option = getch();
        system("cls");

        switch(option){
            case '1': ;
                char *name = read_name();
                if(name == NULL)
                    return;

                char *firstname = read_firstname();
                if(firstname == NULL)
                    return;

                free(acc->name);
                free(acc->firstname);

                acc->name = name;
                acc->firstname = firstname;

                printf("\nNUMELE A FOST SCHIMBAT CU SUCCES!\n");
                break;

            case '2': ;
                char *currency = read_currency();
                if(currency == NULL)
                    return;

                strncpy(acc->currency, currency, 3);
                printf("\nVALUTA A FOST SCHIMBATA CU SUCCES!\n");
                break;

            case '3': ;
                char *pin1 = read_account_pin(0);
                if(pin1 == NULL)
                    return;

                char *pin2 = read_account_pin(1);
                if(pin2 == NULL){
                    free(pin1);
                    return;
                }

                if(strcmp(pin1,pin2)){
                    free(pin1);
                    free(pin2);
                    getError(12,1);
                    return;
                }

                strncpy(acc->pin, pin1, 4);
                printf("\nCODUL PIN A FOST SCHIMBAT CU SUCCES!\n");
                break;

            default:
                return;
        }

        goBack(1);
    }
    else{
        getError(4,1);
    }

    free(pin);
}

void draw(char *option){
    show_menu(0);
    *option = getch();
    switch(*option) {
        case '1':
            search_user();
            break;

        case '2':
            show_all_users();
            break;

        case '3':
            acces_account();
            break;

        case '4':
            add_new_account();
            break;

        case '5':
            delete_existing_account();
            break;

        case '6':
            withdraw();
            break;

        case '7':
            deposit();
            break;

        case '8':
            modify_account();
            break;

        default:
            system("cls");
            *option = 0;
            return;
    }
}

int main(void){
    getAccounts(&DATABASE);
    load_menus();
    load_supported_currencies();
    char option = 0;
    do {
        draw(&option);
    }while(option);
    update_DB(&List_ids);
    free_menus();
    free_DB();
    return 0;
}
