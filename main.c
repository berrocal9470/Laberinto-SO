#include <stdio.h>
#include <stdlib.h>

#include "Juego.h"

int main()
{
    Juego juego;
    Juego_init(&juego);

    Juego_leerArchivo(&juego);
    //Juego_pintar(juego.laberinto.campos, juego.laberinto.filas, juego.laberinto.columnas);
    Juego_jugar(&juego);


    printf("\n\n");
    return 0;
}
