#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../include/database.h"

#define NR_MENUS 3
#define MAX_LEN_ID 8

typedef unsigned int uint;

struct{
    uint nr;
    char** iso;
}currencies;

int convert_to_upper(char *s);
void load_menus(void);
void load_supported_currencies(void);
void free_menus(void);
void addDate(char *date, int d, int m, int y);

#endif // UTILITIES_H_INCLUDED
