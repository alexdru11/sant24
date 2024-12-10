#ifndef GAMELIB_H
#define GAMELIB_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_NAME_LENGTH 50
#define MAX_ROOMS 15
#define MAX_PLAYERS 3

// Enumerazione per la classe del giocatore
typedef enum {
    PRINCIPE,
    DOPPLEGANGER
} Tipo_giocatore;

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

// Struttura per il giocatore
typedef struct {
    char nome_giocatore[MAX_NAME_LENGTH];
    Tipo_giocatore classe;
    struct Stanza *posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    int fuggito; // Contatore per le fughe, 0 = non è fuggito, 1 = 1 volta, 2 = 2 volte (per i doppleganger)
} Giocatore;

// Struttura per la stanza
typedef struct Stanza {
    Tipo_stanza tipo;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
    int trabocchetto_attivato; // Flag per indicare se il trabocchetto è già stato attivato da un giocatore
    int tentativi_porta[4];  // Contatore tentativi per ogni direzione (0: Sopra, 1: Sotto, 2: Sinistra, 3: Destra)
    struct Stanza *stanza_sopra;
    struct Stanza *stanza_sotto;
    struct Stanza *stanza_sinistra;
    struct Stanza *stanza_destra;
} Stanza;

// Funzioni di gamelib.c
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

#endif
