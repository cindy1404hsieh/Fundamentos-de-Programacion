#ifndef __UTILES__
#define __UTILES__

#include "la_sombra_de_mac.h"

/*	
 *  Procedimiento que inicializa los mapas.
 *  Debe llamarse una sola vez antes de llamar a obtener_mapa()
*/
void cargar_mapas();

/*
 *  Funcion que recibe un vector de paredes y su tope.
 *  Inicializa el vector con las paredes de algun mapa.
*/
void obtener_mapa(coordenada_t paredes[MAX_PAREDES], int* tope);

#endif