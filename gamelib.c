#include "gamelib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Global variables
static Stanza *pFirst = NULL;                   // Pointer to the first room in the map
static int map_closed = 1;                      // 0 if the map is closed, 1 otherwise
static int num_giocatori;                       // Number of players
static Giocatore *giocatori[MAX_PLAYERS];       // Array of players
static char *vincitori[3] = {NULL, NULL, NULL}; // Array of the last 3 winners
static char *creatore_gioco = NULL;             // Name of the game creator
static int gioco_impostato = 0;                 // 0 if the game has not been set up, 1 otherwise

// Static function prototypes (private functions)
static void ins_stanza();
static void canc_stanza();
static void stampa_stanze();
static void genera_random();
static void chiudi_mappa();
static int lancia_dado();
static int calcola_danno(int dadi_attacco);
static int calcola_difesa(int dadi_difesa);
static void combatti(Giocatore *giocatore, int tipo_nemico);
static void avanza(Giocatore *giocatore);
static void stampa_giocatore(Giocatore *giocatore);
static void stampa_zona(Giocatore *giocatore);
static void prendi_tesoro(Giocatore *giocatore);
static void cerca_porta_segreta(Giocatore *giocatore);
static void passa(Giocatore *giocatore);
static void applica_effetto_trabocchetto(Giocatore *giocatore, Tipo_trabocchetto trabocchetto);
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro);
static void libera_mappa();
static void aggiorna_vincitori(char *vincitore);
static void salva_vincitori();
static void carica_vincitori();

// Function to generate a random number between min and max (inclusive)
static int random_number(int min, int max)
{
    return min + rand() % (max - min + 1);
}

// Function to insert a new room
static void ins_stanza()
{
    if (map_closed == 0)
    {
        printf("La mappa e' gia' stata chiusa. Impossibile aggiungere nuove stanze.\n");
        return;
    }
    Stanza *new_stanza = (Stanza *)malloc(sizeof(Stanza));
    if (new_stanza == NULL)
    {
        printf("Errore: impossibile allocare memoria per una nuova stanza.\n");
        return;
    }

    // Initialize the new room with default values
    new_stanza->tipo = random_number(0, 9);
    new_stanza->trabocchetto = random_number(0, 4);
    new_stanza->tesoro = random_number(0, 5);
    new_stanza->stanza_destra = NULL;
    new_stanza->stanza_sinistra = NULL;
    new_stanza->stanza_sopra = NULL;
    new_stanza->stanza_sotto = NULL;

    // If it's the first room
    if (pFirst == NULL)
    {
        pFirst = new_stanza;
    }
    else
    {
        Stanza *current = pFirst;
        while (current->stanza_destra != NULL)
        {
            current = current->stanza_destra;
        }

        int direction;
        printf("In che direzione vuoi inserire la nuova stanza?\n");
        printf("1. Sopra\n2. Sotto\n3. Sinistra\n4. Destra\n");
        scanf("%d", &direction);

        switch (direction)
        {
        case 1:
            if (current->stanza_sopra != NULL)
            {
                printf("Errore: c'e' gia' una stanza sopra.\n");
                free(new_stanza);
                return;
            }
            current->stanza_sopra = new_stanza;
            break;
        case 2:
            if (current->stanza_sotto != NULL)
            {
                printf("Errore: c'e' gia' una stanza sotto.\n");
                free(new_stanza);
                return;
            }
            current->stanza_sotto = new_stanza;
            break;
        case 3:
            if (current->stanza_sinistra != NULL)
            {
                printf("Errore: c'e' gia' una stanza a sinistra.\n");
                free(new_stanza);
                return;
            }
            current->stanza_sinistra = new_stanza;
            break;
        case 4:
            current->stanza_destra = new_stanza;
            break;
        default:
            printf("Errore: direzione non valida.\n");
            free(new_stanza);
            return;
        }
    }

    printf("Stanza inserita con successo.\n");
}

// Function to delete the last room
static void canc_stanza()
{
    if (map_closed == 0)
    {
        printf("La mappa e' gia' stata chiusa. Impossibile cancellare stanze.\n");
        return;
    }
    if (pFirst == NULL)
    {
        printf("Errore: nessuna stanza da cancellare.\n");
        return;
    }

    if (pFirst->stanza_destra == NULL && pFirst->stanza_sinistra == NULL &&
        pFirst->stanza_sopra == NULL && pFirst->stanza_sotto == NULL)
    {
        free(pFirst);
        pFirst = NULL;
        printf("Ultima stanza cancellata.\n");
        return;
    }

    Stanza *current = pFirst;
    Stanza *prev = NULL;

    while (current->stanza_destra != NULL || current->stanza_sinistra != NULL ||
           current->stanza_sopra != NULL || current->stanza_sotto != NULL)
    {
        prev = current;
        if (current->stanza_destra != NULL)
        {
            current = current->stanza_destra;
        }
        else if (current->stanza_sinistra != NULL)
        {
            current = current->stanza_sinistra;
        }
        else if (current->stanza_sopra != NULL)
        {
            current = current->stanza_sopra;
        }
        else
        {
            current = current->stanza_sotto;
        }
    }

    if (prev != NULL)
    {
        if (prev->stanza_destra == current)
        {
            prev->stanza_destra = NULL;
        }
        else if (prev->stanza_sinistra == current)
        {
            prev->stanza_sinistra = NULL;
        }
        else if (prev->stanza_sopra == current)
        {
            prev->stanza_sopra = NULL;
        }
        else
        {
            prev->stanza_sotto = NULL;
        }
    }

    free(current);
    printf("Ultima stanza cancellata.\n");
}

// Function to print all rooms
static void stampa_stanze()
{
    if (pFirst == NULL)
    {
        printf("Nessuna stanza presente nella mappa.\n");
        return;
    }

    Stanza *current = pFirst;
    int count = 1;

    while (current != NULL)
    {
        printf("Stanza %d:\n", count);
        printf("  Tipo: %u\n", current->tipo);
        printf("  Trabocchetto: %u\n", current->trabocchetto);
        printf("  Tesoro: %u\n", current->tesoro);
        printf("  Stanza a destra: %s\n", current->stanza_destra ? "presente" : "assente");
        printf("  Stanza a sinistra: %s\n", current->stanza_sinistra ? "presente" : "assente");
        printf("  Stanza sopra: %s\n", current->stanza_sopra ? "presente" : "assente");
        printf("  Stanza sotto: %s\n", current->stanza_sotto ? "presente" : "assente");
        printf("\n");

        // Move to the next room (you can choose any direction)
        if (current->stanza_destra != NULL)
        {
            current = current->stanza_destra;
        }
        else if (current->stanza_sinistra != NULL)
        {
            current = current->stanza_sinistra;
        }
        else if (current->stanza_sopra != NULL)
        {
            current = current->stanza_sopra;
        }
        else
        {
            current = current->stanza_sotto;
        }
        count++;
    }
}

// Function to generate a random map
static void genera_random()
{
    libera_mappa();

    for (int i = 0; i < MAP_SIZE; i++)
    {
        Stanza *new_stanza = (Stanza *)malloc(sizeof(Stanza));
        if (new_stanza == NULL)
        {
            printf("Errore nell'allocazione della memoria per la stanza.\n");
            return;
        }

        new_stanza->tipo = random_number(0, 9);
        if (random_number(1, 100) <= 65)
        {
            new_stanza->trabocchetto = NESSUNO;
        }
        else
        {
            int random_trap = random_number(1, 4);
            switch (random_trap)
            {
            case 1:
                new_stanza->trabocchetto = TEGOLA;
                break;
            case 2:
                new_stanza->trabocchetto = LAME;
                break;
            case 3:
                new_stanza->trabocchetto = CADUTA;
                break;
            case 4:
                new_stanza->trabocchetto = BURRONE;
                break;
            }
        }
        if (random_number(1, 100) <= 20)
        {
            new_stanza->tesoro = NESSUN_TESORO;
        }
        else
        {
            int random_treasure = random_number(1, 5);
            switch (random_treasure)
            {
            case 1:
                new_stanza->tesoro = VERDE_VELENO;
                break;
            case 2:
                new_stanza->tesoro = BLU_GUARIGIONE;
                break;
            case 3:
                new_stanza->tesoro = ROSSO_AUMENTA_VITA;
                break;
            case 4:
                new_stanza->tesoro = SPADA_TAGLIENTE;
                break;
            case 5:
                new_stanza->tesoro = SCUDO;
                break;
            }
        }
        new_stanza->stanza_destra = NULL;
        new_stanza->stanza_sinistra = NULL;
        new_stanza->stanza_sopra = NULL;
        new_stanza->stanza_sotto = NULL;

        if (pFirst == NULL)
        {
            pFirst = new_stanza;
        }
        else
        {
            Stanza *current = pFirst;
            while (current->stanza_destra != NULL)
            {
                current = current->stanza_destra;
            }
            current->stanza_destra = new_stanza;
        }
    }
    printf("Mappa casuale generata con %d stanze.\n", MAP_SIZE);
}

// Function to close the map
static void chiudi_mappa()
{
    if (map_closed == 0)
    {
        printf("La mappa e' gia' chiusa!\n");
        return;
    }

    // Check if there are at least 15 rooms
    int room_count = 0;
    Stanza *current = pFirst;
    while (current != NULL)
    {
        room_count++;
        current = current->stanza_destra;
    }

    if (room_count < MIN_ROOMS)
    {
        printf("La mappa deve contenere almeno %d stanze prima di essere chiusa.\n", MIN_ROOMS);
        return;
    }

    // Set the position of each player to the first room
    for (int i = 0; i < num_giocatori; i++)
    {
        if (giocatori[i] != NULL)
        {
            giocatori[i]->posizione = pFirst;
        }
    }

    map_closed = 0;
    printf("Mappa chiusa con successo.\n");
}

// Function to roll a die
static int lancia_dado()
{
    return random_number(1, 6);
}

// Function to calculate damage
static int calcola_danno(int dadi_attacco)
{
    int danno = 0;
    for (int i = 0; i < dadi_attacco; i++)
    {
        int tiro = lancia_dado();
        if (tiro >= 4)
        {
            danno++;
            if (tiro == 6)
            {
                danno++; // Critical hit
            }
        }
    }
    return danno;
}

// Function to calculate defense
static int calcola_difesa(int dadi_difesa)
{
    int difesa = 0;
    for (int i = 0; i < dadi_difesa; i++)
    {
        int tiro = lancia_dado();
        if (tiro >= 4)
        {
            difesa++;
            if (tiro == 6)
            {
                difesa++; // Critical defense
            }
        }
    }
    return difesa;
}

// Function to handle combat
static void combatti(Giocatore *giocatore, int tipo_nemico)
{
    int dadi_attacco_nemico, dadi_difesa_nemico, p_vita_nemico;
    switch (tipo_nemico)
    {
    case 1: // Skeleton
        dadi_attacco_nemico = 1;
        dadi_difesa_nemico = 1;
        p_vita_nemico = 1;
        printf("Uno scheletro attacca!\n");
        break;
    case 2: // Guard
        dadi_attacco_nemico = 2;
        dadi_difesa_nemico = 2;
        p_vita_nemico = 2;
        printf("Una guardia attacca!\n");
        break;
    case 3: // Jaffar
        dadi_attacco_nemico = 3;
        dadi_difesa_nemico = 2;
        p_vita_nemico = 3;
        printf("Jafar attacca!\n");
        break;
    default:
        return; // No enemy
    }

    while (giocatore->p_vita > 0 && p_vita_nemico > 0)
    {
        // Determine who attacks first
        int attacco_giocatore = lancia_dado();
        int attacco_nemico = lancia_dado();

        if (attacco_giocatore >= attacco_nemico)
        {
            // Player attacks first
            int danno = calcola_danno(giocatore->dadi_attacco);
            int difesa = calcola_difesa(dadi_difesa_nemico);
            int danno_effettivo = danno - difesa;
            if (danno_effettivo > 0)
            {
                p_vita_nemico -= danno_effettivo;
                printf("Il giocatore infligge %d danni.\n", danno_effettivo);
            }
            else
            {
                printf("Il nemico para l'attacco.\n");
            }

            // Enemy attacks
            if (p_vita_nemico > 0)
            {
                danno = calcola_danno(dadi_attacco_nemico);
                difesa = calcola_difesa(giocatore->dadi_difesa);
                danno_effettivo = danno - difesa;
                if (danno_effettivo > 0)
                {
                    giocatore->p_vita -= danno_effettivo;
                    printf("Il nemico infligge %d danni.\n", danno_effettivo);
                }
                else
                {
                    printf("Il giocatore para l'attacco.\n");
                }
            }
        }
        else
        {
            // Enemy attacks first
            int danno = calcola_danno(dadi_attacco_nemico);
            int difesa = calcola_difesa(giocatore->dadi_difesa);
            int danno_effettivo = danno - difesa;
            if (danno_effettivo > 0)
            {
                giocatore->p_vita -= danno_effettivo;
                printf("Il nemico infligge %d danni.\n", danno_effettivo);
            }
            else
            {
                printf("Il giocatore para l'attacco.\n");
            }

            // Player attacks
            if (giocatore->p_vita > 0)
            {
                danno = calcola_danno(giocatore->dadi_attacco);
                difesa = calcola_difesa(dadi_difesa_nemico);
                danno_effettivo = danno - difesa;
                if (danno_effettivo > 0)
                {
                    p_vita_nemico -= danno_effettivo;
                    printf("Il giocatore infligge %d danni.\n", danno_effettivo);
                }
                else
                {
                    printf("Il nemico para l'attacco.\n");
                }
            }
        }
    }

    if (giocatore->p_vita <= 0)
    {
        printf("Il giocatore e' stato sconfitto.\n");
    }
    else
    {
        printf("Il nemico e' stato sconfitto.\n");
        giocatore->p_vita++; // Gain 1 health point for winning
    }
}

// Function to move the player
static void avanza(Giocatore *giocatore)
{
    if (giocatore == NULL || giocatore->posizione == NULL)
    {
        printf("Errore: giocatore o posizione non validi.\n");
        return;
    }

    Stanza *current_room = giocatore->posizione;
    Stanza *next_room = NULL;

    // Determine the available direction to move
    if (current_room->stanza_sopra != NULL)
    {
        next_room = current_room->stanza_sopra;
        printf("Il giocatore si muove verso l'alto.\n");
    }
    else if (current_room->stanza_sotto != NULL)
    {
        next_room = current_room->stanza_sotto;
        printf("Il giocatore si muove verso il basso.\n");
    }
    else if (current_room->stanza_sinistra != NULL)
    {
        next_room = current_room->stanza_sinistra;
        printf("Il giocatore si muove verso sinistra.\n");
    }
    else if (current_room->stanza_destra != NULL)
    {
        next_room = current_room->stanza_destra;
        printf("Il giocatore si muove verso destra.\n");
    }
    else
    {
        printf("Non c'e' una direzione valida in cui muoversi.\n");
        return;
    }

    if (next_room != NULL)
    {
        giocatore->posizione = next_room;

        // Check for traps
        applica_effetto_trabocchetto(giocatore, next_room->trabocchetto);

        // Check for enemies
        if (next_room->tipo == POSTO_GUARDIA)
        {
            if (random_number(1, 100) <= 25)
            {
                int tipo_nemico = (random_number(1, 100) <= 60) ? 1 : 2; // 1: Skeleton, 2: Guard
                combatti(giocatore, tipo_nemico);
            }
        }
        else if (next_room->tipo == TORRE)
        {
            combatti(giocatore, 3); // 3: Jaffar
        }
    }
    else
    {
        printf("Non c'e' una stanza in cui muoversi.\n");
    }
}

// Function to print player information
static void stampa_giocatore(Giocatore *giocatore)
{
    if (giocatore == NULL)
    {
        printf("Errore: giocatore non valido.\n");
        return;
    }

    printf("Nome: %s\n", giocatore->nome_giocatore);
    printf("Classe: %s\n", (giocatore->classe_giocatore == PRINCIPE) ? "Principe" : "Doppleganger");
    printf("Punti vita: %d/%d\n", giocatore->p_vita, giocatore->p_vita_max);
    printf("Dadi attacco: %d\n", giocatore->dadi_attacco);
    printf("Dadi difesa: %d\n", giocatore->dadi_difesa);
}

// Function to print room information
static void stampa_zona(Giocatore *giocatore)
{
    if (giocatore == NULL || giocatore->posizione == NULL)
    {
        printf("Errore: giocatore o posizione non validi.\n");
        return;
    }

    Stanza *current_room = giocatore->posizione;
    printf("Tipo di stanza: %u\n", current_room->tipo);
    printf("Trabocchetto: %u\n", current_room->trabocchetto);
    printf("Tesoro presente: %s\n", (current_room->tesoro != NESSUN_TESORO) ? "si" : "no");
}

// Function to pick up treasure
static void prendi_tesoro(Giocatore *giocatore)
{
    if (giocatore == NULL || giocatore->posizione == NULL)
    {
        printf("Errore: giocatore o posizione non validi.\n");
        return;
    }

    Stanza *current_room = giocatore->posizione;
    if (current_room->tesoro != NESSUN_TESORO)
    {
        applica_effetto_tesoro(giocatore, current_room->tesoro);
        current_room->tesoro = NESSUN_TESORO; // Remove treasure after picking up
    }
    else
    {
        printf("Non c'e' nessun tesoro qui.\n");
    }
}

// Function to search for a secret door
static void cerca_porta_segreta(Giocatore *giocatore)
{
    if (giocatore == NULL || giocatore->posizione == NULL)
    {
        printf("Errore: giocatore o posizione non validi.\n");
        return;
    }

    Stanza *current_room = giocatore->posizione;

    // Check each direction for a possible secret door
    for (int dir = 0; dir < 4; dir++)
    {
        Stanza *existing_room = NULL;
        switch (dir)
        {
        case 0:
            existing_room = current_room->stanza_sopra;
            break;
        case 1:
            existing_room = current_room->stanza_sotto;
            break;
        case 2:
            existing_room = current_room->stanza_sinistra;
            break;
        case 3:
            existing_room = current_room->stanza_destra;
            break;
        }

        // If there's no room in the current direction, check for a secret door
        if (existing_room == NULL && random_number(1, 100) <= 33)
        {
            // Create a new secret room
            Stanza *secret_room = (Stanza *)malloc(sizeof(Stanza));
            if (secret_room == NULL)
            {
                printf("Errore nell'allocazione della memoria per la stanza segreta.\n");
                return;
            }

            // Initialize the secret room with random values
            secret_room->tipo = random_number(0, 9);
            secret_room->trabocchetto = random_number(0, 4);
            secret_room->tesoro = random_number(0, 5);
            secret_room->stanza_destra = NULL;
            secret_room->stanza_sinistra = NULL;
            secret_room->stanza_sopra = NULL;
            secret_room->stanza_sotto = NULL;

            // Link the secret room to the current room
            switch (dir)
            {
            case 0:
                current_room->stanza_sopra = secret_room;
                break;
            case 1:
                current_room->stanza_sotto = secret_room;
                break;
            case 2:
                current_room->stanza_sinistra = secret_room;
                break;
            case 3:
                current_room->stanza_destra = secret_room;
                break;
            }

            printf("Hai trovato una porta segreta!\n");
            giocatore->posizione = secret_room; // Move the player to the secret room

            // Automatically move the player back to the original room after finding the secret door
            switch (dir)
            {
            case 0:
                giocatore->posizione = current_room;
                break;
            case 1:
                giocatore->posizione = current_room;
                break;
            case 2:
                giocatore->posizione = current_room;
                break;
            case 3:
                giocatore->posizione = current_room;
                break;
            }
            printf("Ritorni nella stanza precedente.\n");
            return; // Exit after finding one secret door
        }
    }

    printf("Non hai trovato nessuna porta segreta.\n");
}

// Function to pass the turn
static void passa(Giocatore *giocatore)
{
    printf("Il giocatore %s passa il turno.\n", giocatore->nome_giocatore);
}

// Function to apply trap effects
static void applica_effetto_trabocchetto(Giocatore *giocatore, Tipo_trabocchetto trabocchetto)
{
    if (giocatore->classe_giocatore == PRINCIPE && giocatore->p_vita == giocatore->p_vita_max)
    {
        printf("Il principe ignora il primo trabocchetto.\n");
        return;
    }
    switch (trabocchetto)
    {
    case TEGOLA:
        giocatore->p_vita--;
        printf("Sei stato colpito da una tegola e perdi 1 punto vita.\n");
        break;
    case LAME:
        giocatore->p_vita -= 2;
        printf("Sei stato colpito dalle lame e perdi 2 punti vita.\n");
        break;
    case CADUTA:
        if (random_number(1, 2) == 1)
        {
            giocatore->p_vita--;
            printf("Sei caduto e perdi 1 punto vita.\n");
        }
        else
        {
            giocatore->p_vita -= 2;
            printf("Sei caduto e perdi 2 punti vita.\n");
        }
        break;
    case BURRONE:
        giocatore->p_vita = 0;
        printf("Sei caduto in un burrone e sei morto.\n");
        break;
    case NESSUNO:
    default:
        break;
    }
}

// Function to apply treasure effects
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro)
{
    switch (tesoro)
    {
    case VERDE_VELENO:
        giocatore->p_vita--;
        printf("Hai raccolto del veleno verde e perdi 1 punto vita.\n");
        break;
    case BLU_GUARIGIONE:
        giocatore->p_vita = (giocatore->p_vita < giocatore->p_vita_max) ? giocatore->p_vita + 1 : giocatore->p_vita_max;
        printf("Hai raccolto una pozione di guarigione blu e recuperi 1 punto vita.\n");
        break;
    case ROSSO_AUMENTA_VITA:
        giocatore->p_vita_max++;
        giocatore->p_vita = giocatore->p_vita_max;
        printf("Hai raccolto una pozione rossa che aumenta la tua vita massima di 1.\n");
        break;
    case SPADA_TAGLIENTE:
        giocatore->dadi_attacco++;
        printf("Hai raccolto una spada tagliente e ottieni 1 dado attacco aggiuntivo.\n");
        break;
    case SCUDO:
        giocatore->dadi_difesa++;
        printf("Hai raccolto uno scudo e ottieni 1 dado difesa aggiuntivo.\n");
        break;
    case NESSUN_TESORO:
    default:
        break;
    }
}

// Function to free the memory allocated for the map
static void libera_mappa()
{
    if (pFirst == NULL)
        return;

    Stanza *current = pFirst;
    while (current != NULL)
    {
        Stanza *next = current->stanza_destra; // Assuming a linear structure for simplicity

        // Free the current room
        free(current);

        // Move to the next room
        current = next;
    }

    pFirst = NULL;  // Reset the pointer to the first room
    map_closed = 1; // Reset the map closed flag
}

// Function to update the winners array
static void aggiorna_vincitori(char *vincitore)
{
    // Shift the existing winners to the right
    if (vincitori[2] != NULL)
    {
        free(vincitori[2]); // Free the memory of the last winner
    }
    for (int i = 2; i > 0; i--)
    {
        vincitori[i] = vincitori[i - 1];
    }

    // Add the new winner at the beginning
    vincitori[0] = strdup(vincitore); // Allocate memory and copy the winner's name
}

// Function to save the winners to a file
static void salva_vincitori()
{
    FILE *file = fopen("vincitori.txt", "w");
    if (file == NULL)
    {
        printf("Errore nell'apertura del file dei vincitori.\n");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        if (vincitori[i] != NULL)
        {
            fprintf(file, "%s\n", vincitori[i]);
        }
    }

    fclose(file);
}

// Function to load the winners from a file
static void carica_vincitori()
{
    FILE *file = fopen("vincitori.txt", "r");
    if (file == NULL)
    {
        printf("File dei vincitori non trovato.\n");
        return;
    }

    for (int i = 0; i < 3; i++)
    {
        if (vincitori[i] != NULL)
        {
            free(vincitori[i]); // Free any existing memory
            vincitori[i] = NULL;
        }
        char buffer[MAX_NAME_LENGTH];
        if (fgets(buffer, sizeof(buffer), file) != NULL)
        {
            buffer[strcspn(buffer, "\n")] = 0; // Remove trailing newline
            vincitori[i] = strdup(buffer);     // Allocate memory and copy the winner's name
        }
    }

    fclose(file);
}

// Function to set up the game
void imposta_gioco()
{
    srand(time(NULL));

    if (gioco_impostato == 1)
    {
        // Free the memory allocated for the players
        for (int i = 0; i < MAX_PLAYERS; i++)
        {
            if (giocatori[i] != NULL)
            {
                free(giocatori[i]);
                giocatori[i] = NULL;
            }
        }

        // Reset the map if it was previously closed
        libera_mappa();
    }

    // 0. Game Creator
    if (creatore_gioco == NULL)
    {
        creatore_gioco = (char *)malloc(MAX_NAME_LENGTH * sizeof(char));
        if (creatore_gioco == NULL)
        {
            printf("Errore nell'allocazione della memoria per il nome del creatore.\n");
            exit(1);
        }
        printf("Inserisci il nome del creatore del gioco: ");
        scanf(" %s", creatore_gioco);
    }

    // 1. Player Setup
    do
    {
        printf("Inserisci il numero di giocatori (1-%d): ", MAX_PLAYERS);
        scanf("%d", &num_giocatori);
        if (num_giocatori < 1 || num_giocatori > MAX_PLAYERS)
        {
            printf("Numero di giocatori non valido.\n");
        }
    } while (num_giocatori < 1 || num_giocatori > MAX_PLAYERS);

    int principe_presente = 0;
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
        giocatori[i] = NULL;
    }

    for (int i = 0; i < num_giocatori; i++)
    {
        giocatori[i] = (Giocatore *)malloc(sizeof(Giocatore));
        if (giocatori[i] == NULL)
        {
            printf("Errore nell'allocazione della memoria per il giocatore.\n");
            exit(1);
        }

        printf("Inserisci il nome del giocatore %d: ", i + 1);
        scanf(" %s", giocatori[i]->nome_giocatore);

        if (!principe_presente)
        {
            printf("Scegli la classe del giocatore %d (0: PRINCIPE, 1: DOPPLEGANGER): ", i + 1);
            int scelta;
            scanf("%d", &scelta);
            if (scelta == 0)
            {
                giocatori[i]->classe_giocatore = PRINCIPE;
                principe_presente = 1;
            }
            else
            {
                giocatori[i]->classe_giocatore = DOPPLEGANGER;
            }
        }
        else
        {
            giocatori[i]->classe_giocatore = DOPPLEGANGER;
        }

        giocatori[i]->p_vita_max = INITIAL_HEALTH;
        giocatori[i]->p_vita = INITIAL_HEALTH;
        giocatori[i]->dadi_attacco = INITIAL_ATTACK_DICE;
        giocatori[i]->dadi_difesa = INITIAL_DEFENSE_DICE;
    }
