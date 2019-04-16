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

void imp_s_juego_dir(long id, int ubicacion, int cantPasos, int estado){
    printf("%s","----------------------------------------------------------------------------\n");
    printf("%s","Hilo: ");
    printf("%ld",id);
    printf("%s",". Ubicación final: ");
    printf("%d",ubicacion);
    printf("%s",". Recorrió: ");
    printf("%d",cantPasos);
    printf("%s"," casillas. Estado: ");
    if(estado==0){printf("%s","KIA\n");}
    else{printf("%s","MA\n");}
}

typedef struct s_juego_dir{
    long id;
    int fila;
    int columna;
    int direccion;
    int ubicacion;
    int cantPasos;
} s_juego_dir;

void juego_direccion(void * arg){
    //printf("ID hilo: %ld\n", pthread_self());
    struct s_juego_dir r2;
    struct s_juego_dir * actual;
    actual = (struct s_juego_dir *)arg;
    long id = actual->id;
    int fila = actual->fila;
    int columna = actual->columna;
    int direccion = actual->direccion;
    int ubicacion = actual->ubicacion;
    int cantPasos = actual->cantPasos;

    while(1){
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
            imp_s_juego_dir(id, ubicacion, cantPasos, 0);
            break;
        }

        if(juego[ubicacion]=='/'){
            imp_s_juego_dir(id, ubicacion, cantPasos+1, 1);
            //printf("%c",juego[ubicacion]);
            //printf("%s", "Hilo encontró la solución\n");
            break;
        }

        if(direccion==0){
            juego[ubicacion]='^';
            if(ubicacion+1<=0){
                imp_s_juego_dir(id, ubicacion+fila, cantPasos, 0);
                //printf("%s", "KIA\n");
                break;}
            if((ubicacion+1)%columna!=0){
                if(juego[ubicacion+1]==' ' || juego[ubicacion+1]=='/'){
                    //printf("%s","Espacio vacio a la derecha \n");
                    pthread_t h2;
                    r2.id = pthread_self()+1;
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.direccion = 3;
                    r2.ubicacion = ubicacion+1;
                    r2.cantPasos = cantPasos+1;
                    pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                    pthread_join(h2, NULL);
                    //juego_direccion(fila, columna,3,ubicacion+1);
                }
            }
            if((ubicacion)%columna!=0){
                if(juego[ubicacion-1]==' ' || juego[ubicacion-1]=='/'){
                    //printf("%s","Espacio vacio a la izquierda \n");
                    pthread_t h2;
                    r2.id = pthread_self();
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.direccion = 2;
                    r2.ubicacion = ubicacion-1;
                    r2.cantPasos = cantPasos+1;
                    pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                    pthread_join(h2, NULL);
                    //juego_direccion(fila, columna,2,ubicacion-1);
                }
            }
            ubicacion=ubicacion-fila;
        }
        else if(direccion==1){
            juego[ubicacion]='v';
            if(ubicacion+1>fila*columna){
                imp_s_juego_dir(id, ubicacion-fila, cantPasos, 0);
                //printf("%s", "KIA\n");
                break;}
            if((ubicacion)%columna!=0){
                if(juego[ubicacion-1]==' ' || juego[ubicacion-1]=='/'){
                    //printf("%s","Espacio vacio a la izquierda \n");
                    pthread_t h2;
                    r2.id = pthread_self();
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.direccion = 2;
                    r2.ubicacion = ubicacion-1;
                    r2.cantPasos = cantPasos+1;
                    pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                    pthread_join(h2, NULL);
                    //juego_direccion(fila, columna,2,ubicacion-1);
                }
            }
            if((ubicacion+1)%columna!=0){
                if(juego[ubicacion+1]==' ' || juego[ubicacion+1]=='/'){
                    //printf("%s","Espacio vacio a la derecha \n");
                    pthread_t h2;
                    r2.id = pthread_self()+1;
                    r2.fila = fila;
                    r2.columna = columna;
                    r2.direccion = 3;
                    r2.ubicacion = ubicacion+1;
                    r2.cantPasos = cantPasos+1;
                    pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                    pthread_join(h2, NULL);
                    //juego_direccion(fila, columna,3,ubicacion+1);
                }
            }
            ubicacion=ubicacion+fila;
        }
        else if(direccion==2){
            juego[ubicacion]='<';

            if((juego[ubicacion-fila]==' ' || juego[ubicacion-fila]=='/')&&(ubicacion-fila>=0)){
                //printf("%s","Espacio vacio arriba \n");
                pthread_t h2;
                r2.id = pthread_self()+1;
                r2.fila = fila;
                r2.columna = columna;
                r2.direccion = 0;
                r2.ubicacion = ubicacion-fila;
                r2.cantPasos = cantPasos+1;
                pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                pthread_join(h2, NULL);
                //juego_direccion(fila, columna,0,ubicacion-fila);
            }
            if((juego[ubicacion+fila]==' ' || juego[ubicacion+fila]=='/')&&(ubicacion+fila<fila*columna)){
                //printf("%s","Espacio vacio abajo \n");
                pthread_t h2;
                r2.id = pthread_self();
                r2.fila = fila;
                r2.columna = columna;
                r2.direccion = 1;
                r2.ubicacion = ubicacion+fila;
                r2.cantPasos = cantPasos+1;
                pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                pthread_join(h2, NULL);
                //juego_direccion(fila, columna,1,ubicacion+fila);
            }
            if((ubicacion)%columna==0){
                imp_s_juego_dir(id, ubicacion, cantPasos+1, 0);
                //printf("%s", "KIA\n");
                break;}
            ubicacion=ubicacion-1;
        }
        else{
            juego[ubicacion]='>';

            if((juego[ubicacion-fila]==' ' || juego[ubicacion-fila]=='/')&&(ubicacion-fila>=0)){
                //printf("%s","Espacio vacio arriba \n");
                pthread_t h2;
                r2.id = pthread_self()+1;
                r2.fila = fila;
                r2.columna = columna;
                r2.direccion = 0;
                r2.ubicacion = ubicacion-fila;
                r2.cantPasos = cantPasos+1;
                pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                pthread_join(h2, NULL);
                //juego_direccion(fila, columna,0,ubicacion-fila);
            }
            if((juego[ubicacion+fila]==' ' || juego[ubicacion+fila]=='/')&&(ubicacion+fila<fila*columna)){
                //printf("%s","Espacio vacio abajo \n");
                pthread_t h2;
                r2.id = pthread_self();
                r2.fila = fila;
                r2.columna = columna;
                r2.direccion = 1;
                r2.ubicacion = ubicacion+fila;
                r2.cantPasos = cantPasos+1;
                pthread_create(&h2, NULL, juego_direccion, (void *)&r2);
                pthread_join(h2, NULL);
            }
            if((ubicacion+1)%columna==0){
                imp_s_juego_dir(id, ubicacion, cantPasos+1, 0);
                //printf("%s", "KIA\n");
                break;}
            ubicacion=ubicacion+1;
        }
        cantPasos = cantPasos + 1;
    }
}


void Juego_jugar(Juego *self){
    int numCeldas = self->laberinto.filas*self->laberinto.columnas;
    juego = (char*)malloc(sizeof(char)*numCeldas);
    //crea una copia de la matriz
    memcpy(juego, self->laberinto.campos, sizeof(char)*numCeldas);

    //juego[0] = 'a';
    //juego_direccion(self->laberinto.filas, self->laberinto.columnas,1,0);
    printf("%c",'\n');

    pthread_t h1;
    //pthread_create(&h1, NULL, avanzar, NULL);

    struct s_juego_dir r1;
    r1.id = pthread_self();
    r1.fila = self->laberinto.filas;
    r1.columna = self->laberinto.columnas;
    r1.direccion = 1;
    r1.ubicacion = 0;
    r1.cantPasos = 0;
    imp_s_juego_dir(r1.id,r1.ubicacion,r1.cantPasos,0);

    pthread_create(&h1, NULL, juego_direccion, (void *)&r1);
    //sleep y print

    pthread_join(h1, NULL);
    Juego_pintar(juego, self->laberinto.filas, self->laberinto.columnas);
}
