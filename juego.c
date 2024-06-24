#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "la_sombra_de_mac.h"


int main(){ 
    srand (( unsigned)time(NULL));
    

    juego_t juego;
    
    inicializar_juego(&juego);
    imprimir_terreno(juego);
    while(estado_juego(juego) == 0){
        while(estado_nivel(juego) == 0){
            realizar_jugada(&juego);
            printf("estas en nivel: %i\n",juego.nivel_actual+1 );
            printf("puntos: %i\n" , juego.personaje.puntos);
            printf("mac tiene %i vidas\n" , juego.personaje.vida);
            imprimir_terreno(juego);
        }
        if (estado_nivel(juego) == 1 && juego.nivel_actual<= 2){
            juego.nivel_actual++;
        }
    }
    if (estado_juego(juego) == -1){
        printf("perdiste!\n");
    }else if (estado_juego(juego) == 1){
        printf("ganaste!\n");
    }
    return 0;

} 





