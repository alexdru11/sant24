#ifndef GAMELIB_H
#define GAMELIB_H

#define MAX_PLAYERS 3
#define MAX_NAME_LENGTH 50
#define MAP_SIZE 15

#define MAX_ROOMS 100      // Maximum number of rooms in the map
#define MIN_ROOMS 15       // Minimum number of rooms to close the map
#define MAX_NAME_LENGTH 50 // Maximum length of player names
#define INITIAL_HEALTH 10
#define INITIAL_ATTACK_DICE 2
#define INITIAL_DEFENSE_DICE 2

// Enumerations
typedef enum
{
    PRINCIPE,
    DOPPLEGANGER
} tipo_giocatore;

typedef enum
{
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

typedef enum
{
    NESSUNO,
    TEGOLA,
    LAME,
    CADUTA,
    BURRONE
} Tipo_trabocchetto;

typedef enum
{
    NESSUN_TESORO,
    VERDE_VELENO,
    BLU_GUARIGIONE,
    ROSSO_AUMENTA_VITA,
    SPADA_TAGLIENTE,
    SCUDO
} Tipo_tesoro;

// Structures
typedef struct Giocatore
{
    char nome_giocatore[MAX_NAME_LENGTH];
    tipo_giocatore classe_giocatore;
    struct Stanza *posizione;
    unsigned char p_vita_max;
    unsigned char p_vita;
    unsigned char dadi_attacco;
    unsigned char dadi_difesa;
} Giocatore;

typedef struct Stanza
{
    Tipo_stanza tipo;
    Tipo_trabocchetto trabocchetto;
    Tipo_tesoro tesoro;
    struct Stanza *stanza_destra;
    struct Stanza *stanza_sinistra;
    struct Stanza *stanza_sopra;
    struct Stanza *stanza_sotto;
} Stanza;

// Function prototypes (public functions)
void imposta_gioco();
void gioca();
void termina_gioco();
void crediti();

#endif
