#include <stdlib.h>
#include <time.h>
#include "la_sombra_de_mac.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define CONFIG_JUEGO "config_juego"
#define AGREGAR_PARTIDA "agregar_partida"
#define ELIMINAR_PARTIDA "eliminar_partida"
#define ORDENAR_PARTIDA "ordenar_partida"
#define MAX_NOMBRE 100
#define MAX_PARTIDAS 1000
#define LECTURA "%[^;];%i;%i;%i;%[^\n]\n"
#define ESCRITURA "%s;%i;%i;%i;%s\n"
#define CANTIDAD_LEIDO 5
#define GANO "Si"
#define NO_GANO "No"

typedef struct partida {
    char jugador[MAX_NOMBRE];
    int nivel_llegado;
    int puntos;
    int vidas_restantes;
    bool gano;
} partida_t;

void pedir_informacion(partida_t* partida_nueva ,char partida_nueva_gano[MAX_NOMBRE]){
    printf("Ingrese una nueva partida: \n");
    printf("Ingrese su nombre: \n");
    scanf("%s" , (*partida_nueva).jugador);
    printf("Ingrese el nivel llegado: \n");
    scanf("%i" , &(*partida_nueva).nivel_llegado);
    printf("Ingrese puntos obtenidos: \n");
    scanf("%i" , &(*partida_nueva).puntos);
    printf("Ingrese las vidas restantes: \n");
    scanf("%i" , &(*partida_nueva).vidas_restantes);
    printf("Ingrese si gano(Si/No): \n");
    scanf("%s" ,partida_nueva_gano);
}
void agregar_partida(FILE* jugadores , FILE* jugadores_aux){
    partida_t partida;
    partida_t partida_nueva;
    bool se_agrego = false;
    char gano[MAX_NOMBRE];
    char partida_nueva_gano[MAX_NOMBRE];
    pedir_informacion(&partida_nueva,partida_nueva_gano);

    int leido = fscanf(jugadores , LECTURA , partida.jugador,&partida.nivel_llegado,&partida.puntos,&partida.vidas_restantes,gano);
    while(leido == CANTIDAD_LEIDO){
        if (strcmp(partida.jugador , partida_nueva.jugador)<0 || se_agrego){
            fprintf(jugadores_aux, ESCRITURA, partida.jugador,partida.nivel_llegado,partida.puntos,partida.vidas_restantes,gano);
            
        }else if (strcmp(partida.jugador , partida_nueva.jugador)>0){
            fprintf(jugadores_aux, ESCRITURA, partida_nueva.jugador,partida_nueva.nivel_llegado,partida_nueva.puntos,partida_nueva.vidas_restantes,partida_nueva_gano);
            fprintf(jugadores_aux, ESCRITURA, partida.jugador,partida.nivel_llegado,partida.puntos,partida.vidas_restantes,gano);
            se_agrego = true;
        }
        
        leido = fscanf(jugadores , LECTURA , partida.jugador,&partida.nivel_llegado,&partida.puntos,&partida.vidas_restantes,gano);
    }
    if (!se_agrego){
        fprintf(jugadores_aux, ESCRITURA, partida_nueva.jugador,partida_nueva.nivel_llegado,partida_nueva.puntos,partida_nueva.vidas_restantes,partida_nueva_gano);
    }
}
void eliminar_partida(FILE* jugadores , FILE* jugadores_aux ,char nombre_a_eliminar[MAX_NOMBRE]){

    partida_t partida;
    char gano[MAX_NOMBRE];
    bool eliminado = false;

    int leido = fscanf(jugadores , LECTURA , partida.jugador,&partida.nivel_llegado,&partida.puntos,&partida.vidas_restantes,gano);
    while(leido == CANTIDAD_LEIDO){
        if (strcmp(partida.jugador,nombre_a_eliminar) != 0 || (strcmp(partida.jugador , nombre_a_eliminar)== 0 && eliminado)){
            fprintf(jugadores_aux, ESCRITURA, partida.jugador,partida.nivel_llegado,partida.puntos,partida.vidas_restantes,gano);
        }else{
            eliminado = true;
        }
        leido = fscanf(jugadores , LECTURA , partida.jugador,&partida.nivel_llegado,&partida.puntos,&partida.vidas_restantes,gano);
    }

}
void cargar_partidas(FILE* jugadores , partida_t partidas[MAX_PARTIDAS],int* tope,char gano[MAX_PARTIDAS]){

    while(!feof(jugadores)){
        fscanf(jugadores , LECTURA , partidas[(*tope)].jugador,&partidas[(*tope)].nivel_llegado,&partidas[(*tope)].puntos,&partidas[(*tope)].vidas_restantes,gano);
        if (strcmp(gano,"Si")==0){
            partidas[(*tope)].gano = true;
        }else{
            partidas[(*tope)].gano = false;
        }
        (*tope)++;
    }
    
}
void ordenar_partida(FILE* jugadores,FILE* jugadores_aux){
    partida_t partidas[MAX_PARTIDAS];
    int tope_partidas = 0;
    char gano[MAX_NOMBRE];
    cargar_partidas(jugadores , partidas , &tope_partidas , gano);

    partida_t aux_partida;
    int j;

    for (int i = 1; i < tope_partidas; i++) {
        j=i;
        aux_partida = partidas[i];
        while (j > 0 && strcmp(aux_partida.jugador,partidas[j-1].jugador)<0){
            partidas[j] = partidas[j-1];
            
            j--;
        }
        partidas[j] = aux_partida;

    }
    for(int i=0;i<tope_partidas;i++){
        if (partidas[i].gano){
            strcpy(gano , "Si");
        }else{
            strcpy(gano , "No");
        }
        fprintf(jugadores_aux,ESCRITURA,partidas[i].jugador,partidas[i].nivel_llegado,partidas[i].puntos,partidas[i].vidas_restantes,gano);
    }
}
int main(int argc, char* argv[]){ 

    if (strcmp(argv[1] , CONFIG_JUEGO) !=0 ){
        FILE* jugadores = fopen(argv[2], "r");
        if(!jugadores) {
            perror("No se pudo abrir el archivo :c \n");
            return -1;
        }

        FILE* jugadores_aux = fopen("jugadores_aux.csv", "w");
        if(!jugadores_aux) {
            fclose(jugadores);
            perror("No se pudo abrir el archivo :c \n");
            return -1;
        }
        char nombre_a_eliminar[MAX_NOMBRE];

        if (strcmp(argv[1] , AGREGAR_PARTIDA)== 0 && argc == 2){
            agregar_partida(jugadores , jugadores_aux);
            rename("jugadores_aux.csv", argv[2]);
        }
        else if (strcmp(argv[1] , ELIMINAR_PARTIDA)== 0 && argc == 3){
            
            strcpy(nombre_a_eliminar , argv[3]);
            eliminar_partida(jugadores , jugadores_aux ,nombre_a_eliminar);
            rename("jugadores_aux.csv", argv[2]);
        }
        else if (strcmp(argv[1] , ORDENAR_PARTIDA)== 0 && argc == 2){
            
            ordenar_partida(jugadores,jugadores_aux);
            rename("jugadores_aux.csv", argv[2]);
        }
        fclose(jugadores);
        fclose(jugadores_aux);
        
    }
    else{

        srand (( unsigned)time(NULL));
        juego_t juego;
        
        inicializar_juego(&juego);
        imprimir_terreno(juego);
        printf("Estas en nivel: %i\n",juego.nivel_actual );
                printf("Puntos: %i\n" , juego.personaje.puntos);
                printf("Mac tiene %i vidas\n" , juego.personaje.vida);
        while(estado_juego(juego) == 0){
            while(estado_nivel(juego) == 0){
                realizar_jugada(&juego);
                imprimir_terreno(juego);
                printf("Estas en nivel: %i\n",juego.nivel_actual );
                printf("Puntos: %i\n" , juego.personaje.puntos);
                printf("Mac tiene %i vidas\n" , juego.personaje.vida);
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

    }
    
    return 0;

} 





