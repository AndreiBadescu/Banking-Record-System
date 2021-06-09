#include "../include/input_read.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../include/utilities.h"
#include "../include/errors.h"

#define MAX_STR 64
#define MAX_WITHDRAW 1000000

inline void flush_stdin(void){
    char str[1024];
    fgets(str, 1024, stdin);
}

void show_menu(const uint nr){
    system("cls");
    printf("%s",MENU[nr]);
}

void print_account(const struct account *acc){
    if(acc == NULL)
        return;
    printf("NUMAR CONT:   %0*u\n", MAX_LEN_ID, acc->id);
    printf("NUME:         %s\n", acc->name);
    printf("PRENUME:      %s\n", acc->firstname);
    printf("DATA CREARII: %.2s/%.2s/%.4s\n", acc->created, acc->created+2, acc->created+4);
}

void print_privateInfo_account(const struct account *acc){
    if(acc == NULL)
        return;
    printf("NUMAR CONT:   %0*u\n", MAX_LEN_ID, acc->id);
    printf("NUME:         %s\n", acc->name);
    printf("PRENUME:      %s\n", acc->firstname);
    printf("VALUTA:       %.3s\n", acc->currency);
    printf("SUMA CURENTA: %+I64d\n", acc->curr_sum);
    printf("DATA CREARII: %.2s/%.2s/%.4s\n", acc->created, acc->created+2, acc->created+4);
}

void write_account(FILE *file, const struct account *acc){
    if(acc == NULL)
        return;
    fprintf(file,"%s\n",acc->name);
    fprintf(file,"%s\n",acc->firstname);
    fprintf(file,"%.3s\n",acc->currency);
    fprintf(file,"%I64d\n",acc->curr_sum);
    fprintf(file,"%.4s\n",acc->pin);
    fprintf(file,"%.8s\n\n",acc->created);
}

char* read_file(const char *filename){
    FILE* file = fopen(filename,"r");
    if(file == NULL) {
        fprintf(stderr, "ERROR: COULD NOT OPEN FILE: '%s' !\n", filename);
        exit(EXIT_FAILURE);
        //return NULL;
    }

    uint size = 0;
    for (char c; (c = fgetc(file)) != EOF; ++size);

    char* content = (char *) malloc(size + 1);

    rewind(file);
    if(fread(content,1,size,file) != size){
        fprintf(stderr, "ERROR: COULD NOT READ THIS FILE: %s !\n", filename);
        fclose(file);
        exit(EXIT_FAILURE);
    }
    content[size] = '\0';
    fclose(file);

    return content;
}

int read_account_id(void){
    printf("Introduceti numarul de cont:\n");
    char *input = (char*) malloc(MAX_LEN_ID + 2); // +2 for "\n\0"
    fgets(input, MAX_LEN_ID + 2, stdin); // fgets(char *str, size_t n) reads max (n - 1) characters

    if(input[0] == '\n'){
        free(input);
        return -1;
    }

    if(strlen(input) == (MAX_LEN_ID + 1) && input[MAX_LEN_ID] != '\n'){
        flush_stdin();
        free(input);
        return -1;
    }

    for(uint i = 0; input[i+1]; ++i){
        if(input[i] < '0' || input[i] > '9'){
            free(input);
            return -1;
        }
    }

    int nr = strtol(input, NULL, 10);
    free(input);
    return nr;
}

inline char* getInput(uint noSpaces, uint elimDashes){
    char *input = (char*) malloc(MAX_STR);
    fgets(input, MAX_STR, stdin);

    if(input[0] == '\n'){
        free(input);
        getError(9,0);
        return NULL;
    }

    if(input[strlen(input) - 1] != '\n'){
        flush_stdin();
        free(input);
        getError(6,0);
        return NULL;
    }

    if(noSpaces){
        for(uint i = 0; input[i]; ++i){
            if(input[i] == ' '){
                free(input);
                getError(7,0);
                return NULL;
            }
        }
    }

    if(elimDashes){
        for(uint i = 0; input[i]; ++i){
            if(input[i] == '-')
                input[i] = ' ';
        }
    }

    if(convert_to_upper(input)){
        free(input);
        getError(1,0);
        return NULL;
    }

    return input;
}

char* read_name(void){
    printf("Introduceti numele de familie:\n");
    return getInput(1,0);
}

char* read_firstname(void){
    printf("Introduceti prenumele:\n");
    return getInput(0,1);
}

char* read_currency(void){
    //extern struct currencies;
    printf("Introduceti valuta:\n");
    char *input = (char*) malloc(5);
    fgets(input, 5, stdin);

    if(strlen(input) < 4){
        free(input);
        getError(8,0);
        return NULL;
    }

    if(input[3] != '\n'){
        flush_stdin();
        free(input);
        getError(6,0);
        return NULL;
    }

    if(convert_to_upper(input)){
        free(input);
        getError(1,0);
        return NULL;
    }

    uint ok = 0;
    for(uint i = 0; i < currencies.nr; ++i){
        char tmp[4];
        strncpy(tmp, currencies.iso[i], 3);
        tmp[3] = '\0';

        if(!strcmp(input,tmp)){
            ok = 1;
            break;
        }
    }

    if(ok)
        return input;

    free(input);
    getError(10,0);
    return NULL;
}

char* read_account_pin(uint option){
    if(option == 0)
        printf("Introduceti codul pin (se permit doar cifre):\n");
    else
        printf("Introduceti codul pin din nou:\n");

    char *pin = (char*) malloc(PIN_LEN + 1);

    for(int i = 0; i < PIN_LEN; ){
        char c = getch();
        if(c == '\r'){
            printf("\n");
            getError(8,1);
            free(pin);
            return NULL;
        }
        else if(c == '\b'){
            if(i > 0)
                --i;
            printf("\b \b");
        }
        else if(c >= '0' && c <= '9'){
            printf("*");
            pin[i++] = c;
        }
    }
    printf("\n");

    pin[PIN_LEN] = '\0';
    return pin;
}

long long read_account_sum(void){
    printf("Introduceti suma dorita:\n");
    char *input = (char*) malloc(MAX_WITHDRAW + 3); // +3 for '-' and "\n\0"
    fgets(input, MAX_WITHDRAW + 3, stdin); // fgets(char *str, size_t n) reads max (n - 1) characters

    if(input[0] == '\n'){
        free(input);
        return 0;
    }

    uint sign = (input[0] == '-') ? 1 : 0;

    if(strlen(input) == (MAX_WITHDRAW + 1 + sign) && input[MAX_WITHDRAW + sign] != '\n'){
        flush_stdin();
        free(input);
        return 0;
    }

    long long sum = 0;
    for(uint i = (input[0] == '-') ? 1 : 0; input[i+1]; ++i){
        if(input[i] < '0' || input[i] > '9'){
            free(input);
            return 0;
        }
        sum = sum * 10 + input[i] - '0';
    }

    if(sign)
        sum = -sum;

    free(input);
    return sum;
}
