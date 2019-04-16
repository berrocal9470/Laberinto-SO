#include <stdio.h>
#include <stdlib.h>
#include "DAOtxt.h"

int DAOtxt_init(DAOtxt *self){
    strcpy(self->nombreArchivo, "laberintos/"); //direccion

    strcat(self->nombreArchivo, "laberinto2");  //nombre
    //strcat(self->nombreArchivo, "lab2-profe");
    return 0;
}

/**
    Separa una cadena de caracteres en dos enteros, tomando como separador
    el sìmbolo de espacio.
    str: linea leida del archivo con las dimensiones
    dim: array int de dos posiciones donde alojara los valores
*/
void DAOtxt_getDimensiones(char *str, int* dim)
{
    size_t num = strlen(str)+1;

    char* izq = (char*) calloc( num , sizeof(char) );
    char* der = (char*) calloc( num , sizeof(char) );
    char* split = strchr(str, ' ');

    size_t len1 = strlen(str) - strlen(split);
    size_t len2 = strlen(split)-1;

    strncpy(izq, str, len1 );
    strncpy(der, split+1, len2 );

    dim[0] = atoi(izq);
    dim[1] = atoi(der);
}


/**
    Lee de un archivo txt y crea la matriz del juego
*/
Laberinto DAOtxt_leer(DAOtxt *self){
    printf("Leyendo el archivo: %s\n\n", self->nombreArchivo);

    FILE *archivo;
 	char caracteres[100];

 	archivo = fopen(self->nombreArchivo,"r");

    Laberinto juego;

 	if (archivo == NULL){
        printf("Error al leer archivo.\n");
 	}
 	else{
        int dimensiones[2];

        if(feof(archivo) == 0){
            fgets(caracteres, 100, archivo);

            DAOtxt_getDimensiones(caracteres, dimensiones);

            juego.filas = dimensiones[0];
            juego.columnas = dimensiones[1];
        }


 	    char temp[dimensiones[0]][dimensiones[1]];

 	    for(int i=0; i<dimensiones[0]; i++){
            for(int j=0; j<dimensiones[1]; j++){
                char caracter = fgetc(archivo);

                if (caracter == '\n')
                    temp[i][j] = fgetc(archivo);
                else
                    temp[i][j] = caracter;
            }
 	    }

 	    juego.campos = &temp[0][0];

        fclose(archivo);
    }
    return juego;
}
