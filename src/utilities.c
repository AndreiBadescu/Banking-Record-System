#include "../include/utilities.h"

#include <stdio.h>
#include "../include/errors.h"
#include "../include/input_read.h"

int convert_to_upper(char *s){
    /// 0 - SUCCEDED
    /// 1 - THE STRING CONTAINS NON-ALPHA CHARACTERS
    /// *** IT STOPS WHEN REACHING A NEW-LINE AND ADDS A NULL TERMINATOR ***
    for(uint i = 0; s[i]; ++i){
        if(s[i] == ' ')
            continue;
        if(s[i] >= 'A' && s[i] <= 'Z')
            continue;
        if(s[i] >= 'a' && s[i] <= 'z')
            s[i] -= 32;
        else{
            if(s[i] == '\n'){
                s[i] = '\0';
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

void load_menus(void){
    extern char *MENU[];
    char path[] = "./files/menus/menu_#.txt";
    for(uint i = 0; i < NR_MENUS; ++i){
        /// the # character from path is the number of the menu we want
        path[19] = '0' + i;
        MENU[i] = read_file(path);
    }
}

void load_supported_currencies(void){
    FILE *file = fopen("./files/currencies.txt", "r");
    if(file == NULL)
        DatabaseError(9);

    currencies.nr = 0;
    char buffer[6];

    while(fgets(buffer, 5, file)){
        if(buffer[3] != '\n')
            DatabaseError(10);
        ++currencies.nr;
    }

    currencies.iso = (char**) malloc(currencies.nr*sizeof(char*) + 3*currencies.nr);
    char *ptr = (char*)(currencies.iso + currencies.nr);
    for(uint i = 0; i < currencies.nr; ++i){
        currencies.iso[i] = ptr + 3*i;
    }

    fseek(file, 0, SEEK_SET);

    currencies.nr = 0;
    while(fgets(buffer, 5, file)){
        strncpy(currencies.iso[currencies.nr++],buffer,3);
    }
}

void free_menus(void){
    extern char *MENU[];
    for(uint i = 0; i < NR_MENUS; ++i){
        free(MENU[i]);
    }
}

void addDate(char *date, int d, int m, int y){
    m += 1, y += 1900;
    date[0] = d / 10 + '0';
    date[1] = d % 10 + '0';
    date[2] = m / 10 + '0';
    date[3] = m % 10 + '0';
    date[4] = y / 1000 + '0';
    date[5] = (y / 100) % 10 + '0';
    date[6] = (y / 10)  % 10 + '0';
    date[7] = y % 10 + '0';
}
