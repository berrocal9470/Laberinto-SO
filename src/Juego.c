#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "Juego.h"
#include "Recorrido.h"

//variable global para ser accedida desde los hilos
char * juego;

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
void Juego_pintar(char *campos, int filas, int columnas){
    int celdas = filas * columnas;

    //borde superior
    for(int k=0; k<columnas+2; k++){
        printf("\u25AE");   //Cuadro en blanco
    }
    printf("\n");

    for(int i=0; i<celdas; i++){
        char simbolo = campos[i];

        //borde izquierdo
        if(i%columnas == 0)
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
        if (((i+1) % columnas) == 0)
            printf("\u25AE\n");
    }

    //borde inferior
    for(int k=0; k<columnas+2; k++){
        printf("\u25AE");
    }

    printf("\n");
}

//--METODOS DE LOS HILOS
void *avanzar(void * arg){
    struct Recorrido * actual;
    actual = (struct Recorrido *)arg;

    printf("id hilo: %i\n", actual->id);

    return NULL;
}



void Juego_jugar(Juego *self){
    int numCeldas = self->laberinto.filas*self->laberinto.columnas;
    juego = (char*)malloc(sizeof(char)*numCeldas);
    //crea una copia de la matriz
    memcpy(juego, self->laberinto.campos, sizeof(char)*numCeldas);

    /*for(int i=0; i<numCeldas; i++){
        printf("%c", juego[i]);
    }*/
    Juego_pintar(juego, self->laberinto.filas, self->laberinto.columnas);

    pthread_t h1;
    pthread_t h2;

    struct Recorrido r1;
    struct Recorrido r2;
    r1.id = 1;
    r2.id = 2;

    pthread_create(&h1, NULL, avanzar, (void *)&r1);
    pthread_create(&h2, NULL, avanzar, (void *)&r2);
    //sleep y print

    pthread_join(h1, NULL);
    pthread_join(h2, NULL);
}
