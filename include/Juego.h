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
void Juego_pintar(char *campos, int filas, int columnas);
void Juego_jugar(Juego *self);
void *Juego_avanzar(void * arg);

#endif // JUEGO_H
