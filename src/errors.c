#include "../include/errors.h"

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

void goBack(const int nr){
    fprintf(stderr,"\n");
    switch(nr){
        case 0:
            printf("PRESS ANY KEY TO GO BACK...");
            break;

        case 1:
            printf("PRESS ANY KEY TO GO BACK TO THE MAIN MENU...");
            break;

        default:
            exit(EXIT_FAILURE);
    }
    fprintf(stderr,"\n");
    getch();
}

void getError(const int error, const int back_option){
    fprintf(stderr,"\nERROR: ");
    switch(error){
        case 0:
            fprintf(stderr,"ATI INTRODUS UN NUMAR INVALID");
            break;

        case 1:
            fprintf(stderr,"SUNT PERMISE DOAR LITERE");
            break;

        case 2:
            fprintf(stderr,"UTILIZATORUL NU A FOST GASIT");
            break;

        case 3:
            fprintf(stderr,"ATI INTRODUS UN COD PIN INVALID");
            break;

        case 4:
            fprintf(stderr,"CODUL PIN ESTE INCORECT");
            break;

        case 5:
            fprintf(stderr,"NUMARUL DE CONT NU A FOST GASIT");
            break;

        case 6:
            fprintf(stderr,"ATI INTRODUS PREA MULTE CARACTERE");
            break;

        case 7:
            fprintf(stderr,"NU SUNT PERMISE SPATII IN NUMELE DE FAMILIE");
            break;

        case 8:
            fprintf(stderr,"ATI INTRODUS PREA PUTINE CARACTERE");
            break;

        case 9:
            fprintf(stderr,"NU ATI INTRODUS NIMIC");
            break;

        case 10:
            fprintf(stderr,"VALUTA INTRODUSA NU A FOST GASITA IN LISTA VALUTELOR PERMISE");
            break;

        case 11:
            fprintf(stderr,"SOLD INSUFICIENT IN CONT");
            break;

        case 12:
            fprintf(stderr,"CODURILE PIN NU CORESPUND");
            break;

        default:
            printf("UNKNOWN!");
    }
    fprintf(stderr,"!\n");
    goBack(back_option);
}

void DatabaseError(const int error){
    system("cls");
    fprintf(stderr,"ERROR %d: ",error);
    switch(error){
        case 0:
            fprintf(stderr,"COULD NOT OPEN DATABASE FILE WITH ACCOUNTS");
            break;

        case 1:
            fprintf(stderr,"DATABASE IS CORRUPTED (INVALID NUMBER OF LINES)");
            break;

        case 2:
            fprintf(stderr,"NOT ENOUGH MEMORY TO LOAD DATABASE");
            break;

        case 3:
            fprintf(stderr,"DATABASE IS CORRUPTED (NAME)");
            break;

        case 4:
            fprintf(stderr,"DATABASE IS CORRUPTED (FIRSTNAME)");
            break;

        case 5:
            fprintf(stderr,"DATABASE IS CORRUPTED (CURRENCY)");
            break;

        case 6:
            fprintf(stderr,"DATABASE IS CORRUPTED (SUM)");
            break;

        case 7:
            fprintf(stderr,"DATABASE IS CORRUPTED (PIN)");
            break;

        case 8:
            fprintf(stderr,"DATABASE IS CORRUPTED (CREATED DATE)");
            break;

        case 9:
            fprintf(stderr,"COULD NOT OPEN DATABASE FILE WITH SUPPORTED CURRENCIES");
            break;

        case 10:
            fprintf(stderr,"SUPPORTED CURRENCIES FILE IS CORRUPTED");
            break;

        default:
            fprintf(stderr,"UNKNOWN");

    }
    fprintf(stderr,"!\n");
    exit(EXIT_FAILURE);
}
