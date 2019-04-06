#include <stdio.h>
#include <stdlib.h>

#include "Juego.h"

int main()
{
    Juego juego;
    Juego_init(&juego);

    Juego_leerArchivo(&juego);
    Juego_pintar(&juego);


    printf("\n\n");
    return 0;
}
