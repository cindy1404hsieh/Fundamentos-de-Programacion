#include "la_sombra_de_mac.h"
#include "utiles.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ARRIBA 'W'
#define ABAJO 'S'
#define DERECHA 'D'
#define IZQUIERDA 'A'
#define MAC 'M'
#define CONST_MAC "\U0001f466"
#define BLOO 'B'
#define CONST_BLOO "\U0001f47b"
#define VIDAS_MAXIMA 3
#define POSICION_MINIMA 0
#define NIVEL_1 1
#define NIVEL_2 2
#define NIVEL_3 3
#define VELA 'V' 
#define CONST_VELA "\U0001f56f"
#define POZO 'W'
#define CONST_POZO "\U0001f573"
#define INTERRUPTOR 'O' 
#define CONST_INTERRUPTOR "\U0001f47e"
#define PORTAL 'P'
#define CONST_PORTAL "\U0001f430"
#define ESCALERA 'E'
#define CONST_ESCALERA "\U0001fa9c"
#define MONEDA 'C'
#define CONST_MONEDA "\U0001fa99"
#define PUERTA 'D'
#define CONST_PUERTA "\U0001f6aa"
#define LLAVE 'L'
#define CONST_LLAVE "\U0001f511"
#define PAREDES '^'
#define CONST_PAREDES "\U0001f9f1"
#define COMPRAR_VIDA 'V'
#define NO_ENCONTRADO -1
#define GANADO 1
#define PERDIDO -1
#define JUGANDO 0
#define RANGO_NUM_ALEATORIO 11
#define VALOR_MINIMO_NUM_ALEATORIO 10
#define TERRENO '-'
#define CONST_TERRENO "\U0001f578"
#define MAX_LINEA 100
#define CANTIDAD_PUERTA 1
#define POZOS "POZOS"
#define VELAS "VELAS"
#define INTERRUPTORES "INTERRUPTORES"
#define PORTALES "PORTALES"
#define ESCALERAS "ESCALERAS"
#define MONEDAS "MONEDAS"
#define LLAVES "LLAVES"
#define PRIMER_NIVEL "N1"
#define SEGUNDO_NIVEL "N2"
#define TERCER_NIVEL "N3"

/*
*    Función que recibe dos coordenadas, devuelve true si las
* coordenadas son iguales, sino false.
*/
bool coordenadas_iguales(coordenada_t primera_coordenada, coordenada_t segunda_coordenada){

    return (primera_coordenada.fila == segunda_coordenada.fila) && (primera_coordenada.col==segunda_coordenada.col);

}

/*
*   Procedimiento que dejará la estructura personaje_t en un
* estado válido para iniciar el juego.
* 'arranque_personaje' es un parámetro que representa la posición 
* en la arrancará el personaje.
*/
void inicializar_personaje(personaje_t* ref_personaje, coordenada_t arranque_personaje){

    (*ref_personaje).vida=VIDAS_MAXIMA;
    (*ref_personaje).puntos=0;
    (*ref_personaje).tiene_llave=false;
    (*ref_personaje).interruptor_apretado=false;
    (*ref_personaje).posicion= arranque_personaje;

}

/*
*   Procedimiento que dejará una estructura elemento_t que
* represente a la sombra, en un estado válido para iniciar el juego. 
* 'arranque_sombra' es un parámetro que representa la posición 
* en la arrancará la sombra.
*/
void inicializar_sombra(sombra_t* ref_sombra, coordenada_t arranque_sombra){

    (*ref_sombra).posicion=arranque_sombra;
    (*ref_sombra).esta_viva = true;

}

/*
*   La función recibe un caracter y devuelve true
* si el movimiento recibido concuerda con la convención
* propuesta, sino false.
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*/
bool es_movimiento_valido(char movimiento){

    return movimiento== ARRIBA || movimiento== IZQUIERDA || movimiento== ABAJO || movimiento== DERECHA || movimiento== COMPRAR_VIDA;

}

/*
*   Procedimineto que se encargará de pedirle al usuario
* que ingrese un movimiento hasta que sea válido.
*/
void pedir_movimiento(char* ref_movimiento,personaje_t* personaje){

    if ((*personaje).puntos >= 200 && (*personaje).vida < VIDAS_MAXIMA){ 
        printf("Ingrese un movimiento en mayuscula:\n");
        printf("W: mover hacia arriba\n");
        printf("A: mover hacia izquierda\n");
        printf("S: mover hacia abajo\n");
        printf("D: mover hacia derecha\n");
        printf("Apretar V para comprar vida ( 200 puntos/vida ):\n");
        scanf(" %c",ref_movimiento);     

        if ((*ref_movimiento) == COMPRAR_VIDA){
            (*personaje).puntos -= 200;
            (*personaje).vida += 1;
        }
        while(!es_movimiento_valido(*ref_movimiento)){

            printf("Ingrese un movimiento en mayuscula(W , S , A o D):\n");
            printf("Apretar V para comprar vida ( 200 puntos/vida ):\n");
            scanf(" %c",ref_movimiento);
        } 
    }else{
        printf("Ingrese un movimiento en mayuscula:\n");
        printf("W: mover hacia arriba\n");
        printf("A: mover hacia izquierda\n");
        printf("S: mover hacia abajo\n");
        printf("D: mover hacia derecha\n");
        scanf(" %c",ref_movimiento);
        while(!es_movimiento_valido(*ref_movimiento)){

            printf("Ingrese un movimiento en mayuscula(W , S , A o D):\n");
            scanf(" %c",ref_movimiento);
        }
    }

}

/*
*    La función recibe una coordenada, devuelve true
* si la posición dentro de las dimensiones dadas, sino false.
* Una posición nunca podrá tener un valor negativo y no tendrá un 
* valor más alto que los máximos.
*/
bool esta_dentro_rango(coordenada_t posicion, int max_alto, int max_ancho){

    return (posicion.fila < max_alto) && (posicion.fila >= 0) && (posicion.col >= 0)&& (posicion.col < max_ancho);
}

/* Funcion recibe la fila de la posicion de un sujeto , devuelve true si el sujeto esta dentro de las 
 * dimensiones maximas dadas, sino false.
 */
bool no_esta_en_max_fila(int posicion_sujeto){
    return posicion_sujeto != MAX_FILAS-1;
}

/* Funcion recibe la fila de la posicion de un sujeto , devuelve true si el sujeto esta dentro de las 
 * dimensiones minimas dadas, sino false.
 */
bool no_esta_en_min_fila(int posicion_sujeto){
    return posicion_sujeto != POSICION_MINIMA;
}

/* Funcion recibe la columna de la posicion de un sujeto , devuelve true si el sujeto esta dentro de las 
 * dimensiones maximas dadas, sino false.
 */
bool no_esta_en_max_columna(int posicion_sujeto){
    return posicion_sujeto != MAX_COLUMNAS -1;
}

/* Funcion recibe la columna de la posicion de un sujeto , devuelve true si el sujeto esta dentro de las 
 * dimensiones minimas dadas, sino false.
 */
bool no_esta_en_min_columna (int posicion_sujeto){
    return posicion_sujeto != POSICION_MINIMA; 
}
/* Funcion devuelve true si a la derecha del sujeto esta por chocar con una pared, sino false.
 */
bool derecha_choca_pared(int posicion_sujeto_columna,int pared_columna,int posicion_sujeto_fila , int pared_fila){    
    return (posicion_sujeto_columna + 1) == pared_columna && posicion_sujeto_fila == pared_fila;
}
/* Funcion devuelve true si a la izquierda del sujeto esta por chocar con una pared, sino false.
 */
bool izquierda_choca_pared(int posicion_sujeto_columna , int pared_columna,int posicion_sujeto_fila , int pared_fila){    
    return (posicion_sujeto_columna - 1) == pared_columna && posicion_sujeto_fila == pared_fila;
}
/* Funcion devuelve true si arriba del sujeto esta por chocar con una pared, sino false.
 */
bool arriba_choca_pared(int posicion_sujeto_fila , int pared_fila,int posicion_sujeto_columna , int pared_columna){    
    return (posicion_sujeto_fila - 1) == pared_fila && posicion_sujeto_columna == pared_columna;
}
/* Funcion devuelve true si a abajo del sujeto esta por chocar con una pared, sino false.
 */
bool abajo_choca_pared(int posicion_sujeto_fila , int pared_fila , int posicion_sujeto_columna , int pared_columna){    
    return (posicion_sujeto_fila + 1) == pared_fila && posicion_sujeto_columna == pared_columna;
}
/* Funcion devuelve true si alguna pared esta de arriba del sujeto, sino false.
 */
bool se_choca_arriba_pared(int sujeto_fila , int sujeto_col,coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    bool se_choca = false;
    for (int i=0 ;i<tope_paredes;i++){
        if (arriba_choca_pared(sujeto_fila ,paredes[i].fila,sujeto_col ,paredes[i].col) &&!se_choca){
            se_choca = true;
        }  
    }
    return se_choca;
}
/* Funcion devuelve true si alguna pared esta de abajo del sujeto, sino false.
 */
bool se_choca_abajo_pared(int sujeto_fila , int sujeto_col,coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    bool se_choca = false;
    for (int i=0 ;i<tope_paredes;i++){
        if (abajo_choca_pared(sujeto_fila ,paredes[i].fila,sujeto_col ,paredes[i].col) &&!se_choca){
            se_choca = true;
        }  
    }
    return se_choca;
}
/* Funcion devuelve true si alguna pared esta de derecha del sujeto, sino false.
 */
bool se_choca_derecha_pared(int sujeto_fila , int sujeto_col,coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    bool se_choca = false;
    for (int i=0 ;i<tope_paredes;i++){
        if (derecha_choca_pared(sujeto_col ,paredes[i].col,sujeto_fila ,paredes[i].fila) &&!se_choca){
            se_choca = true;
        }  
    }
    return se_choca;
}
/* Funcion devuelve true si alguna pared esta de izquierda del sujeto, sino false.
 */
bool se_choca_izquierda_pared(int sujeto_fila , int sujeto_col,coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    bool se_choca = false;
    for (int i=0 ;i<tope_paredes;i++){
        if (izquierda_choca_pared(sujeto_col ,paredes[i].col,sujeto_fila ,paredes[i].fila) &&!se_choca){
            se_choca = true;
        }  
    }
    return se_choca;
}

/* Funcion devuelve true si a arriba del sujeto esta por chocar con una escalera, sino false.
 */
bool se_encuentra_escalera_arriba(int personaje_col ,int escalera_col,int personaje_fila ,int escalera_fila){
    return personaje_col == escalera_col && personaje_fila-1 == escalera_fila;
}
/* Funcion devuelve true si abajo del sujeto esta por chocar con una escalera, sino false.
 */
bool se_encuentra_escalera_abajo(int personaje_col ,int escalera_col,int personaje_fila ,int escalera_fila){
    return personaje_col == escalera_col && personaje_fila+1 == escalera_fila;
}
/* Funcion devuelve true si a la derecha del sujeto esta por chocar con una escalera, sino false.
 */
bool se_encuentra_escalera_derecha(int personaje_col ,int escalera_col,int personaje_fila ,int escalera_fila){
    return personaje_col+1 == escalera_col && personaje_fila == escalera_fila;
}
/* Funcion devuelve true si a la izquierda del sujeto esta por chocar con una escalera, sino false.
 */
bool se_encuentra_escalera_izquierda(int personaje_col ,int escalera_col,int personaje_fila ,int escalera_fila){
    return personaje_col-1 == escalera_col && personaje_fila == escalera_fila;
}

/* Funcion devuelve la posicion de alguna escalera si tal escalera esta de arriba del sujeto.
 */
int buscar_arriba_escalera(int personaje_fila , int personaje_col,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    bool se_encuentra = false;
    int posicion_encontrado = NO_ENCONTRADO;
    for (int i=0 ;i<(*tope_herramientas);i++){
        if (herramientas[i].tipo == ESCALERA){
            if (se_encuentra_escalera_arriba(personaje_col ,herramientas[i].coordenada.col,personaje_fila ,herramientas[i].coordenada.fila) &&!se_encuentra){
                posicion_encontrado = i;
                se_encuentra = true;
            }  
        }
    }   
    return posicion_encontrado;
}
/* Funcion devuelve la posicion de alguna escalera si tal escalera esta de abajo del sujeto.
 */
int buscar_abajo_escalera(int personaje_fila , int personaje_col,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    bool se_encuentra = false;
    int posicion_encontrado = NO_ENCONTRADO;
    for (int i=0 ;i<(*tope_herramientas);i++){
        if (herramientas[i].tipo == ESCALERA){
            if (se_encuentra_escalera_abajo(personaje_col ,herramientas[i].coordenada.col,personaje_fila ,herramientas[i].coordenada.fila) &&!se_encuentra){
                posicion_encontrado = i;
                se_encuentra = true;
            }  
        }
    }   
    return posicion_encontrado;
}
/* Funcion devuelve la posicion de alguna escalera si tal escalera esta de izquierda del sujeto.
 */
int buscar_izquierda_escalera(int personaje_fila , int personaje_col,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    bool se_encuentra = false;
    int posicion_encontrado = NO_ENCONTRADO;
    for (int i=0 ;i<(*tope_herramientas);i++){
        if (herramientas[i].tipo == ESCALERA){
            if (se_encuentra_escalera_izquierda(personaje_col ,herramientas[i].coordenada.col,personaje_fila ,herramientas[i].coordenada.fila) &&!se_encuentra){
                posicion_encontrado = i;
                se_encuentra = true;
            }  
        }
    }   
    return posicion_encontrado;
}
/* Funcion devuelve la posicion de alguna escalera si tal escalera esta de derecha del sujeto.
 */
int buscar_derecha_escalera(int personaje_fila , int personaje_col,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    bool se_encuentra = false;
    int posicion_encontrado = NO_ENCONTRADO;
    for (int i=0 ;i<(*tope_herramientas);i++){
        if (herramientas[i].tipo == ESCALERA){
            if (se_encuentra_escalera_derecha(personaje_col ,herramientas[i].coordenada.col,personaje_fila ,herramientas[i].coordenada.fila) &&!se_encuentra){
                posicion_encontrado = i;
                se_encuentra = true;
            }  
        }
    }   
    return posicion_encontrado;
}
/* dada la posicion que quiera eliminar de un vector de elementos , el vetor y su tope, elimina ese elemento.
 */
void eliminar_elemento(elemento_t elementos[MAX_ELEMENTOS] , int* tope , int posicion_elemento_a_eliminar){   
        
    elementos[posicion_elemento_a_eliminar] = elementos[(*tope)-1];
    (*tope)--;
      
}

/*Procedimiento el cual 
* se encargara de mover el personaje un paso hacia arriba.
* 
*/
void mac_mover_hacia_arriba(int* personaje_fila, int personaje_col, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    int posicion_escalera_encontrado = buscar_arriba_escalera(*personaje_fila , personaje_col,herramientas , &(*tope_herramientas));
    int posicion_escalera_encontrado_mas_uno = buscar_arriba_escalera(*personaje_fila-1 , personaje_col,herramientas , &(*tope_herramientas));
    if (no_esta_en_min_fila(*personaje_fila) && buscar_arriba_escalera(*personaje_fila ,personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && !se_choca_arriba_pared(*personaje_fila-1 ,personaje_col,paredes,tope_paredes)){
        (*personaje_fila) -= 2;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
    }
    else if (no_esta_en_min_fila(*personaje_fila) && buscar_arriba_escalera(*personaje_fila ,personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && buscar_arriba_escalera(*personaje_fila-1 ,personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO){
        (*personaje_fila) -= 3;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado_mas_uno);
    }
    else if( no_esta_en_min_fila(*personaje_fila) && !se_choca_arriba_pared(*personaje_fila ,personaje_col,paredes,tope_paredes) ){
        (*personaje_fila)--;       
    }
}
/*Procedimiento el cual 
* se encargara de mover el personaje un paso hacia abajo.
* 
*/
void mac_mover_hacia_abajo(int* personaje_fila, int personaje_col, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    int posicion_escalera_encontrado = buscar_abajo_escalera(*personaje_fila ,personaje_col,herramientas , &(*tope_herramientas));
    int posicion_escalera_encontrado_mas_uno = buscar_abajo_escalera(*personaje_fila+1 , personaje_col,herramientas , &(*tope_herramientas));
    if (no_esta_en_max_fila(*personaje_fila) && buscar_abajo_escalera(*personaje_fila , personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && !se_choca_abajo_pared(*personaje_fila+1 ,personaje_col,paredes,tope_paredes)){
        (*personaje_fila) += 2;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
    }
    else if(no_esta_en_max_fila(*personaje_fila) && buscar_abajo_escalera(*personaje_fila , personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && buscar_abajo_escalera(*personaje_fila-1 , personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO){
        (*personaje_fila) += 3;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado_mas_uno);
    }
    else if( no_esta_en_max_fila(*personaje_fila) && !se_choca_abajo_pared(*personaje_fila ,personaje_col,paredes,tope_paredes)){
        (*personaje_fila) ++;
    }
}
/*Procedimiento el cual 
* se encargara de mover el personaje un paso hacia izquierda.
* 
*/
void mac_mover_hacia_izquierda(int* personaje_col,int personaje_fila, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){

    int posicion_escalera_encontrado = buscar_izquierda_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas));
    int posicion_escalera_encontrado_mas_uno= buscar_izquierda_escalera(personaje_fila , *personaje_col-1,herramientas , &(*tope_herramientas));
    if (no_esta_en_min_columna(*personaje_col) && buscar_izquierda_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && !se_choca_izquierda_pared(personaje_fila,*personaje_col-1,paredes,tope_paredes)){
        (*personaje_col) -= 2;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
    }
    else if (no_esta_en_min_columna(*personaje_col) && buscar_izquierda_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && buscar_izquierda_escalera(personaje_fila , *personaje_col-1,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO){
        (*personaje_col) -= 3;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado_mas_uno);
    }
    else if (no_esta_en_min_columna(*personaje_col) && !se_choca_izquierda_pared(personaje_fila,*personaje_col,paredes,tope_paredes)){
        (*personaje_col) --;
    }
}
/*Procedimiento el cual 
* se encargara de mover el personaje un paso hacia derecha.
*/
void mac_mover_hacia_derecha(int* personaje_col,int personaje_fila, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){

    int posicion_escalera_encontrado = buscar_derecha_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas));
    int posicion_escalera_encontrado_mas_uno= buscar_derecha_escalera(personaje_fila , *personaje_col+1,herramientas , &(*tope_herramientas));
    if (no_esta_en_min_columna(*personaje_col) && buscar_derecha_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && !se_choca_derecha_pared(personaje_fila ,*personaje_col+1 ,paredes,tope_paredes)){
        (*personaje_col) += 2;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
    }
    else if (no_esta_en_min_columna(*personaje_col) && buscar_derecha_escalera(personaje_fila , *personaje_col,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO && buscar_derecha_escalera(personaje_fila , *personaje_col+1,herramientas , &(*tope_herramientas)) != NO_ENCONTRADO){
        (*personaje_col) += 3;
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado);
        eliminar_elemento(herramientas , &(*tope_herramientas) , posicion_escalera_encontrado_mas_uno);
    }
    else if ( no_esta_en_max_columna(*personaje_col) && !se_choca_derecha_pared(personaje_fila ,*personaje_col,paredes,tope_paredes)){
        (*personaje_col)++;        
    }
}

/*Procedimiento el cual se encargara de mover sombra un paso hacia abajo.*/
void bloo_mover_hacia_abajo( int* sombra_fila,int sombra_col, coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    if(no_esta_en_max_fila(*sombra_fila) && !se_choca_abajo_pared(*sombra_fila ,sombra_col,paredes,tope_paredes)){
        (*sombra_fila)++;
    }
}

/*Procedimiento el cual se encargara de mover sombra un paso hacia arriba.*/
void bloo_mover_hacia_arriba( int* sombra_fila,int sombra_col, coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    if(no_esta_en_min_fila(*sombra_fila)&& !se_choca_arriba_pared(*sombra_fila ,sombra_col,paredes,tope_paredes)){
        (*sombra_fila)--;
    }
}

/*Procedimiento el cual se encargara de mover sombra un paso hacia derecha.*/
void bloo_mover_hacia_derecha( int* sombra_col,int sombra_fila, coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    if (no_esta_en_max_columna(*sombra_col)&& !se_choca_derecha_pared(sombra_fila ,*sombra_col,paredes,tope_paredes)){
        (*sombra_col)++;
    }
}
/*Procedimiento el cual se encargara de mover sombra un paso hacia izquierda.*/
void bloo_mover_hacia_izquierda( int* sombra_col,int sombra_fila, coordenada_t paredes[MAX_PAREDES] , int tope_paredes){
    if (no_esta_en_min_columna(*sombra_col)&& !se_choca_izquierda_pared(sombra_fila,*sombra_col,paredes,tope_paredes)){
        (*sombra_col)--;
    }
}



/*
*    El procedimiento recibe el personaje y la sombra por referencia y el movimiento,
* y según este último, los mueve acordemente.
* 
* El personaje se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*
*   La sombra se mueve de igual forma para el eje vertical, pero
* para el eje horizontal se moverá de forma inversa:
*
* -> A: Si el personaje debe moverse para la derecha.
* -> D: Si el personaje debe moverse para la izquierda.
*/


void mover_personajes_espejo(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    if(movimiento==ARRIBA){
        
        mac_mover_hacia_arriba(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_arriba(&(*ref_sombra).posicion.fila,(*ref_sombra).posicion.col,paredes,tope_paredes);


    }else if(movimiento==IZQUIERDA){
        mac_mover_hacia_izquierda(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_derecha(&(*ref_sombra).posicion.col,(*ref_sombra).posicion.fila,paredes,tope_paredes);

    }else if(movimiento==ABAJO){
        mac_mover_hacia_abajo(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_abajo(&(*ref_sombra).posicion.fila,(*ref_sombra).posicion.col,paredes,tope_paredes);
        
        
    }else if(movimiento==DERECHA){

        mac_mover_hacia_derecha(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_izquierda(&(*ref_sombra).posicion.col,(*ref_sombra).posicion.fila,paredes,tope_paredes);                     
    }
}

/*
*    El procedimiento recibe el personaje y la sombra por referencia y el movimiento,
* y según este último, los mueve acordemente.
* 
* El personaje se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*
*   La sombra se mueve de igual forma .
*/
void mover_personajes_igualados(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){

    if(movimiento==ARRIBA){

        mac_mover_hacia_arriba(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_arriba(&(*ref_sombra).posicion.fila,(*ref_sombra).posicion.col,paredes,tope_paredes);
                
    }else if(movimiento==IZQUIERDA){

        mac_mover_hacia_izquierda(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_izquierda(&(*ref_sombra).posicion.col,(*ref_sombra).posicion.fila,paredes,tope_paredes);

    }else if(movimiento==ABAJO){

        mac_mover_hacia_abajo(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_abajo(&(*ref_sombra).posicion.fila,(*ref_sombra).posicion.col,paredes,tope_paredes);
        
    }else if(movimiento==DERECHA){

        mac_mover_hacia_derecha(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        bloo_mover_hacia_derecha(&(*ref_sombra).posicion.col,(*ref_sombra).posicion.fila,paredes,tope_paredes);                  
    }
}

/*
*    El procedimiento recibe el personaje por referencia y el movimiento,
* y según este último, lo mueve acordemente.
* 
* El personaje se mueve de la forma:
* -> W: Si el personaje debe moverse para la arriba. 
* -> A: Si el personaje debe moverse para la izquierda.
* -> S: Si el personaje debe moverse para la abajo.
* -> D: Si el personaje debe moverse para la derecha.
*/
void mover_solo_mac(personaje_t* ref_personaje, sombra_t ref_sombra, char movimiento, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){
    if(movimiento==ARRIBA){

        mac_mover_hacia_arriba(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
                
    }else if(movimiento==IZQUIERDA){

        mac_mover_hacia_izquierda(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));

    }else if(movimiento==ABAJO){

       mac_mover_hacia_abajo(&(*ref_personaje).posicion.fila,(*ref_personaje).posicion.col,paredes,tope_paredes,herramientas , &(*tope_herramientas));
        
    }else if(movimiento==DERECHA){

        mac_mover_hacia_derecha(&(*ref_personaje).posicion.col,(*ref_personaje).posicion.fila,paredes,tope_paredes,herramientas , &(*tope_herramientas));                     
    }
}

/*
*    El procedimiento recibe el personaje y la sombra por referencia y el movimiento,
* y según este último, los mueve acordemente.
*/
void mover_personaje(personaje_t* ref_personaje, sombra_t* ref_sombra, char movimiento, coordenada_t paredes[MAX_PAREDES] , int tope_paredes,elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas){

    
    if (!(*ref_personaje).interruptor_apretado && (*ref_sombra).esta_viva){
        mover_personajes_espejo(&(*ref_personaje), &(*ref_sombra), movimiento,paredes,tope_paredes,herramientas , &(*tope_herramientas));

    }else if ((*ref_personaje).interruptor_apretado && (*ref_sombra).esta_viva){
        mover_personajes_igualados(&(*ref_personaje), &(*ref_sombra), movimiento,paredes,tope_paredes,herramientas , &(*tope_herramientas));
    }
    else if (!(*ref_sombra).esta_viva){
        mover_solo_mac(&(*ref_personaje), (*ref_sombra), movimiento,paredes,tope_paredes,herramientas , &(*tope_herramientas));
    }
}
/*
*   Función que dados una altura máxima y un ancho máximo
* devuelve una coordenada aleatoria. Una posición nunca podrá
* tener un valor negativo, ni ser mayor a sus máximos.
*/
coordenada_t coordenada_aleatoria(int max_alto, int max_ancho){

    coordenada_t coordenada_aleatoria; 
    coordenada_aleatoria.fila = rand() % max_alto;
    coordenada_aleatoria.col = rand() %  max_ancho;
    return coordenada_aleatoria;

}

/*
*    La función recibe un vector de elementos, su respectivo tope y una coordenada,
* devuelve la posición del elemento del vector, que coincida con la coordenada pasada 'posicion',
* si no se encuentra ningún elemento, se devolverá -1. 
*/
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

/*La función recibe un vector de coordenadas, su respectivo tope y una coordenada,
devuelve la posición de la coordenada del vector, que coincida con la coordenada pasada 'posicion',
si no se encuentra ningúna coordenada, se devolverá -1. */
int buscar_coordenada_paredes(coordenada_t paredes[MAX_PAREDES], int tope, coordenada_t posicion_buscado){
    
    int posicion_encontrado= NO_ENCONTRADO;
    bool encontrado = false;
    for (int i = 0 ; i < tope ; i++){
        if (coordenadas_iguales(paredes[i] , posicion_buscado) && !encontrado){
            posicion_encontrado = i;
            encontrado = true;
        }
    }   
    return posicion_encontrado;
}
/*La función recibe un vector de numeros, su respectivo tope y un numero,
devuelve la posición del numero del vector, que coincida con el numero,
si no se encuentra ningún numero, se devolverá -1. */
int buscar_numero(int anotador_numeros[MAX_PAREDES] , int tope , int numero_buscado){
    int pos_encontrado = NO_ENCONTRADO;
    bool encontrado = false;
    for (int i = 0 ; i < tope ; i++){
        if (anotador_numeros[i] == numero_buscado && !encontrado){
            pos_encontrado = i;
            encontrado = true;
        }
    }   
    return pos_encontrado;
}
/*La función recibe un vector de numeros, su respectivo tope y un numero,
devuelve true si el numero coincide con algun numero del vector,
si no false. */
bool repite(int anotador_numeros[MAX_PAREDES] , int tope , int numero_buscado){
    return buscar_numero(anotador_numeros ,tope ,numero_buscado) != NO_ENCONTRADO;
}
/*la funcion recibe la posicon de sombra y personaje , el nivel ,
devuelve true si son ambas posiciones vacias , si no false*/
bool son_posiciones_vacias(nivel_t nivel ,coordenada_t posicion_personaje,coordenada_t posicion_sombra){
    return buscar_coordenada_paredes(nivel.paredes,nivel.tope_paredes,posicion_personaje)== NO_ENCONTRADO && buscar_elemento(nivel.obstaculos , nivel.tope_obstaculos , posicion_personaje)== NO_ENCONTRADO && buscar_elemento(nivel.herramientas , nivel.tope_herramientas , posicion_personaje)== NO_ENCONTRADO &&buscar_coordenada_paredes(nivel.paredes,nivel.tope_paredes,posicion_sombra)== NO_ENCONTRADO && buscar_elemento(nivel.obstaculos , nivel.tope_obstaculos , posicion_sombra)== NO_ENCONTRADO && buscar_elemento(nivel.herramientas , nivel.tope_herramientas , posicion_sombra)== NO_ENCONTRADO;
}

/*la funcion recibe una posicon, el nivel , y el tope de obstaculos
devuelve true si es posicion vacia , si no false*/
bool es_posicion_vacia_en_paredes_y_obstaculos(nivel_t nivel ,int tope_obstaculos,coordenada_t posicion_generada){

    return (buscar_coordenada_paredes(nivel.paredes , nivel.tope_paredes ,posicion_generada )==NO_ENCONTRADO) && (buscar_elemento(nivel.obstaculos, tope_obstaculos, posicion_generada)== NO_ENCONTRADO);
}
/*
*   Procedimiento que inicializa las coordenadas de los obstaculos.
*/
void inicializar_coordenadas_obstaculos(nivel_t* nivel){

    coordenada_t posicion_generada;
    posicion_generada = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);

    for (int i=0 ; i<(*nivel).tope_obstaculos ;i++){
        while (!es_posicion_vacia_en_paredes_y_obstaculos((*nivel),i,posicion_generada)){
            posicion_generada = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
        }
        (*nivel).obstaculos[i].coordenada = posicion_generada;
    }

}
/*la funcion recibe una posicon, el nivel , y el tope de herramientas
devuelve true si es posicion vacia , si no false*/
bool es_posicion_vacia_en_paredes_obstaculos_y_paredes(nivel_t nivel ,int tope_herramientas,coordenada_t posicion_generada){

    return (buscar_coordenada_paredes(nivel.paredes , nivel.tope_paredes ,posicion_generada )==NO_ENCONTRADO) && (buscar_elemento(nivel.obstaculos, nivel.tope_obstaculos, posicion_generada)== NO_ENCONTRADO)&& (buscar_elemento(nivel.herramientas, tope_herramientas, posicion_generada)== NO_ENCONTRADO);
}
/*
*   Procedimiento que inicializa las coordenadas de las herramientas.
*/
void inicializar_coordenadas_herramientas(nivel_t* nivel){

    coordenada_t posicion_generada;
    posicion_generada = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);

    int pos_pared = rand() % (*nivel).tope_paredes;
    int anotador_numeros[MAX_PAREDES];
    int tope_anotador = 0 ;

    for (int i=0 ; i<(*nivel).tope_herramientas ; i++){
        if ((*nivel).herramientas[i].tipo == ESCALERA){
            while(repite(anotador_numeros , tope_anotador , pos_pared)){
                pos_pared = rand() % (*nivel).tope_paredes;
            }
            (*nivel).herramientas[i].coordenada = (*nivel).paredes[pos_pared];
            anotador_numeros[tope_anotador] = pos_pared;
            tope_anotador++;
        }
        else {
            while(!es_posicion_vacia_en_paredes_obstaculos_y_paredes((*nivel), i ,posicion_generada)){
                posicion_generada = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
            }
            (*nivel).herramientas[i].coordenada = posicion_generada;  
        }
    }

}  
/*
*   Procedimiento que inicializa las coordenadas de las paredes.
*/
void inicializar_paredes(coordenada_t paredes[MAX_PAREDES] , int* tope_paredes){
    
    obtener_mapa(paredes, &(*tope_paredes));
}
/*
*   Procedimiento que inicializa los tipos de los obstaculos.
*/
void inicializar_tipo_obstaculos(elemento_t obstaculos[MAX_ELEMENTOS] ,int* tope_obstaculos,int cantidad_velas,int cantidad_pozos,int cantidad_interruptores,int cantidad_portales){

    (*tope_obstaculos) = cantidad_velas;
    for (int i = 0 ; i < (*tope_obstaculos) ; i++){
        obstaculos[i].tipo = VELA;     
    }

    (*tope_obstaculos) = cantidad_velas+cantidad_pozos;
    for (int i = cantidad_velas ; i < (*tope_obstaculos) ; i++){
        obstaculos[i].tipo = POZO;
    }

    (*tope_obstaculos) = cantidad_velas+cantidad_pozos+cantidad_interruptores;
    for (int i = cantidad_velas+cantidad_pozos ; i < (*tope_obstaculos) ; i++){
        obstaculos[i].tipo = INTERRUPTOR;
    }

    if(cantidad_portales != 0){
        (*tope_obstaculos) = cantidad_velas+cantidad_pozos+cantidad_interruptores+cantidad_portales;
        for (int i = cantidad_velas+cantidad_pozos+cantidad_interruptores ; i < (*tope_obstaculos) ; i++){
            obstaculos[i].tipo = PORTAL;
        }
    }
}
/*
*   Procedimiento que inicializa los tipos de las herramientas.
*/
void inicializar_tipo_herramientas(elemento_t herramientas[MAX_ELEMENTOS] ,int* tope_herramientas ,int cantidad_escaleras,int cantidad_monedas, int cantidad_puerta,int cantidad_llave){

    (*tope_herramientas) = cantidad_escaleras;
    
    for (int i = 0 ; i < (*tope_herramientas) ; i++){
        
        herramientas[i].tipo = ESCALERA;        
    }
    
    (*tope_herramientas) = cantidad_escaleras+cantidad_monedas;
    for (int i = cantidad_escaleras ; i < (*tope_herramientas) ; i++){
        herramientas[i].tipo = MONEDA;
    }

    (*tope_herramientas) = cantidad_escaleras+cantidad_monedas+cantidad_puerta;
    for (int i = cantidad_escaleras+cantidad_monedas ; i < (*tope_herramientas) ; i++){
        herramientas[i].tipo = PUERTA;
    }

    if (cantidad_llave != 0){
        (*tope_herramientas) = cantidad_escaleras+cantidad_monedas+cantidad_puerta+cantidad_llave;
        for (int i = cantidad_escaleras+cantidad_monedas+cantidad_puerta ; i < (*tope_herramientas) ; i++){
            herramientas[i].tipo = LLAVE;
        }
    }
    
}
/*
*   Procedimiento que dejará a un nivel con sus cantidades de elementos 
*   en un estado válido para iniciar el juego. 
*/
void inicializar_elementos(nivel_t* nivel,int numero_nivel,int cantidad_velas,int cantidad_pozos,int cantidad_interruptores,int cantidad_portales,int cantidad_escaleras,int cantidad_monedas, int cantidad_puerta,int cantidad_llave){
    
    inicializar_paredes((*nivel).paredes , &(*nivel).tope_paredes);

    (*nivel).numero_nivel = numero_nivel;

    inicializar_tipo_obstaculos((*nivel).obstaculos ,&(*nivel).tope_obstaculos,cantidad_velas, cantidad_pozos,cantidad_interruptores,cantidad_portales);

    inicializar_tipo_herramientas((*nivel).herramientas ,&(*nivel).tope_herramientas,cantidad_escaleras,cantidad_monedas, cantidad_puerta,cantidad_llave);

    inicializar_coordenadas_obstaculos(&(*nivel));
    
    inicializar_coordenadas_herramientas(&(*nivel));
    
}
void cargar_elementos_configurados(configuracion_nivel_t* nivel , char* elemento , char* cantidad){
    if (strcmp(elemento, POZOS) == 0){
            strcpy((*nivel).pozos, elemento);
            (*nivel).cantidad_pozos = atoi(cantidad);
            
        }
        if (strcmp(elemento, VELAS) == 0){
            strcpy((*nivel).velas, elemento);
            (*nivel).cantidad_velas = atoi(cantidad);
        }
        if (strcmp(elemento, INTERRUPTORES) == 0){
            strcpy((*nivel).interruptores, elemento);
            (*nivel).cantidad_interruptores = atoi(cantidad);
        }
        if (strcmp(elemento, PORTALES) == 0){
            strcpy((*nivel).portales, elemento);
            (*nivel).cantidad_portales = atoi(cantidad);
        }
        if (strcmp(elemento, MONEDAS) == 0){
            strcpy((*nivel).monedas, elemento);
            (*nivel).cantidad_monedas = atoi(cantidad);
        }
        if (strcmp(elemento, ESCALERAS) == 0){
            strcpy((*nivel).escaleras, elemento);
            (*nivel).cantidad_escaleras = atoi(cantidad);
            
        }
        if (strcmp(elemento, LLAVES) == 0){
            strcpy((*nivel).llaves, elemento);
            (*nivel).cantidad_llaves = atoi(cantidad);
        }
}
void inicializar_configuracion(FILE* config , nivel_t niveles[MAX_NIVELES]){
    
    char linea[MAX_LINEA];
    configuracion_t configuracion;
 
    char* numero_nivel;

    char* elemento;
    char* cantidad;
    
    
    
    while(!feof(config)){

        fscanf(config , "%s\n" , linea);
        numero_nivel = strtok(linea , "_");
        elemento = strtok(NULL , "=");
        cantidad = strtok(NULL , "\0");
        
        if(strcmp(numero_nivel, PRIMER_NIVEL) == 0){

            cargar_elementos_configurados(&configuracion.nivel_uno,elemento,cantidad);
            
        }else if(strcmp(numero_nivel, SEGUNDO_NIVEL) == 0){
            cargar_elementos_configurados(&configuracion.nivel_dos,elemento,cantidad);

        }else if(strcmp(numero_nivel, TERCER_NIVEL) == 0){
            cargar_elementos_configurados(&configuracion.nivel_tres,elemento,cantidad);
        }

    }

    inicializar_elementos(&niveles[NIVEL_1-1],NIVEL_1,configuracion.nivel_uno.cantidad_velas,configuracion.nivel_uno.cantidad_pozos,configuracion.nivel_uno.cantidad_interruptores,configuracion.nivel_uno.cantidad_portales,configuracion.nivel_uno.cantidad_escaleras,configuracion.nivel_uno.cantidad_monedas,CANTIDAD_PUERTA,configuracion.nivel_uno.cantidad_llaves);

    inicializar_elementos(&niveles[NIVEL_2-1],NIVEL_2,configuracion.nivel_dos.cantidad_velas,configuracion.nivel_dos.cantidad_pozos,configuracion.nivel_dos.cantidad_interruptores,configuracion.nivel_dos.cantidad_portales,configuracion.nivel_dos.cantidad_escaleras,configuracion.nivel_dos.cantidad_monedas,CANTIDAD_PUERTA,configuracion.nivel_dos.cantidad_llaves);

    inicializar_elementos(&niveles[NIVEL_3-1],NIVEL_3,configuracion.nivel_tres.cantidad_velas,configuracion.nivel_tres.cantidad_pozos,configuracion.nivel_tres.cantidad_interruptores,configuracion.nivel_tres.cantidad_portales,configuracion.nivel_tres.cantidad_escaleras,configuracion.nivel_tres.cantidad_monedas,CANTIDAD_PUERTA,configuracion.nivel_tres.cantidad_llaves);

}




void inicializar_juego(juego_t* juego){
    cargar_mapas();
    coordenada_t posicion_personaje;
    posicion_personaje = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
    coordenada_t posicion_sombra;
    posicion_sombra.fila = posicion_personaje.fila;
    posicion_sombra.col = MAX_COLUMNAS - posicion_personaje.col -1 ;

    FILE* config=fopen("config.txt", "r");
    
    
    if(!config){

        for (int i=1 ; i<MAX_NIVELES+1 ;i++){
            if (i==NIVEL_1){

                inicializar_elementos(&(*juego).niveles[NIVEL_1-1],1,5,15,1,0,10,10,1,0);

            }
            else if (i==NIVEL_2){

                inicializar_elementos(&(*juego).niveles[NIVEL_2-1],2,10,20,2,2,15,15,1,1);
            }
            else if (i==NIVEL_3){

                inicializar_elementos(&(*juego).niveles[NIVEL_3-1],3,12,30,4,4,15,15,1,1);
            }  
        }
        
    }else{

        inicializar_configuracion(config,(*juego).niveles);
        fclose(config);
    }
    
    (*juego).nivel_actual = NIVEL_1;
    
    while(!son_posiciones_vacias((*juego).niveles[(*juego).nivel_actual] ,posicion_personaje,posicion_sombra)){
        posicion_personaje = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
        posicion_sombra.fila = posicion_personaje.fila;
        posicion_sombra.col = MAX_COLUMNAS - posicion_personaje.col -1 ;
    }
    
    inicializar_personaje(&(*juego).personaje, posicion_personaje);
    inicializar_sombra(&(*juego).sombra, posicion_sombra); 
    
    
}
/*funcion que dado un vector de elementos, su tope y un elemento de tipo char , 
devuelve la posicion encontrada en el vector sino encuentra devuelve -1.*/
int buscar_elemento_nombres_iguales(elemento_t elementos[MAX_ELEMENTOS], int tope, char elemento_buscado){

    int posicion_encontrado= NO_ENCONTRADO;
    for (int i = 0 ; i < tope ; i++){
        if ((elementos[i].tipo == elemento_buscado)){
            posicion_encontrado = i;
        }
    }   
    return posicion_encontrado;
}
/*funcion que dado un vector de herramientas, su tope y las coordenadas de los personajes , 
devuelve true si estan a una distancia manhattan <=1 de la puerta, sino false.*/
bool estan_a_distancia_manhattan_menor_a_uno_de_la_puerta(coordenada_t coordenada_personaje, coordenada_t coordenada_sombra , elemento_t herramientas[MAX_ELEMENTOS] , int tope_herramientas) {

    int posicion_puerta = buscar_elemento_nombres_iguales(herramientas,tope_herramientas, PUERTA);

    return (abs(coordenada_personaje.fila - herramientas[posicion_puerta].coordenada.fila) + abs(coordenada_personaje.col - herramientas[posicion_puerta].coordenada.col) <= 1) && (abs(coordenada_sombra.fila - herramientas[posicion_puerta].coordenada.fila) + abs(coordenada_sombra.col - herramientas[posicion_puerta].coordenada.col) <= 1);
}

int estado_nivel(juego_t juego){
    int estado_nivel = JUGANDO;

    if (juego.nivel_actual == NIVEL_1){
        if (estan_a_distancia_manhattan_menor_a_uno_de_la_puerta(juego.personaje.posicion, juego.sombra.posicion , juego.niveles[juego.nivel_actual-1].herramientas , juego.niveles[juego.nivel_actual-1].tope_herramientas) && juego.sombra.esta_viva){
            estado_nivel = GANADO;
        }
    }
    else {
        if (juego.personaje.tiene_llave && estan_a_distancia_manhattan_menor_a_uno_de_la_puerta(juego.personaje.posicion, juego.sombra.posicion , juego.niveles[juego.nivel_actual-1].herramientas , juego.niveles[juego.nivel_actual-1].tope_herramientas) && juego.sombra.esta_viva) {
            estado_nivel = GANADO;          
        }
    }
    return estado_nivel;
} 

int estado_juego(juego_t juego){
    int estado_juego = JUGANDO ;    

    if (juego.personaje.vida < 0 ){
        estado_juego = PERDIDO;
    }

    else if (juego.nivel_actual == NIVEL_3 && estado_nivel(juego) == GANADO){

        estado_juego = GANADO;
    }
    
    return estado_juego;
}
/*funcion que dadas las coordenadas de la sombra y de una vela, 
devuelve true si la sombra esta dentro del rango de la vela, sino false.*/
bool esta_dentro_efecto_vela(coordenada_t posicion_sombra , coordenada_t posicion_vela){

    if (abs(posicion_sombra.fila - posicion_vela.fila) <= 1 && abs(posicion_sombra.fila - posicion_vela.fila) <= 1){
        if (abs(posicion_sombra.fila - posicion_vela.fila) + abs(posicion_sombra.col - posicion_vela.col) <= 2){
            return true;
        }
    }
    return false;
}
/*procedimiento que se encargara de llenar el terreno con paredes con las coordenadas de un vector*/
void llenar_tablero_con_paredes(coordenada_t paredes[MAX_PAREDES],int tope_paredes, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for (int i = 0; i < tope_paredes; i++){
        terreno [paredes[i].fila] [paredes[i].col] = PAREDES;
    }
}
/*procedimiento que se encargara de llenar el terreno con elementos con las coordenadas de un vector*/
void llenar_tablero_con_elementos(elemento_t elementos[MAX_ELEMENTOS],int tope, char terreno[MAX_FILAS][MAX_COLUMNAS]){

    for (int i = 0; i < tope; i++){
        terreno [elementos[i].coordenada.fila] [elementos[i].coordenada.col] = elementos[i].tipo;
    }
}

void imprimir_terreno(juego_t juego){

    char terreno[MAX_FILAS][MAX_COLUMNAS];

    for (int i = 0 ; i< MAX_FILAS ; i++){
        for (int j = 0 ; j < MAX_COLUMNAS ; j++){
            terreno[i][j] = TERRENO;
        }
    }

    llenar_tablero_con_paredes(juego.niveles[juego.nivel_actual-1].paredes ,juego.niveles[juego.nivel_actual-1].tope_paredes, terreno);    
    llenar_tablero_con_elementos(juego.niveles[juego.nivel_actual-1].obstaculos,juego.niveles[juego.nivel_actual-1].tope_obstaculos, terreno);
    llenar_tablero_con_elementos(juego.niveles[juego.nivel_actual-1].herramientas,juego.niveles[juego.nivel_actual-1].tope_herramientas, terreno);
    terreno[juego.personaje.posicion.fila][juego.personaje.posicion.col]=MAC;
    terreno[juego.sombra.posicion.fila][juego.sombra.posicion.col]=BLOO;


    system("clear");
    printf("                    ✨✨✨✨✨✨✨✨✨✨\n");
    printf("                    ✨LA SOMBRA DE MAC✨\n");
    printf("                    ✨✨✨✨✨✨✨✨✨✨\n");

    for (int i = 0 ; i< MAX_FILAS ; i++){        
        for (int j = 0 ; j < MAX_COLUMNAS ; j++){
            if(terreno[i][j] == TERRENO){
                printf(" %s ",CONST_TERRENO);
            }
            if(terreno[i][j] == MAC){
                printf(" %s",CONST_MAC);
            }if(terreno[i][j] == BLOO){
                printf(" %s",CONST_BLOO);
            }
            if(terreno[i][j] == VELA){
                printf(" %s ",CONST_VELA);
            }
            if(terreno[i][j] == POZO){
                printf(" %s ",CONST_POZO);
            }
            if(terreno[i][j] == INTERRUPTOR){
                printf(" %s",CONST_INTERRUPTOR);
            }
            if(terreno[i][j] == PAREDES){
                printf(" %s",CONST_PAREDES);
            }
            if(terreno[i][j] == PORTAL){
                printf(" %s",CONST_PORTAL);
            }
            if(terreno[i][j] == ESCALERA){
                printf(" %s ",CONST_ESCALERA);
            }
            if(terreno[i][j] == MONEDA){
                printf(" %s ",CONST_MONEDA);
            }
            if(terreno[i][j] == PUERTA){
                printf(" %s",CONST_PUERTA);
            }
            if(terreno[i][j] == LLAVE){
                printf(" %s",CONST_LLAVE);
            }
            
            
            //printf(" %c ",terreno[i][j]);  
        }
        printf("\n");
    }
}
/*procedimiento que se encargara de efectuar al pisar la vela la sombra y/o el personaje*/
void efectuar_vela(personaje_t* personaje , sombra_t* sombra ,elemento_t obstaculos[MAX_ELEMENTOS] , int* tope_obstaculos , int personaje_obstaculo_coincidido, bool* vela_encendida){
    
    for (int i = 0 ; i<(*tope_obstaculos) ; i++){
        if (obstaculos[i].tipo == VELA){
            if (esta_dentro_efecto_vela((*sombra).posicion , obstaculos[i].coordenada)){
                (*sombra).esta_viva = false;
                (*vela_encendida) = true;
            }
        }
    } 

    

    if (coordenadas_iguales((*personaje).posicion , (*sombra).posicion) && !(*sombra).esta_viva && !(*vela_encendida)){
        (*sombra).esta_viva = true;
        (*personaje).puntos -= 50;
    }
    else if (coordenadas_iguales((*personaje).posicion , (*sombra).posicion) && (*vela_encendida)){
        (*personaje).puntos -= 50;
    }
}
/*procedimiento que se encargara de efectuar al pisar el pozo la sombra y/o el personaje*/
void efectuar_pozo(personaje_t* personaje , sombra_t* sombra , nivel_t nivel){

    coordenada_t posicion_personaje = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
    coordenada_t posicion_sombra;
    posicion_sombra.fila = posicion_personaje.fila;
    posicion_sombra.col = MAX_COLUMNAS-posicion_personaje.col-1 ;

    while (!son_posiciones_vacias(nivel ,posicion_personaje,posicion_sombra)){
        posicion_personaje = coordenada_aleatoria(MAX_FILAS,MAX_COLUMNAS);
        posicion_sombra.fila = posicion_personaje.fila;
        posicion_sombra.col = MAX_COLUMNAS-posicion_personaje.col-1 ;
    }
    (*personaje).posicion = posicion_personaje;
    (*sombra).posicion.fila = posicion_sombra.fila;
    (*sombra).posicion.col = posicion_sombra.col ;
    (*personaje).vida--;    
}    
/*procedimiento que se encargara de efectuar al pisar el interruptor el personaje*/
void efectuar_interruptor(personaje_t* personaje){
    if (!(*personaje).interruptor_apretado){
        (*personaje).interruptor_apretado = true;
           
    }else{
        (*personaje).interruptor_apretado = false;
    }
}
/*procedimiento que se encargara de efectuar al pisar el portal la sombra y/o el personaje*/
void efectuar_portal(personaje_t* personaje , sombra_t* sombra){
    coordenada_t coordenada_aux;
    coordenada_aux = (*personaje).posicion;
    (*personaje).posicion = (*sombra).posicion;
    (*sombra).posicion = coordenada_aux;
}
/*procedimiento que se encargara de efectuar al pisar la moneda la sombra y/o el personaje*/
void efectuar_moneda(personaje_t* personaje , elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas , int posicion_moneda_coincidido){

    (*personaje).puntos += rand()%RANGO_NUM_ALEATORIO+VALOR_MINIMO_NUM_ALEATORIO;
    eliminar_elemento(herramientas ,&(*tope_herramientas), posicion_moneda_coincidido);
}
/*procedimiento que se encargara de efectuar al pisar la llave el personaje*/
void efectuar_llave(personaje_t* personaje , elemento_t herramientas[MAX_ELEMENTOS] , int* tope_herramientas , int personaje_herramienta_coincidido){
    (*personaje).tiene_llave = true;
    eliminar_elemento(herramientas ,&(*tope_herramientas), personaje_herramienta_coincidido);
}

void realizar_jugada(juego_t* juego){
   
    char movimiento;

    pedir_movimiento(&movimiento , &(*juego).personaje);

    mover_personaje(&(*juego).personaje, &(*juego).sombra, movimiento ,(*juego).niveles[(*juego).nivel_actual-1].paredes,(*juego).niveles[(*juego).nivel_actual-1].tope_paredes ,(*juego).niveles[(*juego).nivel_actual-1].herramientas ,  &(*juego).niveles[(*juego).nivel_actual-1].tope_herramientas);
    
    int personaje_obstaculo_coincidido = buscar_elemento((*juego).niveles[juego->nivel_actual-1].obstaculos , (*juego).niveles[juego->nivel_actual-1].tope_obstaculos , (*juego).personaje.posicion);

    int sombra_obstaculo_coincidido = buscar_elemento((*juego).niveles[juego->nivel_actual-1].obstaculos , (*juego).niveles[juego->nivel_actual-1].tope_obstaculos , (*juego).sombra.posicion);

    int personaje_herramienta_coincidido = buscar_elemento((*juego).niveles[juego->nivel_actual-1].herramientas , (*juego).niveles[juego->nivel_actual-1].tope_herramientas , (*juego).personaje.posicion);

    int sombra_herramienta_coincidido = buscar_elemento((*juego).niveles[juego->nivel_actual-1].herramientas , (*juego).niveles[juego->nivel_actual-1].tope_herramientas , (*juego).sombra.posicion);

    bool vela_encendida  = false;
    efectuar_vela(&(*juego).personaje ,&(*juego).sombra ,(*juego).niveles[juego->nivel_actual-1].obstaculos , &(*juego).niveles[juego->nivel_actual-1].tope_obstaculos , personaje_obstaculo_coincidido,&vela_encendida);

    if ((*juego).niveles[juego->nivel_actual-1].obstaculos[personaje_obstaculo_coincidido].tipo == VELA){
        eliminar_elemento((*juego).niveles[juego->nivel_actual-1].obstaculos ,&(*juego).niveles[juego->nivel_actual-1].tope_obstaculos, personaje_obstaculo_coincidido);
        vela_encendida = false;
    }
    else if ((*juego).niveles[juego->nivel_actual-1].obstaculos[personaje_obstaculo_coincidido].tipo == POZO){
        efectuar_pozo(&(*juego).personaje , &(*juego).sombra , (*juego).niveles[juego->nivel_actual-1]);
            
    }
    
    else if ((*juego).niveles[juego->nivel_actual-1].obstaculos[personaje_obstaculo_coincidido].tipo == INTERRUPTOR){
        efectuar_interruptor(&(*juego).personaje);
    }


    else if ((*juego).niveles[juego->nivel_actual-1].obstaculos[sombra_obstaculo_coincidido].tipo == PORTAL){
        efectuar_portal(&(*juego).personaje , &(*juego).sombra);
    }

 
    else if ((*juego).niveles[juego->nivel_actual-1].herramientas[personaje_herramienta_coincidido].tipo == MONEDA){

        efectuar_moneda(&(*juego).personaje , (*juego).niveles[juego->nivel_actual-1].herramientas , &(*juego).niveles[juego->nivel_actual-1].tope_herramientas , personaje_herramienta_coincidido);

    }
    else if ((*juego).niveles[juego->nivel_actual-1].herramientas[sombra_herramienta_coincidido].tipo == MONEDA){
        efectuar_moneda(&(*juego).personaje , (*juego).niveles[juego->nivel_actual-1].herramientas , &(*juego).niveles[juego->nivel_actual-1].tope_herramientas ,sombra_herramienta_coincidido);

    }
    else if ((*juego).niveles[juego->nivel_actual-1].herramientas[personaje_herramienta_coincidido].tipo == LLAVE){

        efectuar_llave(&(*juego).personaje , (*juego).niveles[juego->nivel_actual-1].herramientas , &(*juego).niveles[juego->nivel_actual-1].tope_herramientas , personaje_herramienta_coincidido);
        
    }
    
}