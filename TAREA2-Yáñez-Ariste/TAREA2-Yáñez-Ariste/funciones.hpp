#ifndef AJEDREZ_H 
#define AJEDREZ_H

#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Pieza {
    char simbolo;
    int x, y;
};


struct Tablero {
    Pieza *piezas_tablero;
    int cantidad_piezas;
};

bool jaqueDama(Tablero &tablero, int reyX, int reyY);
bool jaquePeon(Tablero &tablero, int reyX, int reyY);
bool jaqueCaballo(Tablero &tablero, int reyX, int reyY);
bool jaqueAlfil(Tablero &tablero, int reyX, int reyY);
void leerTablero(Tablero &tablero);
void liberarMemoria(Tablero &tablero);
bool jaqueRey(Tablero &tablero, int reyX, int reyY);
bool jaqueMate(Tablero &tablero, int reyX, int reyY);

#endif 
