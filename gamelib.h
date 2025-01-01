#ifndef GAMELIB_H
#define GAMELIB_H

#define MAX_PLAYERS 3
#define MAX_ROOMS 15
#define MAX_NAME_LENGTH 20

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
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
    struct Stanza *posizione;
    int fuggito;
} Giocatore;

// Struttura per la stanza
typedef struct Stanza {
    Tipo_stanza tipo;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
    struct Stanza *stanza_successiva;
    int trabocchetto_attivato;
    int tentativi_porta[4];
} Stanza;

// Funzioni
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

#endif
