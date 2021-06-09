#ifndef INPUT_READ_H_INCLUDED
#define INPUT_READ_H_INCLUDED

#include <stdio.h>
#include "../include/database.h"

#define MAX_LEN_ID 8
#define PIN_LEN 4

typedef unsigned int uint;
extern char *MENU[];

void flush_stdin(void);
void show_menu(const uint nr);
void print_account(const struct account *acc);
void print_privateInfo_account(const struct account *acc);
void write_account(FILE *file, const struct account *acc);

/* reading functions */
char* read_file(const char *filename);
char* getInput(uint noSpaces, uint elimDashes);
int read_account_id(void);
char* read_firstname(void);
char* read_name(void);
char* read_currency(void);
char* read_account_pin(uint option);
long long read_account_sum(void);

#endif // INPUT_READ_H_INCLUDED
