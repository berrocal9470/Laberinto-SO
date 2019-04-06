#include <stdio.h>
#include "Juego.h"

int Juego_init(Juego *self){
    DAOtxt_init(&self->txt);
    return 0;
}

/**
    Llama a DAOtxt para leer el archivo y crear la matriz del juego
*/
void Juego_leerArchivo(Juego *self){
    self->laberinto = DAOtxt_leer(&self->txt);

    printf("Filas: %i\n", self->laberinto.filas);
    printf("Columnas: %i\n", self->laberinto.columnas);
    printf("------------\n");

}

/**
    Pinta en consola el laberinto leido.
    ----> Le añade bordes al laberinto <----
*/
void Juego_pintar(Juego *self){
    int celdas = self->laberinto.filas * self->laberinto.columnas;

    //borde superior
    for(int k=0; k<self->laberinto.columnas+2; k++){
        printf("\u25AE");   //Cuadro en blanco
    }
    printf("\n");

    for(int i=0; i<celdas; i++){
        char simbolo = *(self->laberinto.campos + i);

        //borde izquierdo
        if(i%self->laberinto.columnas == 0)
            printf("\u25AE");

        //lo puse en switch para tener màs libertad por si quisieramos
        // cambiar los sìmbolos a mostrar
        switch(simbolo){
            case '*':
                printf("\u25AE");   //cuadro en blanco
                break;
            default:
                printf("%c", simbolo);
        }

        //borde derecho
        if (((i+1) % self->laberinto.columnas) == 0)
            printf("\u25AE\n");
    }

    //borde inferior
    for(int k=0; k<self->laberinto.columnas+2; k++){
        printf("\u25AE");
    }
}
