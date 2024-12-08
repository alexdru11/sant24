#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Numero massimo di giocatori
#define MAX_PLAYERS 3

// Lunghezza massima del nome del giocatore
#define MAX_NAME_LENGTH 50

// Numero di stanze generate casualmente
#define NUM_RANDOM_ROOMS 15

// Enumerazione per il tipo di giocatore
typedef enum {
    PRINCIPE,
    DOPPLEGANGER
} tipo_giocatore;

// Enumerazione per il tipo di stanza
typedef enum {
    CORRIDOIO,
    SCALA,
    SALA_BANCHETTO,
    MAGAZZINO,
    POSTO_GUARDIA,
    PRIGIONE,
    ARMERIA,
    MOSCHEA,
    TORRE,
    BAGNI
} Tipo_stanza;

// Enumerazione per il tipo di trabocchetto
typedef enum {
    NESSUNO,
    TEGOLA,
    LAME,
    CADUTA,
    BURRONE
} Tipo_trabocchetto;

// Enumerazione per il tipo di tesoro
typedef enum {
    NESSUN_TESORO,
    VERDE_VELENO,
    BLU_GUARIGIONE,
    ROSSO_AUMENTA_VITA,
    SPADA_TAGLIENTE,
    SCUDO
} Tipo_tesoro;

// Struttura per rappresentare una stanza
typedef struct Stanza {
    Tipo_stanza tipo;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
    struct Stanza* stanza_destra;
    struct Stanza* stanza_sinistra;
    struct Stanza* stanza_sopra;
    struct Stanza* stanza_sotto;
} Stanza;

// Struttura per rappresentare un giocatore
typedef struct Giocatore {
    char nome_giocatore[MAX_NAME_LENGTH];
    tipo_giocatore tipo;
    Stanza* posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    int turni_saltati;
} Giocatore;

// Funzioni principali (dichiarazioni)
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

// variabili globali
int mappa_creata;
Stanza *pFirst;
Giocatore* giocatori[MAX_PLAYERS];
char nomi_vincitori[3][MAX_NAME_LENGTH];
int indice_vincitori;
#endif
