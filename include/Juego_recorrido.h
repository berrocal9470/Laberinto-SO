#ifndef RECORRIDO_H
#define RECORRIDO_H

//             0       1      2          3
enum direccion{Arriba, Abajo, Izquierda, Derecha};

typedef struct Recorrido
{
    int id;
    int posX;   //vendria siendo el j
    int posY;   //vendrìa siendo el i
    int cantPasos;
    enum direccion dir;

} Recorrido;

#endif // RECORRIDO_H
