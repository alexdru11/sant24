#include "gamelib.h"

// Funzione per la creazione di una nuova stanza
static Stanza* crea_stanza() {
    Stanza* nuova_stanza = (Stanza*)malloc(sizeof(Stanza));
    if (nuova_stanza == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria per la stanza!\n");
        exit(EXIT_FAILURE);
    }

    nuova_stanza->tipo = CORRIDOIO; // Valore di default, verrà modificato in seguito
    nuova_stanza->trabocchetto = NESSUNO;
    nuova_stanza->tesoro = NESSUN_TESORO;
    nuova_stanza->stanza_destra = NULL;
    nuova_stanza->stanza_sinistra = NULL;
    nuova_stanza->stanza_sopra = NULL;
    nuova_stanza->stanza_sotto = NULL;

    return nuova_stanza;
}

// Funzione per l'inserimento di una stanza in coda
static void ins_stanza() {
    Stanza* nuova_stanza = crea_stanza();
    int direzione;

    printf("Inserisci il tipo di stanza:\n");
    printf("0. Corridoio\n1. Scala\n2. Sala Banchetto\n3. Magazzino\n4. Posto Guardia\n");
    printf("5. Prigione\n6. Armeria\n7. Moschea\n8. Torre\n9. Bagni\n");
    do {
        printf("Scelta: ");
        scanf("%d", (int*)&nuova_stanza->tipo);
    } while(nuova_stanza->tipo < CORRIDOIO || nuova_stanza->tipo > BAGNI);

    printf("Inserisci il tipo di trabocchetto:\n");
    printf("0. Nessuno\n1. Tegola\n2. Lame\n3. Caduta\n4. Burrone\n");
    do {
        printf("Scelta: ");
        scanf("%d", (int*)&nuova_stanza->trabocchetto);
    } while(nuova_stanza->trabocchetto < NESSUNO || nuova_stanza->trabocchetto > BURRONE);

    printf("Inserisci il tipo di tesoro:\n");
    printf("0. Nessun Tesoro\n1. Verde Veleno\n2. Blu Guarigione\n3. Rosso Aumenta Vita\n");
    printf("4. Spada Tagliente\n5. Scudo\n");
    do {
        printf("Scelta: ");
        scanf("%d", (int*)&nuova_stanza->tesoro);
    } while(nuova_stanza->tesoro < NESSUN_TESORO || nuova_stanza->tesoro > SCUDO);

    if (pFirst == NULL) {
        pFirst = nuova_stanza;
    } else {
        Stanza* current = pFirst;
        while (current->stanza_destra != NULL) { // Trova l'ultima stanza
            current = current->stanza_destra;
        }
        printf("Inserisci la direzione in cui collegare la nuova stanza:\n");
        printf("0: Sopra\n1: Destra\n2: Sinistra\n3: Sotto\n");

        do{
            printf("Scelta: ");
            scanf("%d", &direzione);
        } while(direzione < 0 || direzione > 3);

        switch (direzione)
        {
            case 0:
                if(current->stanza_sopra != NULL){
                    printf("Sopra e' gia' collegata ad un'altra stanza. Non puoi collegarla.\n");
                    free(nuova_stanza);
                    return;
                }
                current->stanza_sopra = nuova_stanza;
                break;
            case 1:
                if(current->stanza_destra != NULL){
                    printf("Destra e' gia' collegata ad un'altra stanza. Non puoi collegarla.\n");
                    free(nuova_stanza);
                    return;
                }
                current->stanza_destra = nuova_stanza;
                break;
            case 2:
                if(current->stanza_sinistra != NULL){
                    printf("Sinistra e' gia' collegata ad un'altra stanza. Non puoi collegarla.\n");
                    free(nuova_stanza);
                    return;
                }
                current->stanza_sinistra = nuova_stanza;
                break;
            case 3:
                if(current->stanza_sotto != NULL){
                    printf("Sotto e' gia' collegata ad un'altra stanza. Non puoi collegarla.\n");
                    free(nuova_stanza);
                    return;
                }
                current->stanza_sotto = nuova_stanza;
                break;
        }
    }
    printf("Stanza inserita con successo!\n");
}

// Funzione per cancellare l'ultima stanza inserita
static void canc_stanza() {
    if (pFirst == NULL) {
        printf("Nessuna stanza da cancellare!\n");
        return;
    }

    if (pFirst->stanza_destra == NULL && pFirst->stanza_sinistra == NULL && pFirst->stanza_sopra == NULL && pFirst->stanza_sotto == NULL) {
        free(pFirst);
        pFirst = NULL;
        printf("Ultima stanza cancellata!\n");
        return;
    }

    Stanza* current = pFirst;
    Stanza* prev = NULL;

    while (current->stanza_destra != NULL) {
        prev = current;
        current = current->stanza_destra;
    }

    if(current->stanza_sopra != NULL){
        printf("La stanza che si vuole cancellare ha una stanza collegata sopra. Non e' possibile cancellarla.\n");
        return;
    }
    if(current->stanza_sinistra != NULL){
        printf("La stanza che si vuole cancellare ha una stanza collegata a sinistra. Non e' possibile cancellarla.\n");
        return;
    }
    if(current->stanza_sotto != NULL){
        printf("La stanza che si vuole cancellare ha una stanza collegata sotto. Non e' possibile cancellarla.\n");
        return;
    }

    if(prev != NULL){
        prev->stanza_destra = NULL;
    }

    free(current);
    printf("Ultima stanza cancellata!\n");
}

// Funzione per stampare le stanze create
static void stampa_stanze() {
    if (pFirst == NULL) {
        printf("Nessuna stanza creata!\n");
        return;
    }

    Stanza* current = pFirst;
    int i = 0;

    while (current != NULL) {
        printf("Stanza %d:\n", i + 1);
        printf("  Tipo: ");
        switch (current->tipo) {
            case CORRIDOIO: printf("Corridoio\n"); break;
            case SCALA: printf("Scala\n"); break;
            case SALA_BANCHETTO: printf("Sala Banchetto\n"); break;
            case MAGAZZINO: printf("Magazzino\n"); break;
            case POSTO_GUARDIA: printf("Posto Guardia\n"); break;
            case PRIGIONE: printf("Prigione\n"); break;
            case ARMERIA: printf("Armeria\n"); break;
            case MOSCHEA: printf("Moschea\n"); break;
            case TORRE: printf("Torre\n"); break;
            case BAGNI: printf("Bagni\n"); break;
        }

        printf("  Trabocchetto: ");
        switch (current->trabocchetto) {
            case NESSUNO: printf("Nessuno\n"); break;
            case TEGOLA: printf("Tegola\n"); break;
            case LAME: printf("Lame\n"); break;
            case CADUTA: printf("Caduta\n"); break;
            case BURRONE: printf("Burrone\n"); break;
        }

        printf("  Tesoro: ");
        switch (current->tesoro) {
            case NESSUN_TESORO: printf("Nessun Tesoro\n"); break;
            case VERDE_VELENO: printf("Verde Veleno\n"); break;
            case BLU_GUARIGIONE: printf("Blu Guarigione\n"); break;
            case ROSSO_AUMENTA_VITA: printf("Rosso Aumenta Vita\n"); break;
            case SPADA_TAGLIENTE: printf("Spada Tagliente\n"); break;
            case SCUDO: printf("Scudo\n"); break;
        }

        printf("  Stanza Destra: %p\n", (void*)current->stanza_destra);
        printf("  Stanza Sinistra: %p\n", (void*)current->stanza_sinistra);
        printf("  Stanza Sopra: %p\n", (void*)current->stanza_sopra);
        printf("  Stanza Sotto: %p\n", (void*)current->stanza_sotto);

        current = current->stanza_destra;
        i++;
    }
}

// Funzione per la generazione casuale delle stanze
static void genera_random() {

    // Libera la memoria delle stanze esistenti
    while (pFirst != NULL) {
        canc_stanza();
    }

    // Crea NUM_RANDOM_ROOMS stanze casuali
    for (int i = 0; i < NUM_RANDOM_ROOMS; i++) {
        Stanza* nuova_stanza = crea_stanza();

        // Genera il tipo di stanza casualmente
        nuova_stanza->tipo = rand() % 10;

        // Genera il tipo di trabocchetto casualmente
        int random_trabocchetto = rand() % 100;
        if (random_trabocchetto < 65) {
            nuova_stanza->trabocchetto = NESSUNO;
        } else if (random_trabocchetto < 75) {
            nuova_stanza->trabocchetto = TEGOLA;
        } else if (random_trabocchetto < 84) {
            nuova_stanza->trabocchetto = LAME;
        } else if (random_trabocchetto < 92) {
            nuova_stanza->trabocchetto = CADUTA;
        } else {
            nuova_stanza->trabocchetto = BURRONE;
        }

        // Genera il tipo di tesoro casualmente
        int random_tesoro = rand() % 100;
        if (random_tesoro < 20) {
            nuova_stanza->tesoro = NESSUN_TESORO;
        } else if (random_tesoro < 40) {
            nuova_stanza->tesoro = VERDE_VELENO;
        } else if (random_tesoro < 60) {
            nuova_stanza->tesoro = BLU_GUARIGIONE;
        } else if (random_tesoro < 75) {
            nuova_stanza->tesoro = ROSSO_AUMENTA_VITA;
        } else if (random_tesoro < 90) {
            nuova_stanza->tesoro = SPADA_TAGLIENTE;
        } else {
            nuova_stanza->tesoro = SCUDO;
        }

        // Collega la nuova stanza alla mappa
        if (pFirst == NULL) {
            pFirst = nuova_stanza;
        } else {
            Stanza* current = pFirst;
            while (current->stanza_destra != NULL) {
                current = current->stanza_destra;
            }
            int direzione;

            do{
                direzione = rand() % 4;

                switch (direzione)
                {
                    case 0:
                        if(current->stanza_sopra == NULL){
                            current->stanza_sopra = nuova_stanza;
                        }
                        break;
                    case 1:
                        if(current->stanza_destra == NULL){
                            current->stanza_destra = nuova_stanza;
                        }
                        break;
                    case 2:
                        if(current->stanza_sinistra == NULL){
                            current->stanza_sinistra = nuova_stanza;
                        }
                        break;
                    case 3:
                        if(current->stanza_sotto == NULL){
                            current->stanza_sotto = nuova_stanza;
                        }
                        break;
                }
            } while((direzione == 0 && current->stanza_sopra == NULL) ||
                    (direzione == 1 && current->stanza_destra == NULL) ||
                    (direzione == 2 && current->stanza_sinistra == NULL) ||
                    (direzione == 3 && current->stanza_sotto == NULL));
        }
    }
    printf("Generate %d stanze casuali!\n", NUM_RANDOM_ROOMS);
}

static void chiudi_mappa(){
    int num_stanze = 0;
    Stanza *current = pFirst;
    while(current != NULL){
        num_stanze++;
        current = current->stanza_destra;
    }
    if(num_stanze < 15){
        printf("La mappa ha meno di 15 stanze (%d). Non si puo' chiudere.\n", num_stanze);
    } else {
        mappa_creata = 0;
        printf("Mappa chiusa!\n");
    }
}

// Funzione per impostare il gioco
void imposta_gioco() {
    // Inizializzazione del generatore di numeri casuali
    srand((unsigned)time(NULL));

    // Resetta le variabili globali
    mappa_creata = 1;

    for(int i = 0; i < 3; i++){
        if(giocatori[i] != NULL){
            free(giocatori[i]);
            giocatori[i] = NULL;
        }
    }
    while(pFirst != NULL){
        canc_stanza();
    }

    // Richiesta del numero di giocatori
    int num_giocatori;
    do {
        printf("Inserisci il numero di giocatori (1-%d): ", MAX_PLAYERS);
        scanf("%d", &num_giocatori);
    } while (num_giocatori < 1 || num_giocatori > MAX_PLAYERS);

    // Creazione dei giocatori
    for (int i = 0; i < num_giocatori; i++) {
        giocatori[i] = (Giocatore*)malloc(sizeof(Giocatore));
        if (giocatori[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione della memoria per il giocatore %d!\n", i + 1);
            exit(EXIT_FAILURE);
        }

        printf("Inserisci il nome del giocatore %d: ", i + 1);
        scanf(" %s", giocatori[i]->nome_giocatore);

        if (i == 0) {
            giocatori[i]->tipo = PRINCIPE;
        } else {
            giocatori[i]->tipo = DOPPLEGANGER;
        }

        giocatori[i]->posizione = NULL; // Verrà assegnata dopo la creazione della mappa
        giocatori[i]->p_vita_max = 10;
        giocatori[i]->p_vita = giocatori[i]->p_vita_max;
        giocatori[i]->dadi_attacco = 2;
        giocatori[i]->dadi_difesa = 2;
        giocatori[i]->turni_saltati = 0;
    }

    // Creazione della mappa
    printf("Creazione della mappa...\n");
    int scelta_mappa;

    do {
        printf("\nMenu creazione mappa:\n");
        printf("1. Inserisci stanza\n");
        printf("2. Cancella ultima stanza\n");
        printf("3. Stampa stanze\n");
        printf("4. Genera stanze casuali\n");
        printf("5. Chiudi mappa\n");
        printf("Scelta: ");
        scanf("%d", &scelta_mappa);

        switch (scelta_mappa) {
            case 1:
                ins_stanza();
                break;
            case 2:
                canc_stanza();
                break;
            case 3:
                stampa_stanze();
                break;
            case 4:
                genera_random();
                break;
            case 5:
                chiudi_mappa();
                break;
            default:
                printf("Scelta non valida!\n");
        }
    } while (mappa_creata);

    // Assegna la posizione iniziale ai giocatori
    for(int i = 0; i < num_giocatori; i++){
        giocatori[i]->posizione = pFirst;
    }
}
// Funzione per far avanzare il giocatore
static void avanza(Giocatore *g){
    // Controlla se il giocatore puo' avanzare
    if(g->posizione->stanza_sopra == NULL && g->posizione->stanza_destra == NULL &&
       g->posizione->stanza_sinistra == NULL && g->posizione->stanza_sotto == NULL){
        printf("Non c'e' una stanza in cui avanzare.\n");
        return;
    }

    // Stampa le direzioni in cui il giocatore puo' avanzare
    printf("Puoi avanzare nelle seguenti direzioni:\n");
    if(g->posizione->stanza_sopra != NULL){
        printf("0: Sopra\n");
    }
    if(g->posizione->stanza_destra != NULL){
        printf("1: Destra\n");
    }
    if(g->posizione->stanza_sinistra != NULL){
        printf("2: Sinistra\n");
    }
    if(g->posizione->stanza_sotto != NULL){
        printf("3: Sotto\n");
    }

    // Chiedi al giocatore in quale direzione vuole avanzare
    int direzione;

    do{
        printf("Scelta: ");
        scanf("%d", &direzione);
    } while((direzione == 0 && g->posizione->stanza_sopra == NULL) ||
            (direzione == 1 && g->posizione->stanza_destra == NULL) ||
            (direzione == 2 && g->posizione->stanza_sinistra == NULL) ||
            (direzione == 3 && g->posizione->stanza_sotto == NULL));

    // Avanza il giocatore nella direzione scelta
    switch (direzione)
    {
        case 0:
            g->posizione = g->posizione->stanza_sopra;
            break;
        case 1:
            g->posizione = g->posizione->stanza_destra;
            break;
        case 2:
            g->posizione = g->posizione->stanza_sinistra;
            break;
        case 3:
            g->posizione = g->posizione->stanza_sotto;
            break;
    }
    printf("%s e' avanzato nella stanza %d.\n", g->nome_giocatore, g->posizione->tipo);

    // Attiva l'effetto del trabocchetto
    switch(g->posizione->trabocchetto){
        case NESSUNO:
            printf("Non c'e' nessun trabocchetto in questa stanza.\n");
            break;
        case TEGOLA:
            printf("Sei stato colpito da una tegola!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                g->p_vita--;
            }
            break;
        case LAME:
            printf("Sei stato colpito dalle lame!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                g->p_vita -= 2;
            }
            break;
        case CADUTA:
            printf("Sei caduto in una botola!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                if(rand() % 2 == 0){
                    g->p_vita--;
                } else {
                    g->p_vita -= 2;
                }
            }
            break;
        case BURRONE:
            printf("Sei caduto in un burrone!\n");
            g->p_vita = 0;
            break;
    }

    // Controlla se il giocatore e' morto
    if(g->p_vita <= 0){
        printf("%s e' morto!\n", g->nome_giocatore);
        g->posizione = NULL;
        return;
    }

    // Controlla se c'e' un nemico
    if(g->posizione->tipo == TORRE){
        printf("Hai incontrato Jaffar!\n");
        // TODO: Combattimento con Jaffar
    } else {
        int random_nemico = rand() % 100;
        if(random_nemico < 25){
            if(random_nemico < 15){
                printf("Hai incontrato uno scheletro!\n");
                // TODO: Combattimento con lo scheletro
            } else {
                printf("Hai incontrato una guardia!\n");
                // TODO: Combattimento con la guardia
            }
        } else {
            printf("Non c'e' nessun nemico in questa stanza.\n");
        }
    }
}

static void combatti(Giocatore *g){

}

static void scappa(Giocatore *g){

}

static void stampa_giocatore(Giocatore *g){
    if(g == NULL) return;
    printf("Nome: %s\n", g->nome_giocatore);
    printf("Tipo: ");
    switch (g->tipo)
    {
        case PRINCIPE:
            printf("Principe\n");
            break;
        case DOPPLEGANGER:
            printf("Doppleganger\n");
            break;
    }
    printf("Posizione: ");
    if(g->posizione != NULL){
        switch (g->posizione->tipo) {
            case CORRIDOIO: printf("Corridoio\n"); break;
            case SCALA: printf("Scala\n"); break;
            case SALA_BANCHETTO: printf("Sala Banchetto\n"); break;
            case MAGAZZINO: printf("Magazzino\n"); break;
            case POSTO_GUARDIA: printf("Posto Guardia\n"); break;
            case PRIGIONE: printf("Prigione\n"); break;
            case ARMERIA: printf("Armeria\n"); break;
            case MOSCHEA: printf("Moschea\n"); break;
            case TORRE: printf("Torre\n"); break;
            case BAGNI: printf("Bagni\n"); break;
        }
    } else {
        printf("Nessuna\n");
    }

    printf("Punti vita: %d/%d\n", g->p_vita, g->p_vita_max);
    printf("Dadi attacco: %d\n", g->dadi_attacco);
    printf("Dadi difesa: %d\n", g->dadi_difesa);
    if(g->tipo == PRINCIPE){
        printf("Turni saltati: %d (Il Principe puo' ignorare il primo trabocchetto e puo' scappare una sola volta)\n", g->turni_saltati);
    }
    if(g->tipo == DOPPLEGANGER){
        printf("Turni saltati: %d (Il Doppleganger puo' scappare due volte)\n", g->turni_saltati);
    }
}

static void stampa_stanza(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    printf("Ti trovi in una stanza di tipo: ");
    switch (g->posizione->tipo) {
        case CORRIDOIO: printf("Corridoio\n"); break;
        case SCALA: printf("Scala\n"); break;
        case SALA_BANCHETTO: printf("Sala Banchetto\n"); break;
        case MAGAZZINO: printf("Magazzino\n"); break;
        case POSTO_GUARDIA: printf("Posto Guardia\n"); break;
        case PRIGIONE: printf("Prigione\n"); break;
        case ARMERIA: printf("Armeria\n"); break;
        case MOSCHEA: printf("Moschea\n"); break;
        case TORRE: printf("Torre\n"); break;
        case BAGNI: printf("Bagni\n"); break;
    }

    printf("Trabocchetto: ");
    switch (g->posizione->trabocchetto) {
        case NESSUNO: printf("Nessuno\n"); break;
        case TEGOLA: printf("Tegola\n"); break;
        case LAME: printf("Lame\n"); break;
        case CADUTA: printf("Caduta\n"); break;
        case BURRONE: printf("Burrone\n"); break;
    }

    printf("Tesoro: ");
    if(g->posizione->tesoro == NESSUN_TESORO){
        printf("Nessuno\n");
    } else {
        printf("C'e' un tesoro!\n");
    }
}

static void prendi_tesoro(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    if(g->posizione->tesoro == NESSUN_TESORO){
        printf("Non c'e' nessun tesoro da prendere!\n");
        return;
    }

    switch (g->posizione->tesoro)
    {
        case VERDE_VELENO:
            printf("Hai preso il Veleno Verde! Perdi un punto vita.\n");
            g->p_vita--;
            break;
        case BLU_GUARIGIONE:
            printf("Hai preso la Pozione Blu! Recuperi un punto vita.\n");
            g->p_vita = (g->p_vita < g->p_vita_max) ? g->p_vita + 1 : g->p_vita_max;
            break;
        case ROSSO_AUMENTA_VITA:
            printf("Hai preso la Pozione Rossa! Aumenti di uno i tuoi punti vita massimi.\n");
            g->p_vita_max++;
            g->p_vita++;
            break;
        case SPADA_TAGLIENTE:
            printf("Hai preso la Spada Tagliente! Aumenti di uno i tuoi dadi attacco.\n");
            g->dadi_attacco++;
            break;
        case SCUDO:
            printf("Hai preso lo Scudo! Aumenti di uno i tuoi dadi difesa.\n");
            g->dadi_difesa++;
            break;
        default:
            break;
    }
    g->posizione->tesoro = NESSUN_TESORO;
}

static void cerca_porta_segreta(Giocatore *g){

}

// Funzione per far passare il turno al prossimo giocatore
static int passa(Giocatore *g, int giocatore_corrente, int num_giocatori){
    int trovato = 0;
    int prossimo_giocatore = (giocatore_corrente + 1) % num_giocatori;
    while(trovato == 0){
        if(giocatori[prossimo_giocatore] != NULL && giocatori[prossimo_giocatore]->p_vita > 0){
            trovato = 1;
        } else {
            prossimo_giocatore = (prossimo_giocatore + 1) % num_giocatori;
        }
    }
    return prossimo_giocatore;
}

// Funzione per la gestione del gioco
void gioca() {
    if(mappa_creata == 1){
        printf("La mappa non e' ancora stata creata!\n");
        printf("Impostare il gioco prima di giocare.\n");
        return;
    }
    // Controlla se c'è almeno un principe tra i giocatori
    int principe_presente = 0;
    int num_giocatori = 0;
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(giocatori[i] != NULL){
            num_giocatori++;
            if(giocatori[i]->tipo == PRINCIPE){
                principe_presente = 1;
            }
        }
    }
    if(principe_presente == 0){
        printf("Non c'e' nessun principe tra i giocatori!\n");
        printf("Impostare il gioco prima di giocare.\n");
        return;
    }

    // Inizializzazione del turno e dell'ordine dei giocatori
    int turno = 1;
    int ordine_giocatori[MAX_PLAYERS];
    for(int i = 0; i < MAX_PLAYERS; i++){
        ordine_giocatori[i] = i;
    }

    // Ciclo di gioco
    int giocatore_corrente = 0;
    int fine_gioco = 0;
    while (fine_gioco == 0) {
        // Selezione del giocatore corrente in modo casuale, con l'accortezza di far giocare tutti prima di ripetere un giocatore
        if(turno == 1){
            for(int i = num_giocatori - 1; i > 0; i--){
                int j = rand() % (i + 1);
                int temp = ordine_giocatori[i];
                ordine_giocatori[i] = ordine_giocatori[j];
                ordine_giocatori[j] = temp;
            }
            giocatore_corrente = 0;
        } else if (turno > 1){
            giocatore_corrente = (giocatore_corrente + 1) % num_giocatori;
        }
        if(giocatori[ordine_giocatori[giocatore_corrente]] != NULL && giocatori[ordine_giocatori[giocatore_corrente]]->p_vita > 0){
            printf("\nTurno %d - Giocatore %s\n", turno, giocatori[ordine_giocatori[giocatore_corrente]]->nome_giocatore);

            // Azioni disponibili per il giocatore corrente
            int scelta_azione;
            do {
                printf("\nAzioni disponibili:\n");
                printf("1. Avanza\n");
                printf("2. Combatti\n");
                printf("3. Scappa\n");
                printf("4. Stampa giocatore\n");
                printf("5. Stampa stanza\n");
                printf("6. Prendi tesoro\n");
                printf("7. Cerca porta segreta\n");

// ... (codice precedente) ...

// Funzione per far avanzare il giocatore
static void avanza(Giocatore *g){
    // Controlla se il giocatore puo' avanzare
    if(g->posizione->stanza_sopra == NULL && g->posizione->stanza_destra == NULL &&
       g->posizione->stanza_sinistra == NULL && g->posizione->stanza_sotto == NULL){
        printf("Non c'e' una stanza in cui avanzare.\n");
        return;
    }

    // Stampa le direzioni in cui il giocatore puo' avanzare
    printf("Puoi avanzare nelle seguenti direzioni:\n");
    if(g->posizione->stanza_sopra != NULL){
        printf("0: Sopra\n");
    }
    if(g->posizione->stanza_destra != NULL){
        printf("1: Destra\n");
    }
    if(g->posizione->stanza_sinistra != NULL){
        printf("2: Sinistra\n");
    }
    if(g->posizione->stanza_sotto != NULL){
        printf("3: Sotto\n");
    }

    // Chiedi al giocatore in quale direzione vuole avanzare
    int direzione;

    do{
        printf("Scelta: ");
        scanf("%d", &direzione);
    } while((direzione < 0 || direzione > 3) ||
            (direzione == 0 && g->posizione->stanza_sopra == NULL) ||
            (direzione == 1 && g->posizione->stanza_destra == NULL) ||
            (direzione == 2 && g->posizione->stanza_sinistra == NULL) ||
            (direzione == 3 && g->posizione->stanza_sotto == NULL));

    // Avanza il giocatore nella direzione scelta
    switch (direzione)
    {
        case 0:
            g->posizione = g->posizione->stanza_sopra;
            break;
        case 1:
            g->posizione = g->posizione->stanza_destra;
            break;
        case 2:
            g->posizione = g->posizione->stanza_sinistra;
            break;
        case 3:
            g->posizione = g->posizione->stanza_sotto;
            break;
    }
    printf("%s e' avanzato nella stanza %d.\n", g->nome_giocatore, g->posizione->tipo);

    // Attiva l'effetto del trabocchetto
    switch(g->posizione->trabocchetto){
        case NESSUNO:
            printf("Non c'e' nessun trabocchetto in questa stanza.\n");
            break;
        case TEGOLA:
            printf("Sei stato colpito da una tegola!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                g->p_vita--;
            }
            break;
        case LAME:
            printf("Sei stato colpito dalle lame!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                g->p_vita -= 2;
            }
            break;
        case CADUTA:
            printf("Sei caduto in una botola!\n");
            if(g->tipo == PRINCIPE && g->turni_saltati == 0){
                printf("Essendo il Principe, non subisci danni dal primo trabocchetto.\n");
                g->turni_saltati++;
            } else {
                if(rand() % 2 == 0){
                    g->p_vita--;
                } else {
                    g->p_vita -= 2;
                }
            }
            break;
        case BURRONE:
            printf("Sei caduto in un burrone!\n");
            g->p_vita = 0;
            break;
    }

    // Controlla se il giocatore e' morto
    if(g->p_vita <= 0){
        printf("%s e' morto!\n", g->nome_giocatore);
        g->posizione = NULL;
        return;
    }

    // Controlla se c'e' un nemico
    if(g->posizione->tipo == TORRE){
        printf("Hai incontrato Jaffar!\n");
        // TODO: Combattimento con Jaffar
    } else {
        int random_nemico = rand() % 100;
        if(random_nemico < 25){
            if(random_nemico < 15){
                printf("Hai incontrato uno scheletro!\n");
                // TODO: Combattimento con lo scheletro
            } else {
                printf("Hai incontrato una guardia!\n");
                // TODO: Combattimento con la guardia
            }
        } else {
            printf("Non c'e' nessun nemico in questa stanza.\n");
        }
    }
}

// Funzione per gestire il combattimento
static void combatti(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    // Controlla se c'e' un nemico
    int nemico; // 0 = scheletro, 1 = guardia, 2 = Jaffar
    int p_vita_nemico;
    int dadi_attacco_nemico;
    int dadi_difesa_nemico;
    if(g->posizione->tipo == TORRE){
        nemico = 2;
        p_vita_nemico = 3;
        dadi_attacco_nemico = 3;
        dadi_difesa_nemico = 2;
        printf("Inizia lo scontro con Jaffar!\n");
    } else {
        int random_nemico = rand() % 100;
        if(random_nemico < 25){
            if(random_nemico < 15){
                nemico = 0;
                p_vita_nemico = 1;
                dadi_attacco_nemico = 1;
                dadi_difesa_nemico = 1;
                printf("Inizia lo scontro con uno scheletro!\n");
            } else {
                nemico = 1;
                p_vita_nemico = 2;
                dadi_attacco_nemico = 2;
                dadi_difesa_nemico = 2;
                printf("Inizia lo scontro con una guardia!\n");
            }
        } else {
            printf("Non c'e' nessun nemico da combattere in questa stanza.\n");
            return;
        }
    }

    while(g->p_vita > 0 && p_vita_nemico > 0){
        // Determina chi attacca per primo
        int attacco_giocatore = rand() % 6 + 1;
        int attacco_nemico = rand() % 6 + 1;
        printf("%s tira un dado per l'attacco: %d\n", g->nome_giocatore, attacco_giocatore);
        if(nemico == 0){
            printf("Lo scheletro tira un dado per l'attacco: %d\n", attacco_nemico);
        } else if (nemico == 1){
            printf("La guardia tira un dado per l'attacco: %d\n", attacco_nemico);
        } else {
            printf("Jaffar tira un dado per l'attacco: %d\n", attacco_nemico);
        }

        // Attacco del giocatore
        if(attacco_giocatore >= attacco_nemico){
            printf("%s attacca per primo!\n", g->nome_giocatore);
            int danni_inflitti = 0;
            for(int i = 0; i < g->dadi_attacco; i++){
                int attacco = rand() % 6 + 1;
                printf("%s tira un dado di attacco: %d\n", g->nome_giocatore, attacco);
                if(attacco >= 4){
                    if(attacco == 6){
                        printf("Colpo critico!\n");
                        danni_inflitti += 2;
                    } else {
                        danni_inflitti++;
                    }
                }
            }

            int danni_subiti = 0;
            for(int i = 0; i < dadi_difesa_nemico; i++){
                int difesa = rand() % 6 + 1;
                if(nemico == 0){
                    printf("Lo scheletro tira un dado di difesa: %d\n", difesa);
                } else if (nemico == 1){
                    printf("La guardia tira un dado di difesa: %d\n", difesa);
                } else {
                    printf("Jaffar tira un dado di difesa: %d\n", difesa);
                }
                if(difesa >= 4){
                    if(difesa == 6){
                        printf("Difesa critica!\n");
                        danni_subiti += 2;
                    } else {
                        danni_subiti++;
                    }
                }
            }

            int danni = danni_inflitti - danni_subiti;
            if(danni > 0){
                printf("%s infligge %d danni!\n", g->nome_giocatore, danni);
                p_vita_nemico -= danni;
            } else {
                printf("%s non infligge danni.\n", g->nome_giocatore);
            }
        } else { // Attacco del nemico
            printf("Il nemico attacca per primo!\n");
            int danni_inflitti = 0;
            for(int i = 0; i < dadi_attacco_nemico; i++){
                int attacco = rand() % 6 + 1;
                if(nemico == 0){
                    printf("Lo scheletro tira un dado di attacco: %d\n", attacco);
                } else if (nemico == 1){
                    printf("La guardia tira un dado di attacco: %d\n", attacco);
                } else {
                    printf("Jaffar tira un dado di attacco: %d\n", attacco);
                }
                if(attacco >= 4){
                    if(attacco == 6){
                        printf("Colpo critico!\n");
                        danni_inflitti += 2;
                    } else {
                        danni_inflitti++;
                    }
                }
            }

            int danni_subiti = 0;
            for(int i = 0; i < g->dadi_difesa; i++){
                int difesa = rand() % 6 + 1;
                printf("%s tira un dado di difesa: %d\n", g->nome_giocatore, difesa);
                if(difesa >= 4){
                    if(difesa == 6){
                        printf("Difesa critica!\n");
                        danni_subiti += 2;
                    } else {
                        danni_subiti++;
                    }
                }
            }

            int danni = danni_inflitti - danni_subiti;
            if(danni > 0){
                printf("Il nemico infligge %d danni!\n", danni);
                g->p_vita -= danni;
            } else {
                printf("Il nemico non infligge danni.\n");
            }
        }
    }

    if(g->p_vita <= 0){
        printf("%s e' stato sconfitto!\n", g->nome_giocatore);
        g->posizione = NULL;
    } else {
        printf("Il nemico e' stato sconfitto!\n");
        g->p_vita++;
        if(nemico == 2){
            strcpy(nomi_vincitori[indice_vincitori], g->nome_giocatore);
            indice_vincitori = (indice_vincitori + 1) % 3;
        }
    }
}

// Funzione per scappare dal combattimento
static void scappa(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    // Controlla se il giocatore e' il principe e puo' scappare solo una volta
    if(g->tipo == PRINCIPE && g->turni_saltati >= 1){
        printf("Il Principe non puo' scappare piu' di una volta!\n");
        return;
    }
    // Controlla se il giocatore e' un doppleganger e puo' scappare due volte
    if(g->tipo == DOPPLEGANGER && g->turni_saltati >= 2){
        printf("Il Doppleganger non puo' scappare piu' di due volte!\n");
        return;
    }

    // Controlla se c'e' un nemico
    int random_nemico = rand() % 100;
    if(random_nemico > 25 && g->posizione->tipo != TORRE){
        printf("Non c'e' nessun nemico da cui scappare!\n");
        return;
    }

    // Sposta il giocatore nella stanza precedente
    Stanza *stanza_precedente = NULL;
    if(pFirst != g->posizione){
        Stanza *current = pFirst;
        while(current != NULL){
            if(current->stanza_sopra == g->posizione){
                stanza_precedente = current;
                break;
            }
            if(current->stanza_destra == g->posizione){
                stanza_precedente = current;
                break;
            }
            if(current->stanza_sinistra == g->posizione){
                stanza_precedente = current;
                break;
            }
            if(current->stanza_sotto == g->posizione){
                stanza_precedente = current;
                break;
            }
            current = current->stanza_destra;
        }
    }

    if(stanza_precedente != NULL){
        g->posizione = stanza_precedente;
        g->turni_saltati++;
        printf("%s e' scappato nella stanza precedente!\n", g->nome_giocatore);
    } else {
        printf("Non c'e' una stanza precedente in cui scappare!\n");
    }
}

// Funzione per stampare le informazioni del giocatore
static void stampa_giocatore(Giocatore *g){
    if(g == NULL) return;
    printf("Nome: %s\n", g->nome_giocatore);
    printf("Tipo: ");
    switch (g->tipo)
    {
        case PRINCIPE:
            printf("Principe\n");
            break;
        case DOPPLEGANGER:
            printf("Doppleganger\n");
            break;
    }
    printf("Posizione: ");
    if(g->posizione != NULL){
        switch (g->posizione->tipo) {
            case CORRIDOIO: printf("Corridoio\n"); break;
            case SCALA: printf("Scala\n"); break;
            case SALA_BANCHETTO: printf("Sala Banchetto\n"); break;
            case MAGAZZINO: printf("Magazzino\n"); break;
            case POSTO_GUARDIA: printf("Posto Guardia\n"); break;
            case PRIGIONE: printf("Prigione\n"); break;
            case ARMERIA: printf("Armeria\n"); break;
            case MOSCHEA: printf("Moschea\n"); break;
            case TORRE: printf("Torre\n"); break;
            case BAGNI: printf("Bagni\n"); break;
        }
    } else {
        printf("Nessuna\n");
    }

    printf("Punti vita: %d/%d\n", g->p_vita, g->p_vita_max);
    printf("Dadi attacco: %d\n", g->dadi_attacco);
    printf("Dadi difesa: %d\n", g->dadi_difesa);
    if(g->tipo == PRINCIPE){
        printf("Turni saltati: %d (Il Principe puo' ignorare il primo trabocchetto e puo' scappare una sola volta)\n", g->turni_saltati);
    }
    if(g->tipo == DOPPLEGANGER){
        printf("Turni saltati: %d (Il Doppleganger puo' scappare due volte)\n", g->turni_saltati);
    }
}

// Funzione per stampare le informazioni della stanza in cui si trova il giocatore
static void stampa_stanza(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    printf("Ti trovi in una stanza di tipo: ");
    switch (g->posizione->tipo) {
        case CORRIDOIO: printf("Corridoio\n"); break;
        case SCALA: printf("Scala\n"); break;
        case SALA_BANCHETTO: printf("Sala Banchetto\n"); break;
        case MAGAZZINO: printf("Magazzino\n"); break;
        case POSTO_GUARDIA: printf("Posto Guardia\n"); break;
        case PRIGIONE: printf("Prigione\n"); break;
        case ARMERIA: printf("Armeria\n"); break;
        case MOSCHEA: printf("Moschea\n"); break;
        case TORRE: printf("Torre\n"); break;
        case BAGNI: printf("Bagni\n"); break;
    }

    printf("Trabocchetto: ");
    switch (g->posizione->trabocchetto) {
        case NESSUNO: printf("Nessuno\n"); break;
        case TEGOLA: printf("Tegola\n"); break;
        case LAME: printf("Lame\n"); break;
        case CADUTA: printf("Caduta\n"); break;
        case BURRONE: printf("Burrone\n"); break;
    }

    printf("Tesoro: ");
    if(g->posizione->tesoro == NESSUN_TESORO){
        printf("Nessuno\n");
    } else {
        printf("C'e' un tesoro!\n");
    }
}

// Funzione per prendere il tesoro dalla stanza
static void prendi_tesoro(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    if(g->posizione->tesoro == NESSUN_TESORO){
        printf("Non c'e' nessun tesoro da prendere!\n");
        return;
    }

    switch (g->posizione->tesoro)
    {
        case VERDE_VELENO:
            printf("Hai preso il Veleno Verde! Perdi un punto vita.\n");
            g->p_vita--;
            break;
        case BLU_GUARIGIONE:
            printf("Hai preso la Pozione Blu! Recuperi un punto vita.\n");
            g->p_vita = (g->p_vita < g->p_vita_max) ? g->p_vita + 1 : g->p_vita_max;
            break;
        case ROSSO_AUMENTA_VITA:
            printf("Hai preso la Pozione Rossa! Aumenti di uno i tuoi punti vita massimi.\n");
            g->p_vita_max++;
            g->p_vita++;
            break;
        case SPADA_TAGLIENTE:
            printf("Hai preso la Spada Tagliente! Aumenti di uno i tuoi dadi attacco.\n");
            g->dadi_attacco++;
            break;
        case SCUDO:
            printf("Hai preso lo Scudo! Aumenti di uno i tuoi dadi difesa.\n");
            g->dadi_difesa++;
            break;
        default:
            break;
    }
    g->posizione->tesoro = NESSUN_TESORO;
}

// Funzione per cercare una porta segreta
static void cerca_porta_segreta(Giocatore *g){
    if(g == NULL || g->posizione == NULL) return;
    printf("Stai cercando una porta segreta...\n");
    int random = rand() % 3;
    if(random == 0){
        printf("Hai trovato una porta segreta!\n");
        Stanza *nuova_stanza = crea_stanza();
        nuova_stanza->tipo = rand() % 10;
        int random_trabocchetto = rand() % 100;
        if (random_trabocchetto < 65) {
            nuova_stanza->trabocchetto = NESSUNO;
        } else if (random_trabocchetto < 75) {
            nuova_stanza->trabocchetto = TEGOLA;
        } else if (random_trabocchetto < 84) {
            nuova_stanza->trabocchetto = LAME;
        } else if (random_trabocchetto < 92) {
            nuova_stanza->trabocchetto = CADUTA;
        } else {
            nuova_stanza->trabocchetto = BURRONE;
        }
        int random_tesoro = rand() % 100;
        if (random_tesoro < 20) {
            nuova_stanza->tesoro = NESSUN_TESORO;
        } else if (random_tesoro < 40) {
            nuova_stanza->tesoro = VERDE_VELENO;
        } else if (random_tesoro < 60) {
            nuova_stanza->tesoro = BLU_GUARIGIONE;
        } else if (random_tesoro < 75) {
            nuova_stanza->tesoro = ROSSO_AUMENTA_VITA;
        } else if (random_tesoro < 90) {
            nuova_stanza->tesoro = SPADA_TAGLIENTE;
        } else {
            nuova_stanza->tesoro = SCUDO;
        }

        int direzione;
        do{
            direzione = rand() % 4;
            switch(direzione){
                case 0:
                    if(g->posizione->stanza_sopra == NULL){
                        g->posizione->stanza_sopra = nuova_stanza;
                    }
                    break;
                case 1:
                    if(g->posizione->stanza_destra == NULL){
                        g->posizione->stanza_destra = nuova_stanza;
                    }
                    break;
                case 2:
                    if(g->posizione->stanza_sinistra == NULL){
                        g->posizione->stanza_sinistra = nuova_stanza;
                    }
                    break;
                case 3:
                    if(g->posizione->stanza_sotto == NULL){
                        g->posizione->stanza_sotto = nuova_stanza;
                    }
                    break;
            }
        } while((direzione == 0 && g->posizione->stanza_sopra == NULL) ||
                (direzione == 1 && g->posizione->stanza_destra == NULL) ||
                (direzione == 2 && g->posizione->stanza_sinistra == NULL) ||
                (direzione == 3 && g->posizione->stanza_sotto == NULL));

        switch (direzione)
        {
            case 0:
                g->posizione = g->posizione->stanza_sopra;
                break;
            case 1:
                g->posizione = g->posizione->stanza_destra;
                break;
            case 2:
                g->posizione = g->posizione->stanza_sinistra;
                break;
            case 3:
                g->posizione = g->posizione->stanza_sotto;
                break;
        }

        printf("Ora ti trovi nella stanza segreta.\n");
        stampa_stanza(g);
    } else {
        printf("Non hai trovato nessuna porta segreta.\n");
    }
}

// Funzione per far passare il turno al prossimo giocatore
static int passa(Giocatore *g, int giocatore_corrente, int num_giocatori){
    int trovato = 0;
    int prossimo_giocatore = (giocatore_corrente + 1) % num_giocatori;
    while(trovato == 0){
        if(giocatori[prossimo_giocatore] != NULL && giocatori[prossimo_giocatore]->p_vita > 0){
            trovato = 1;
        } else {
            prossimo_giocatore = (prossimo_giocatore + 1) % num_giocatori;
        }
    }
    return prossimo_giocatore;
}

// Funzione per la gestione del gioco
void gioca() {
    if(mappa_creata == 1){
        printf("La mappa non e' ancora stata creata!\n");
        printf("Impostare il gioco prima di giocare.\n");
        return;
    }
    // Controlla se c'è almeno un principe tra i giocatori
    int principe_presente = 0;
    int num_giocatori = 0;
    for(int i = 0; i < MAX_PLAYERS; i++){
        if(giocatori[i] != NULL){
            num_giocatori++;
            if(giocatori[i]->tipo == PRINCIPE){
                principe_presente = 1;
            }
        }
    }
    if(principe_presente == 0){
        printf("Non c'e' nessun principe tra i giocatori!\n");
        printf("Impostare il gioco prima di giocare.\n");
        return;
    }

    // Inizializzazione del turno e dell'ordine dei giocatori
    int turno = 1;
    int ordine_giocatori[MAX_PLAYERS];
    for(int i = 0; i < MAX_PLAYERS; i++){
        ordine_giocatori[i] = i;
    }

    // Ciclo di gioco
    int giocatore_corrente = 0;
    int fine_gioco = 0;
    while (fine_gioco == 0) {
        // Selezione del giocatore corrente in modo casuale, con l'accortezza di far giocare tutti prima di ripetere un giocatore
        if(turno == 1){
            for(int i = num_giocatori - 1; i > 0; i--){
                int j = rand() % (i + 1);
                int temp = ordine_giocatori[i];
                ordine_giocatori[i] = ordine_giocatori[j];
                ordine_giocatori[j] = temp;
            }
            giocatore_corrente = 0;
        } else if (turno > 1){
            giocatore_corrente = (giocatore_corrente + 1) % num_giocatori;
        }
        if(giocatori[ordine_giocatori[giocatore_corrente]] != NULL && giocatori[ordine_giocatori[giocatore_corrente]]->p_vita > 0){
            printf("\nTurno %d - Giocatore %s\n", turno, giocatori[ordine_giocatori[giocatore_corrente]]->nome_giocatore);

            // Azioni disponibili per il giocatore corrente
            int scelta_azione;
            int ha_avanzato = 0;
            do {
                printf("\nAzioni disponibili:\n");
                printf("1. Avanza\n");
                printf("2. Combatti\n");
                printf("3. Scappa\n");
                printf("4. Stampa giocatore\n");
                printf("5. Stampa stanza\n");
                printf("6. Prendi tesoro\n");
                printf("7. Cerca porta segreta\n");
                printf("8. Passa\n");
                printf("Scelta: ");
                scanf("%d", &scelta_azione);

                switch (scelta_azione) {
                    case 1:
                        if(ha_avanzato == 0){
                            avanza(giocatori[ordine_giocatori[giocatore_corrente]]);
                            ha_avanzato = 1;
                        } else {
                            printf("Hai gia' avanzato in questo turno!\n");
                        }
                        break;
                    case 2:
                        combatti(giocatori[ordine_giocatori[giocatore_corrente]]);
                        break;
                    case 3:
                        scappa(giocatori[ordine_giocatori[giocatore_corrente]]);
                        break;
                    case 4:
                        stampa_giocatore(giocatori[ordine_giocatori[giocatore_corrente]]);
                        break;
                    case 5:
                        stampa_stanza(giocatori[ordine_giocatori[giocatore_corrente]]);
                        break;
                    case 6:
                        prendi_tesoro(giocatori[ordine_giocatori[giocatore_corrente]]);
                        break;
                    case 7:
                        if(ha_avanzato == 0){
                            cerca_porta_segreta(giocatori[ordine_giocatori[giocatore_corrente]]);
                        } else {
                            printf("Hai gia' avanzato in questo turno!\n");
                        }
                        break;
                    case 8:
                        giocatore_corrente = passa(giocatori[ordine_giocatori[giocatore_corrente]], giocatore_corrente, num_giocatori);
                        turno++;
                        ha_avanzato = 0;
                        break;
                    default:
                        printf("Scelta non valida!\n");
                }
            } while (scelta_azione != 8);
        } else {
            giocatore_corrente = (giocatore_corrente + 1) % num_giocatori;
        }

        // Controlla se il gioco e' finito
        int giocatori_vivi = 0;
        for(int i = 0; i < MAX_PLAYERS; i++){
            if(giocatori[i] != NULL && giocatori[i]->p_vita > 0){
                giocatori_vivi++;
            }
        }
        if(giocatori_vivi == 0){
            printf("Tutti i giocatori sono morti!\n");
            fine_gioco = 1;
        }
        int principe_vivo = 0;
        for(int i = 0; i < MAX_PLAYERS; i++){
            if(giocatori[i] != NULL && giocatori[i]->tipo == PRINCIPE && giocatori[i]->p_vita > 0){
                principe_vivo = 1;
            }
        }
        if(principe_vivo == 0 && giocatori_vivi > 0){
            printf("Il principe e' morto! Vince il giocatore con piu' punti vita.\n");
            int vincitore = 0;
            int max_punti_vita = 0;
            for(int i = 0; i < MAX_PLAYERS; i++){
                if(giocatori[i] != NULL && giocatori[i]->p_vita > 0){
                    if(giocatori[i]->p_vita > max_punti_vita){
                        max_punti_vita = giocatori[i]->p_vita;
                        vincitore = i;
                    }
                }
            }
            strcpy(nomi_vincitori[indice_vincitori], giocatori[vincitore]->nome_giocatore);
            indice_vincitori = (indice_vincitori + 1) % 3;
            fine_gioco = 1;
        }
        if(fine_gioco == 1){
            termina_gioco();
        }
    }
}

// Funzione per terminare il gioco
void termina_gioco() {
    printf("Gioco terminato!\n");
    crediti();
    printf("Premi invio per tornare al menu principale...\n");
    while(getchar() != '\n');
    getchar();
}

// Funzione per visualizzare i crediti
void crediti() {
    printf("Gioco creato da: [Tuo Nome]\n");
    printf("Vincitori delle ultime tre partite:\n");
    for(int i = 0; i < 3; i++){
        if(strlen(nomi_vincitori[i]) > 0){
            printf("%d. %s\n", i + 1, nomi_vincitori[i]);
        } else {
            printf("%d. Nessun vincitore\n", i + 1);
        }
    }
}
