#include <stdlib.h>
#include <time.h>
#include "la_sombra_de_mac.h"
#include <stdio.h>

int main(){
    srand (( unsigned)time(NULL));
    int fila_aleatoria=rand()% MAX_FILAS;
    int col_aleatoria=rand()% MAX_COLUMNAS;
    
    coordenada_t posicion_personaje;
    posicion_personaje.fila = fila_aleatoria;
    posicion_personaje.col = col_aleatoria;

    coordenada_t posicion_sombra;
    posicion_sombra.fila = fila_aleatoria;
    posicion_sombra.col = MAX_COLUMNAS - posicion_personaje.col-1;

    char movimiento;

    elemento_t elementos[MAX_ELEMENTOS];
    
    coordenada_t coordenada_elemento;
    coordenada_elemento.fila = 3;
    coordenada_elemento.col = 3;
    

    juego_t juego;
    
    int tope = 0;

	inicializar_personaje(&juego.personaje,posicion_personaje);
	inicializar_sombra(&juego.sombra,posicion_sombra);
    imprimir_terreno(juego);
    while(tope < 10){
    	pedir_movimiento(&movimiento);
    	
    	mover_personaje(&juego.personaje, &juego.sombra, movimiento);
    	
    	tope++;
    	imprimir_terreno(juego);
    }
    coordenadas_iguales(posicion_personaje,posicion_sombra);
	
	
	coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
	buscar_elemento(elementos,tope,coordenada_elemento);
	
	



//    printf("mac esta en la fila %i \n",personaje.posicion.fila);
//    printf("mac esta en la col %i \n",personaje.posicion.col);
//    printf("bloo esta en la fila %i \n",sombra.posicion.fila);
//    printf("bloo esta en la col %i \n",sombra.posicion.col);
   	return 0;

} 

	








/*
    printf("mac esta en la fila %i \n",personaje.posicion.fila);
    printf("mac esta en la col %i \n",personaje.posicion.col);
    printf("bloo esta en la fila %i \n",sombra.posicion.fila);
    printf("bloo esta en la col %i \n",sombra.posicion.col);

    char movimiento= 'S';
    mover_personaje(&personaje, &sombra, movimiento);
    printf("el movimiento %c lleva a que ahora: \n",movimiento);

    printf("mac esta en la fila %i \n",personaje.posicion.fila);
    printf("mac esta en la col %i \n",personaje.posicion.col);
    printf("bloo esta en la fila %i \n",sombra.posicion.fila);
    printf("bloo esta en la col %i \n",sombra.posicion.col);
*/



















bool coordenadas_iguales(coordenada_t primera_coordenada, coordenada_t segunda_coordenada){

    return (primera_coordenada.fila == segunda_coordenada.fila) && (primera_coordenada.col==segunda_coordenada.col);

}
bool se_encuentra_escalera_arriba(int personaje_col ,int escalera_col,int personaje_fila ,int escalera_fila){
    return personaje_col == escalera_col && personaje_fila-1 == escalera_fila;
}

int buscar_elemento(elemento_t elementos[MAX_ELEMENTOS], int tope, coordenada_t posicion){

    int posicion_encontrado= NO_ENCONTRADO;
    
    bool encontrado = false;
    for (int i = 0 ; i< tope ; i++){
        if (coordenadas_iguales(elementos[i].coordenada , posicion) && !encontrado){
            posicion_encontrado = i;
            encontrado = true;
            
        }
    }  
    return posicion_encontrado;

}
int posicion_escalera_encontrado = buscar_elemento(herramientas,&(*tope_herramientas),*personaje_posicion );
int buscar_arriba_escalera(int personaje_fila , int personaje_col,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    bool se_encuentra = false;
    int posicion_encontrado = NO_ENCONTRADO;
    for (int i=0 ;i<(*tope_herramientas);i++){
        if (herramientas[i].tipo == ESCALERA){
            if (coordenadas_iguales(personaje_posicion ,herramientas[i].coordenada) &&!se_encuentra){
                posicion_encontrado = i;
                se_encuentra = true;
            }  
        }
    }   
    return posicion_encontrado;
}