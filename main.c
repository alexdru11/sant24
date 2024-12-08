#include <stdio.h>
#include "gamelib.h"

int main()
{
    int scelta;

    do
    {
        printf("\nMenu Principale:\n");
        printf("1. Imposta gioco\n");
        printf("2. Gioca\n");
        printf("3. Termina gioco\n");
        printf("4. Visualizza crediti\n");
        printf("Scegli un'opzione: ");
        scanf("%d", &scelta);

        switch (scelta)
        {
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
