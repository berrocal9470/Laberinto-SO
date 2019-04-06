#ifndef DAOTXT_H
#define DAOTXT_H

#include "Laberinto.h"

typedef struct DAOtxt
{
    //Variables
    char nombreArchivo[50];

} DAOtxt;

int DAOtxt_init(DAOtxt *self);

Laberinto DAOtxt_leer(DAOtxt *self);
void DAOtxt_getDimensiones(char *str, int* dim);

#endif // DAOTXT_H
