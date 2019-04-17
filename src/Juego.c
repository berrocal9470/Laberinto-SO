#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "Juego.h"
#include "Recorrido.h"

//variables globales para ser accedida desde los hilos
char * juego;

typedef int bool;
#define true  1
#define false 0
bool terminado = false;
bool turn = true;

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
    turn = true;
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
                printf("\033[0m");  //color normal
                printf("\u25AE");   //cuadro en blanco
                break;
            case '^':
                printf("\033[0;33m");   //amarillo
                printf("^");
                break;
            case 'v':
                printf("\033[0;31m");   //rojo
                printf("v");
                break;
            case '<':
                printf("\033[0;36m");   //cyan
                printf("<");
                break;
            case '>':
                printf("\033[0;32m");   //verde
                printf(">");
                break;
            default:
                printf("\033[0m");      //color normal
                printf("%c", simbolo);
        }

        printf("\033[0m");
        //borde derecho
        if (((i+1) % columnas) == 0)
            printf("\u25AE\n");
    }

    //borde inferior
    for(int k=0; k<columnas+2; k++){
        printf("\u25AE");
    }

    printf("\n");
    turn = false;
}

/**
    Imprime la informaciòn final de un hilo
*/
void imp_s_juego_dir(long id, int ubicacion, int cantPasos, int estado, int nfilas){
    while(turn);
    int i = ubicacion / nfilas;
    int j = ubicacion - (i*nfilas);

    printf("%s","----------------------------------------------------------------------------\n");
    printf("%s","Hilo: ");
    printf("%ld",id);
    printf("%s",". Ubicación final: ");
    printf("(%d,%d)",i,j);
    printf("%s",". Recorrió: ");
    printf("%d",cantPasos);
    printf("%s"," casillas. Estado: ");
    if(estado==0){printf("%s","KIA\n");}
    else{printf("%s","MA\n");}
    fflush(stdout);
}

typedef struct s_juego_dir{
    long id;
    int fila;
    int columna;
    int direccion;
    int ubicacion;
    int cantPasos;
} s_juego_dir;

/**
    Implementa la lògica del juego en si, usada por los hilos "recursivos"
*/
void * juego_direccion(void * arg){
    //printf("ID hilo: %ld\n", pthread_self());
    struct s_juego_dir * actual;
    actual = (struct s_juego_dir *)arg;
    long id = actual->id;
    int fila = actual->columna;
    int columna = actual->fila;
    int direccion = actual->direccion;
    int ubicacion = actual->ubicacion;
    int cantPasos = actual->cantPasos;

    int mayor;
    if(fila >= columna) mayor = fila;
    else mayor = columna;
    pthread_t * hijos = malloc(sizeof(pthread_t) * mayor);
    int cant_hijos = 0;

    while(juego[ubicacion]!='*'){
        if(juego[ubicacion]=='/'){      //El unico que da solucion
            imp_s_juego_dir(id, ubicacion, cantPasos+1, 1, fila);
            break;
        }

        if(direccion==0 || direccion==1){
            if(direccion==0){
                if(juego[ubicacion]=='^') break;
                else juego[ubicacion]='^';

                if(ubicacion+1<=0){
                    imp_s_juego_dir(id, ubicacion+fila, cantPasos, 0, fila);
                    break;
                }
            }else{
                if(juego[ubicacion]=='v') break;
                else juego[ubicacion]='v';

                if(ubicacion+1>fila*columna){
                    imp_s_juego_dir(id, ubicacion-fila, cantPasos, 0, fila);
                    break;
                }
            }

            if((ubicacion)%columna!=0){
                if(juego[ubicacion-1]==' ' || juego[ubicacion-1]=='/'){
                    //printf("%s","Espacio vacio a la izquierda \n");
                    struct s_juego_dir r2;
                    r2.id = pthread_self();
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.cantPasos = cantPasos+1;
                    r2.direccion = 2;
                    r2.ubicacion = ubicacion-1;
                    int a = cant_hijos;
                    cant_hijos++;
                    pthread_create(&hijos[a], NULL, juego_direccion, (void *)&r2);
                    usleep(500000);
                    //printf("1-%ld\n",hijos[a]);
                    //pthread_join(hijos[a], NULL);
                }
            }
            if((ubicacion+1)%columna!=0){
                if(juego[ubicacion+1]==' ' || juego[ubicacion+1]=='/'){
                    //printf("%s","Espacio vacio a la derecha \n");
                    struct s_juego_dir r2;
                    r2.id = pthread_self();
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.cantPasos = cantPasos+1;
                    r2.direccion = 3;
                    r2.ubicacion = ubicacion+1;
                    int a = cant_hijos;
                    cant_hijos++;
                    pthread_create(&hijos[a], NULL, juego_direccion, (void *)&r2);
                    usleep(500000);
                    //printf("2-%ld\n",hijos[a]);
                    //pthread_join(hijos[a], NULL);
                }
            }

            if(direccion==0) ubicacion=ubicacion-fila;
            else ubicacion=ubicacion+fila;
        }
        else{
            if(direccion==2){
                if(juego[ubicacion]=='<') break;
                else juego[ubicacion]='<';
            }else{
                if(juego[ubicacion]=='>') break;
                else juego[ubicacion]='>';
            }

            if((juego[ubicacion-fila]==' ' || juego[ubicacion-fila]=='/')&&(ubicacion-fila>=0)){
                //printf("%s","Espacio vacio arriba \n");
                struct s_juego_dir r2;
                r2.id = pthread_self();
                r2.fila = fila;
                r2.columna = columna;
                r2.cantPasos = cantPasos+1;
                r2.direccion = 0;
                r2.ubicacion = ubicacion-fila;
                int a = cant_hijos;
                cant_hijos++;
                pthread_create(&hijos[a], NULL, juego_direccion, (void *)&r2);
                usleep(500000);
                //printf("3-%ld\n",hijos[a]);
                //pthread_join(hijos[a], NULL);
            }
            if((juego[ubicacion+fila]==' ' || juego[ubicacion+fila]=='/')&&(ubicacion+fila<fila*columna)){
                //printf("%s","Espacio vacio abajo \n");
                struct s_juego_dir r2;
                r2.id = pthread_self();
                r2.fila = fila;
                r2.columna = columna;
                r2.cantPasos = cantPasos+1;
                r2.direccion = 1;
                r2.ubicacion = ubicacion+fila;
                int a = cant_hijos;
                cant_hijos++;
                pthread_create(&hijos[a], NULL, juego_direccion, (void *)&r2);
                usleep(500000);
                //printf("4-%ld\n",hijos[a]);
                //pthread_join(hijos[a], NULL);
            }

            if(direccion==2){
                if((ubicacion)%columna==0){
                    imp_s_juego_dir(id, ubicacion, cantPasos+1, 0, fila);
                    break;
                }
                ubicacion=ubicacion-1;
            }
            else{
                if((ubicacion+1)%columna==0){
                    imp_s_juego_dir(id, ubicacion, cantPasos+1, 0, fila);
                    break;
                }
                ubicacion=ubicacion+1;
            }
        }

        cantPasos = cantPasos + 1;
        usleep(500000);
    }

    if(juego[ubicacion]=='*'){
        if(direccion==0){
            ubicacion=ubicacion+fila;
        }else if(direccion==1){
            ubicacion=ubicacion-fila;
        }else if(direccion==2){
            ubicacion=ubicacion+1;
        }else{
            ubicacion=ubicacion-1;
        }
        imp_s_juego_dir(id, ubicacion, cantPasos, 0, fila);
    }

    for(int i=0; i<cant_hijos; i++){
        pthread_join(hijos[i], NULL);
    }

    pthread_exit(NULL);
    //return NULL;
}

/**
    Hilo encargado de la impresion, imprime cada 0.5 segundos
*/
void * h_imprimir(void * arg){
    Laberinto * laberinto;
    laberinto = (Laberinto *)arg;

    while(!terminado){
        Juego_pintar(juego, laberinto->filas, laberinto->columnas);
        fflush(stdout);
        usleep(500000);
    }

    return NULL;
}


void Juego_jugar(Juego *self){
    int numCeldas = self->laberinto.filas*self->laberinto.columnas;
    juego = (char*)malloc(sizeof(char)*numCeldas);
    //crea una copia de la matriz
    memcpy(juego, self->laberinto.campos, sizeof(char)*numCeldas);

    printf("%c",'\n');

    pthread_t h1;

    struct s_juego_dir r1;
    r1.id = pthread_self();
    r1.fila = self->laberinto.filas;
    r1.columna = self->laberinto.columnas;
    r1.direccion = 1;
    r1.ubicacion = 0;
    r1.cantPasos = 0;
    //imp_s_juego_dir(r1.id,r1.ubicacion,r1.cantPasos,0);

    pthread_create(&h1, NULL, juego_direccion, (void *)&r1);

    //sleep y print
    pthread_t h_impresion;
    pthread_create(&h_impresion, NULL, h_imprimir, (void *)&self->laberinto);

    pthread_join(h1, NULL);
    terminado = true;
    pthread_join(h_impresion, NULL);

    //Juego_pintar(juego, self->laberinto.filas, self->laberinto.columnas);
    pthread_exit(NULL);
}
