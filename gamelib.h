#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Dimensioni massime per i nomi
#define MAX_NAME_LENGTH 50
#define MAX_ROOMS 15
#define MAX_PLAYERS 3

// Enumerazione per la classe del giocatore
typedef enum {
    principe,
    doppleganger
} tipo_giocatore;

// Enumerazione per il tipo di stanza
typedef enum {
    corridoio,
    scala,
    sala_banchetto,
    magazzino,
    posto_guardia,
    prigione,
    armeria,
    moschea,
    torre,
    bagni
} Tipo_stanza;

// Enumerazione per il tipo di trabocchetto
typedef enum {
    nessuno,
    tegola,
    lame,
    caduta,
    burrone
} Tipo_trabocchetto;

// Enumerazione per il tipo di tesoro
typedef enum {
    nessun_tesoro,
    verde_veleno,
    blu_guarigione,
    rosso_aumenta_vita,
    spada_tagliente,
    scudo
} Tipo_tesoro;

// Struttura per il giocatore
typedef struct {
    char nome_giocatore[MAX_NAME_LENGTH];
    tipo_giocatore classe;
    struct Stanza *posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
} Giocatore;

// Struttura per la stanza
typedef struct Stanza {
    Tipo_stanza tipo;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
    struct Stanza *stanza_destra;
    struct Stanza *stanza_sinistra;
    struct Stanza *stanza_sopra;
    struct Stanza *stanza_sotto;
} Stanza;

// Funzioni di gamelib.c
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

#endif
