#ifndef JUEGO_H
#define JUEGO_H

#include "DAOtxt.h"
#include "Laberinto.h"

typedef struct Juego
{
    //Variables
    DAOtxt txt;
    Laberinto laberinto;

} Juego;

int Juego_init(Juego *self);

void Juego_leerArchivo(Juego *self);
void Juego_pintar(Juego *self);

#endif // JUEGO_H
