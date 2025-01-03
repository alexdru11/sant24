#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gamelib.h"

int main() {
    // Inizializza il generatore di numeri casuali
    srand(time(NULL));

    int scelta;

    do {
        printf("\nMenu Principale:\n");
        printf("1 - Imposta gioco\n");
        printf("2 - Gioca\n");
        printf("3 - Termina gioco\n");
        printf("4 - Crediti\n");
        printf(">> ");

        if (scanf("%d", &scelta) != 1) {
            printf("Errore: inserisci un numero valido.\n");
            while (getchar() != '\n'); // Pulisce il buffer di input
            continue;
        }

        switch (scelta) {
            case 1:
                imposta_gioco();
                break;
            case 2:
                gioca();
                break;
            case 3:
                termina_gioco();
                break;
            case 4:
                crediti();
                break;
            default:
                printf("Comando non valido.\n");
        }
    } while (scelta != 3);

    return 0;
}
