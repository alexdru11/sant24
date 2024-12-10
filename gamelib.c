#include "gamelib.h"

// Variabili globali
static Stanza *primaStanza = NULL; // Puntatore alla prima stanza della mappa
static Giocatore *giocatori[MAX_PLAYERS]; // Array di puntatori ai giocatori
static int mappa_creata = 0; // Flag per indicare se la mappa è stata creata (0: no, 1: sì)
static char nomi_vincitori[3][MAX_NAME_LENGTH] = {"", "", ""}; // Nomi degli ultimi tre vincitori
static int num_giocatori;

// Prototipi funzioni static
static void ins_stanza();
static void canc_stanza();
static void stampa_stanze();
static void genera_random();
static void chiudi_mappa();
static void avanza(Giocatore *giocatore);
static int combatti(Giocatore *giocatore);
static void scappa(Giocatore *giocatore);
static void stampa_giocatore(Giocatore *giocatore);
static void stampa_zona(Giocatore *giocatore);
static void prendi_tesoro(Giocatore *giocatore);
static void cerca_porta_segreta(Giocatore *giocatore);
static void applica_effetto_trabocchetto(Giocatore *giocatore);
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro);
static void libera_mappa();
static int lancia_dado();
static void inizializza_giocatori();
static void sposta_giocatore(Giocatore * giocatore, Stanza * nuova_stanza);
static void stampa_menu_imposta_gioco();
static int scegli_direzione(Stanza *stanza);
static Stanza *crea_stanza_random();
static void collega_stanza(Stanza *stanza_precedente, Stanza *nuova_stanza);
static int conta_stanze();
static void mescola_array(int arr[], int n);

// Funzione per impostare il gioco
void imposta_gioco() {
    if(mappa_creata){
        libera_mappa();
    }

    
    inizializza_giocatori();

    int scelta;
    do {
        stampa_menu_imposta_gioco();
        
        // Controllo dell'input per la scelta
        if (scanf("%d", &scelta) != 1) {
            printf("Errore: inserisci un numero valido.\n");
            while (getchar() != '\n'); // Pulisce il buffer di input
            continue;
        }

        switch (scelta) {
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
                if(primaStanza != NULL){
                    if(conta_stanze() < MAX_ROOMS){
                        printf("La mappa deve avere almeno 15 stanze!\n");
                    } else {
                         chiudi_mappa();
                         printf("Mappa creata con successo!\n");
                         return;
                    }
                } else {
                    printf("La mappa non e' ancora stata creata!\n");
                }
               
                break;
            default:
                printf("Comando non valido.\n");
        }
    } while (scelta != 5);
}

// Funzione per inserire una stanza
static void ins_stanza() {
    Stanza *nuova_stanza = (Stanza *)malloc(sizeof(Stanza));
    if (nuova_stanza == NULL) {
        printf("Errore: impossibile allocare memoria per la nuova stanza.\n");
        return;
    }

    // Inizializzazione valori per la nuova stanza
    nuova_stanza->trabocchetto_attivato = 0;
    for(int i = 0; i < 4; i++){
        nuova_stanza->tentativi_porta[i] = 0;
    }

    // Inserimento valori della stanza da tastiera
    int temp;
    printf("Inserisci il tipo della stanza:\n");
    printf("0 - CORRIDOIO\n1 - SCALA\n2 - SALA_BANCHETTO\n3 - MAGAZZINO\n");
    printf("4 - POSTO_GUARDIA\n5 - PRIGIONE\n6 - ARMERIA\n7 - MOSCHEA\n");
    printf("8 - TORRE\n9 - BAGNI\n");
    if(scanf("%d", &temp) != 1 || temp < 0 || temp > 9){
        printf("Errore: inserisci un numero valido (0-9).\n");
        free(nuova_stanza);
        while (getchar() != '\n');
        return;
    }
    nuova_stanza->tipo = (Tipo_stanza)temp;

    printf("Inserisci il tipo di trabocchetto:\n");
    printf("0 - NESSUNO\n1 - TEGOLA\n2 - LAME\n3 - CADUTA\n4 - BURRONE\n");
    if(scanf("%d", &temp) != 1 || temp < 0 || temp > 4){
        printf("Errore: inserisci un numero valido (0-4).\n");
        free(nuova_stanza);
        while (getchar() != '\n');
        return;
    }
    nuova_stanza->trabocchetto = (Tipo_trabocchetto)temp;

    printf("Inserisci il tipo di tesoro:\n");
    printf("0 - NESSUN_TESORO\n1 - VERDE_VELENO\n2 - BLU_GUARIGIONE\n");
    printf("3 - ROSSO_AUMENTA_VITA\n4 - SPADA_TAGLIENTE\n5 - SCUDO\n");
    if(scanf("%d", &temp) != 1 || temp < 0 || temp > 5){
        printf("Errore: inserisci un numero valido (0-5).\n");
        free(nuova_stanza);
        while (getchar() != '\n');
        return;
    }
    nuova_stanza->tesoro = (Tipo_tesoro)temp;

    nuova_stanza->stanza_destra = NULL;
    nuova_stanza->stanza_sinistra = NULL;
    nuova_stanza->stanza_sopra = NULL;
    nuova_stanza->stanza_sotto = NULL;

    // Inserimento in coda
    if (primaStanza == NULL) {
        primaStanza = nuova_stanza;
    } else {
        Stanza *current = primaStanza;
        while (current->stanza_destra != NULL) {
            current = current->stanza_destra;
        }
        
        
        int direzione = scegli_direzione(current);

        switch(direzione){
            case 0:
                if(current->stanza_sopra == NULL){
                    current->stanza_sopra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 1:
                if(current->stanza_sotto == NULL){
                    current->stanza_sotto = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 2:
                if(current->stanza_sinistra == NULL){
                    current->stanza_sinistra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 3:
                if(current->stanza_destra == NULL){
                    current->stanza_destra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
        }
    }
}

// Funzione per cancellare l'ultima stanza inserita
static void canc_stanza() {
    if (primaStanza == NULL) {
        printf("Nessuna stanza da cancellare.\n");
        return;
    }

    if (primaStanza->stanza_destra == NULL && primaStanza->stanza_sotto == NULL && primaStanza->stanza_sopra == NULL && primaStanza->stanza_sinistra == NULL) {
        free(primaStanza);
        primaStanza = NULL;
        return;
    }

    Stanza *current = primaStanza;
    Stanza *prev = NULL;
    while (current->stanza_destra != NULL || current->stanza_sotto != NULL || current->stanza_sopra != NULL || current->stanza_sinistra != NULL) {
        prev = current;
        if(current->stanza_destra != NULL){
            current = current->stanza_destra;
        } else if (current->stanza_sotto != NULL){
            current = current->stanza_sotto;
        } else if (current->stanza_sopra != NULL){
            current = current->stanza_sopra;
        } else {
            current = current->stanza_sinistra;
        }
    }

    if(prev->stanza_destra == current){
        prev->stanza_destra = NULL;
    } else if (prev->stanza_sotto == current){
        prev->stanza_sotto = NULL;
    } else if (prev->stanza_sopra == current){
        prev->stanza_sopra = NULL;
    } else if (prev->stanza_sinistra == current){
        prev->stanza_sinistra = NULL;
    }

    free(current);
}

// Funzione per stampare le stanze
static void stampa_stanze() {
    if (primaStanza == NULL) {
        printf("Nessuna stanza creata.\n");
        return;
    }

    Stanza *current = primaStanza;
    int i = 0;
    while (current != NULL) {
        printf("Stanza %d:\n", i + 1);
        printf("Tipo: %d\n", current->tipo);
        printf("Trabocchetto: %d\n", current->trabocchetto);
        printf("Tesoro: %d\n", current->tesoro);
        printf("Stanza destra: %s\n", current->stanza_destra != NULL ? "Collegata" : "Non collegata");
        printf("Stanza sinistra: %s\n", current->stanza_sinistra != NULL ? "Collegata" : "Non collegata");
        printf("Stanza sopra: %s\n", current->stanza_sopra != NULL ? "Collegata" : "Non collegata");
        printf("Stanza sotto: %s\n", current->stanza_sotto != NULL ? "Collegata" : "Non collegata");
        printf("\n");
        
        if(current->stanza_destra != NULL){
            current = current->stanza_destra;
        } else if (current->stanza_sotto != NULL){
            current = current->stanza_sotto;
        } else if (current->stanza_sopra != NULL){
            current = current->stanza_sopra;
        } else if (current->stanza_sinistra != NULL){
            current = current->stanza_sinistra;
        } else {
            current = NULL;
        }
        i++;
    }
}

// Funzione per generare una mappa casuale
static void genera_random() {
    // Cancella la mappa esistente
    libera_mappa();

    // Crea 15 stanze casuali
    for (int i = 0; i < MAX_ROOMS; i++) {
        Stanza *nuova_stanza = crea_stanza_random();

        // Inserimento in coda
        if (primaStanza == NULL) {
            primaStanza = nuova_stanza;
        } else {
            Stanza *current = primaStanza;

            // Trova una stanza a cui collegare la nuova stanza
            while (current != NULL) {
                
                // Prova a collegare la stanza in una direzione casuale
                if(current->stanza_sopra == NULL || current->stanza_sotto == NULL || current->stanza_sinistra == NULL || current->stanza_destra == NULL){
                    collega_stanza(current, nuova_stanza);
                }

                //Se una direzione è libera, collega la nuova stanza e passa alla prossima iterazione
                if(current->stanza_sopra == nuova_stanza || current->stanza_sotto == nuova_stanza || current->stanza_sinistra == nuova_stanza || current->stanza_destra == nuova_stanza){
                    break;
                } else {
                    // Altrimenti, passa alla prossima stanza
                    if(current->stanza_destra != NULL){
                        current = current->stanza_destra;
                    } else if (current->stanza_sotto != NULL){
                        current = current->stanza_sotto;
                    } else if (current->stanza_sopra != NULL){
                        current = current->stanza_sopra;
                    } else {
                        current = current->stanza_sinistra;
                    }
                }
            }
            
             //Se non si riesce a collegare la stanza in nessun modo la si elimina
            if(current == NULL){
                free(nuova_stanza);
                printf("Impossibile collegare la stanza, verra eliminata.\n");
                return;
            }
        }
    }
}

// Funzione per chiudere la mappa
static void chiudi_mappa() {
    mappa_creata = 1;
}

// Funzione per iniziare il gioco
void gioca() {
    if (!mappa_creata || primaStanza == NULL) { 
        printf("Errore: la mappa non è ancora stata creata o è vuota.\n");
        return;
    }

    // Resetta i punti vita e la posizione dei giocatori
    for(int i = 0; i < num_giocatori; i++){
        giocatori[i]->p_vita = giocatori[i]->p_vita_max;
        giocatori[i]->posizione = primaStanza;
        giocatori[i]->fuggito = 0;
    }

    //Inizializza array di indici
    int ordine_giocatori[MAX_PLAYERS];
    for (int i = 0; i < num_giocatori; i++) {
        ordine_giocatori[i] = i;
    }

    // Ciclo di gioco
    int gioco_finito = 0;
    int turno_giocatore = 0;
    while (!gioco_finito) {
        // Mescola l'ordine dei giocatori all'inizio di ogni round
        if (turno_giocatore == 0) {
            mescola_array(ordine_giocatori, num_giocatori);
        }

        Giocatore *giocatore_corrente = giocatori[ordine_giocatori[turno_giocatore]];
        
        printf("Turno di %s\n", giocatore_corrente->nome_giocatore);

        int scelta;
        int ha_avanzato = 0;
        int azione_eseguita = 0;
        do {
            azione_eseguita = 0;
            printf("Scegli un'azione:\n");
            printf("1 - Avanza\n");
            printf("2 - Combatti\n");
            printf("3 - Scappa\n");
            printf("4 - Stampa giocatore\n");
            printf("5 - Stampa zona\n");
            printf("6 - Prendi tesoro\n");
            printf("7 - Cerca porta segreta\n");
            if (scanf("%d", &scelta) != 1) {
                printf("Errore: inserisci un numero valido.\n");
                while (getchar() != '\n'); // Pulisce il buffer di input
                continue;
            }

            switch (scelta) {
                case 1:
                    if(!ha_avanzato){
                        avanza(giocatore_corrente);
                        ha_avanzato = 1;
                        azione_eseguita = 1;
                    } else {
                        printf("Hai gia' avanzato in questo turno!\n");
                    }
                    break;
                case 2:
                   if(combatti(giocatore_corrente)){
                        gioco_finito = 1;
                   }
                   azione_eseguita = 1;
                    break;
                case 3:
                    scappa(giocatore_corrente);
                    azione_eseguita = 1;
                    break;
                case 4:
                    stampa_giocatore(giocatore_corrente);
                    break;
                case 5:
                    stampa_zona(giocatore_corrente);
                    break;
                case 6:
                    prendi_tesoro(giocatore_corrente);
                    azione_eseguita = 1;
                    break;
                case 7:
                    if(!ha_avanzato){
                        cerca_porta_segreta(giocatore_corrente);
                        ha_avanzato = 1; // Considera la ricerca come un avanzamento
                        azione_eseguita = 1;
                    } else {
                        printf("Hai gia' avanzato in questo turno!\n");
                    }
                    
                    break;
                default:
                    printf("Comando non valido.\n");
            }
            if(giocatore_corrente->p_vita <= 0 && azione_eseguita){
                printf("%s e' morto!\n", giocatore_corrente->nome_giocatore);
            }
        } while (!gioco_finito && !azione_eseguita);

        // Passa al prossimo giocatore
        if(!gioco_finito){
            turno_giocatore = (turno_giocatore + 1) % num_giocatori;
            int giocatori_vivi = 0;
            for(int i = 0; i < num_giocatori; i++){
                if(giocatori[i]->p_vita > 0){
                    giocatori_vivi++;
                }
            }
            if(giocatori_vivi == 0){
                gioco_finito = 1;
            }
        }
    }

    // Determina il vincitore (se presente)
    int vincitore = -1;
    for(int i = 0; i < num_giocatori; i++){
        if(giocatori[i]->p_vita > 0){
            vincitore = i;
        }
    }
    
    if(vincitore != -1){
        printf("Il vincitore e' %s!\n", giocatori[vincitore]->nome_giocatore);
    
        // Aggiorna i nomi dei vincitori
        for (int i = 2; i > 0; i--) {
            strcpy(nomi_vincitori[i], nomi_vincitori[i - 1]);
        }
        strcpy(nomi_vincitori[0], giocatori[vincitore]->nome_giocatore);
    } else {
        printf("Non ci sono vincitori.\n");
    }
}

// Funzione per far avanzare il giocatore
static void avanza(Giocatore *giocatore) {
    Stanza * stanza_corrente = giocatore->posizione;
    Stanza * prossima_stanza = NULL;
    
    int direzione = scegli_direzione(stanza_corrente);
    switch (direzione){
        case 0:
            prossima_stanza = stanza_corrente->stanza_sopra;
            break;
        case 1:
            prossima_stanza = stanza_corrente->stanza_sotto;
            break;
        case 2:
            prossima_stanza = stanza_corrente->stanza_sinistra;
            break;
        case 3:
            prossima_stanza = stanza_corrente->stanza_destra;
            break;
    }

    
    sposta_giocatore(giocatore, prossima_stanza);

    // Applica l'effetto del tesoro
    if(prossima_stanza->tesoro != NESSUN_TESORO){
        prendi_tesoro(giocatore);
    }
    
    // Applica l'effetto del trabocchetto
    applica_effetto_trabocchetto(giocatore);

    // Probabilità di apparizione di un nemico
    if (rand() % 100 < 25) {
        printf("E' apparso un nemico!\n");
        if(combatti(giocatore)){
            printf("Hai sconfitto Jaffar!\n");
            return;
        }
    }
}

// Funzione per gestire il combattimento
static int combatti(Giocatore *giocatore) {
    // Scegli il tipo di nemico
    int tipo_nemico;
    if(giocatore->posizione->stanza_destra == NULL && giocatore->posizione->stanza_sotto == NULL && giocatore->posizione->stanza_sopra == NULL && giocatore->posizione->stanza_sinistra == NULL){
        tipo_nemico = 2; //Jaffar
    } else {
        tipo_nemico = (rand() % 100 < 60) ? 0 : 1; // 0: scheletro, 1: guardia
    }

    // Statistiche del nemico
    unsigned char nemico_dadi_attacco;
    unsigned char nemico_dadi_difesa;
    unsigned char nemico_punti_vita;

    switch (tipo_nemico) {
        case 0:
            printf("Hai incontrato uno scheletro!\n");
            nemico_dadi_attacco = 1;
            nemico_dadi_difesa = 1;
            nemico_punti_vita = 1;
            break;
        case 1:
            printf("Hai incontrato una guardia!\n");
            nemico_dadi_attacco = 2;
            nemico_dadi_difesa = 2;
            nemico_punti_vita = 2;
            break;
        case 2:
            printf("Hai incontrato Jaffar!\n");
            nemico_dadi_attacco = 3;
            nemico_dadi_difesa = 2;
            nemico_punti_vita = 3;
            break;
    }

    // Ciclo di combattimento
    while (giocatore->p_vita > 0 && nemico_punti_vita > 0) {
        // Determina chi attacca per primo
        int attacco_giocatore = lancia_dado();
        int attacco_nemico = lancia_dado();

        printf("Inizia il giocatore %s (attacco %d) contro nemico (attacco %d)\n", attacco_giocatore >= attacco_nemico ? giocatore->nome_giocatore : "nemico", attacco_giocatore, attacco_nemico);

        if (attacco_giocatore >= attacco_nemico) {
            // Attacca il giocatore
            int danni_inflitti = 0;
            for (int i = 0; i < giocatore->dadi_attacco; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_inflitti++;
                    if (risultato_dado == 6) {
                        danni_inflitti++; // Critico
                    }
                }
            }

            // Difesa del nemico
            int danni_subiti = danni_inflitti;
            for (int i = 0; i < nemico_dadi_difesa; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_subiti--;
                    if (risultato_dado == 6) {
                        danni_subiti--; // Critico
                    }
                }
            }

            // Applica i danni
            if (danni_subiti > 0) {
                nemico_punti_vita -= danni_subiti;
                printf("Il nemico ha subito %d danni!\n", danni_subiti);
            } else {
                printf("Il nemico ha parato tutti i colpi!\n");
            }

            // Controlla se il nemico è sconfitto
            if (nemico_punti_vita <= 0) {
                printf("Hai sconfitto il nemico!\n");
                if(giocatore->p_vita < giocatore->p_vita_max){
                    giocatore->p_vita++;
                    printf("Hai guadagnato 1 punto vita!\n");
                } else {
                    printf("Avevi gia' il massimo dei punti vita.\n");
                }
                return tipo_nemico == 2;
            }

            // Attacca il nemico
            danni_inflitti = 0;
            for (int i = 0; i < nemico_dadi_attacco; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_inflitti++;
                    if (risultato_dado == 6) {
                        danni_inflitti++; // Critico
                    }
                }
            }

            // Difesa del giocatore
            danni_subiti = danni_inflitti;
            for (int i = 0; i < giocatore->dadi_difesa; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_subiti--;
                    if (risultato_dado == 6) {
                        danni_subiti--; // Critico
                    }
                }
            }

            // Applica i danni
            if (danni_subiti > 0) {
                giocatore->p_vita -= danni_subiti;
                printf("Hai subito %d danni!\n", danni_subiti);
            } else {
                printf("Hai parato tutti i colpi!\n");
            }
        } else {
            
            // Attacca il nemico
            int danni_inflitti = 0;
            for (int i = 0; i < nemico_dadi_attacco; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_inflitti++;
                    if (risultato_dado == 6) {
                        danni_inflitti++; // Critico
                    }
                }
            }

            // Difesa del giocatore
            int danni_subiti = danni_inflitti;
            for (int i = 0; i < giocatore->dadi_difesa; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_subiti--;
                    if (risultato_dado == 6) {
                        danni_subiti--; // Critico
                    }
                }
            }

            // Applica i danni
            if (danni_subiti > 0) {
                giocatore->p_vita -= danni_subiti;
                printf("Hai subito %d danni!\n", danni_subiti);
            } else {
                printf("Hai parato tutti i colpi!\n");
            }

            // Controlla se il giocatore è sconfitto
            if (giocatore->p_vita <= 0) {
                printf("Sei stato sconfitto!\n");
                return false;
            }

            // Attacca il giocatore
            danni_inflitti = 0;
            for (int i = 0; i < giocatore->dadi_attacco; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_inflitti++;
                    if (risultato_dado == 6) {
                        danni_inflitti++; // Critico
                    }
                }
            }

            // Difesa del nemico
            danni_subiti = danni_inflitti;
            for (int i = 0; i < nemico_dadi_difesa; i++) {
                int risultato_dado = lancia_dado();
                if (risultato_dado >= 4) {
                    danni_subiti--;
                    if (risultato_dado == 6) {
                        danni_subiti--; // Critico
                    }
                }
            }

            // Applica i danni
            if (danni_subiti > 0) {
                nemico_punti_vita -= danni_subiti;
                printf("Il nemico ha subito %d danni!\n", danni_subiti);
            } else {
                printf("Il nemico ha parato tutti i colpi!\n");
            }

            // Controlla se il nemico è sconfitto
            if (nemico_punti_vita <= 0) {
                printf("Hai sconfitto il nemico!\n");
                if(giocatore->p_vita < giocatore->p_vita_max){
                    giocatore->p_vita++;
                    printf("Hai guadagnato 1 punto vita!\n");
                } else {
                    printf("Avevi gia' il massimo dei punti vita.\n");
                }
                return tipo_nemico == 2; // Ritorna true se il nemico sconfitto è jafar
            }
        }
    }
    return false;
}

// Funzione per la fuga
static void scappa(Giocatore *giocatore) {
    if(giocatore->posizione == primaStanza){
        printf("Non puoi scappare dalla prima stanza!\n");
        return;
    }
    if(giocatore->classe == PRINCIPE){
        if(giocatore->fuggito == 0){
            printf("%s e' tornato alla stanza precedente.\n", giocatore->nome_giocatore);
            Stanza * current = primaStanza;
            Stanza * prev = NULL;
            while(current != giocatore->posizione){
                prev = current;
                if(current->stanza_destra != NULL && current->stanza_destra != giocatore->posizione){
                    current = current->stanza_destra;
                } else if (current->stanza_sotto != NULL && current->stanza_sotto != giocatore->posizione){
                    current = current->stanza_sotto;
                } else if (current->stanza_sopra != NULL && current->stanza_sopra != giocatore->posizione){
                    current = current->stanza_sopra;
                } else {
                    current = current->stanza_sinistra;
                }
            }
            sposta_giocatore(giocatore, prev);
            giocatore->fuggito++;
        } else {
            printf("Un principe puo' scappare solo una volta!\n");
        }
    } else if (giocatore->classe == DOPPLEGANGER){
        if(giocatore->fuggito < 2){
            printf("%s e' tornato alla stanza precedente.\n", giocatore->nome_giocatore);
            Stanza * current = primaStanza;
            Stanza * prev = NULL;
            while(current != giocatore->posizione){
                prev = current;
                if(current->stanza_destra != NULL && current->stanza_destra != giocatore->posizione){
                    current = current->stanza_destra;
                              } else if (current->stanza_sopra != NULL && current->stanza_sopra != giocatore->posizione){
                    current = current->stanza_sopra;
                } else {
                    current = current->stanza_sinistra;
                }
            }
            sposta_giocatore(giocatore, prev);
            giocatore->fuggito++;
        } else {
            printf("Un doppleganger puo' scappare solo due volte!\n");
        }
    }
}

// Funzione per stampare le informazioni del giocatore
static void stampa_giocatore(Giocatore *giocatore) {
    printf("Nome: %s\n", giocatore->nome_giocatore);
    printf("Classe: %s\n", giocatore->classe == PRINCIPE ? "Principe" : "Doppleganger");
    printf("Posizione: Stanza %d\n", conta_stanze());
    printf("Punti vita massimi: %d\n", giocatore->p_vita_max);
    printf("Punti vita attuali: %d\n", giocatore->p_vita);
    printf("Dadi attacco: %d\n", giocatore->dadi_attacco);
    printf("Dadi difesa: %d\n", giocatore->dadi_difesa);
}

// Funzione per stampare le informazioni della stanza corrente
static void stampa_zona(Giocatore *giocatore) {
    Stanza *stanza = giocatore->posizione;
    printf("Tipo stanza: %d\n", stanza->tipo);
    printf("Trabocchetto: %d\n", stanza->trabocchetto);
    printf("Tesoro presente: %s\n", stanza->tesoro != NESSUN_TESORO ? "Si" : "No");
}

// Funzione per prendere il tesoro
static void prendi_tesoro(Giocatore *giocatore) {
    Stanza *stanza = giocatore->posizione;
    if (stanza->tesoro != NESSUN_TESORO) {
        applica_effetto_tesoro(giocatore, stanza->tesoro);
        stanza->tesoro = NESSUN_TESORO; // Rimuovi il tesoro dalla stanza
    } else {
        printf("Non c'e' nessun tesoro qui.\n");
    }
}

// Funzione per cercare una porta segreta
static void cerca_porta_segreta(Giocatore *giocatore) {
    Stanza *stanza_corrente = giocatore->posizione;
    
    // Controlla se c'è una direzione libera in cui cercare
    int direzioni_libere = 0;
    if(stanza_corrente->stanza_sopra == NULL) direzioni_libere++;
    if(stanza_corrente->stanza_sotto == NULL) direzioni_libere++;
    if(stanza_corrente->stanza_sinistra == NULL) direzioni_libere++;
    if(stanza_corrente->stanza_destra == NULL) direzioni_libere++;

    if(direzioni_libere == 0){
        printf("Non ci sono porte segrete in questa stanza.\n");
        return;
    }

    int direzione = scegli_direzione(stanza_corrente);
    stanza_corrente->tentativi_porta[direzione]++;

    // Calcola la probabilità di trovare una porta segreta in base al numero di tentativi
    float probabilita;
    switch (stanza_corrente->tentativi_porta[direzione]) {
        case 1:
            probabilita = 0.33;
            break;
        case 2:
            probabilita = 0.20;
            break;
        case 3:
            probabilita = 0.15;
            break;
        default:
            probabilita = 0.0; // Nessuna probabilità dopo 3 tentativi
    }

    if ((rand() / (float)RAND_MAX) < probabilita) {
        printf("Hai trovato una porta segreta!\n");

        // Crea una nuova stanza
        Stanza *nuova_stanza = crea_stanza_random();

        // Collega la nuova stanza alla stanza corrente
        switch(direzione){
            case 0:
                stanza_corrente->stanza_sopra = nuova_stanza;
                break;
            case 1:
                stanza_corrente->stanza_sotto = nuova_stanza;
                break;
            case 2:
                stanza_corrente->stanza_sinistra = nuova_stanza;
                break;
            case 3:
                stanza_corrente->stanza_destra = nuova_stanza;
                break;
        }

        // Sposta il giocatore nella nuova stanza
        sposta_giocatore(giocatore, nuova_stanza);

        printf("Ora ti trovi nella stanza segreta.\n");

        // Sposta il giocatore nella stanza precedente
        sposta_giocatore(giocatore, stanza_corrente);

        printf("Sei tornato alla stanza precedente.\n");

    } else {
        printf("Non hai trovato nessuna porta segreta.\n");
    }
}

// Funzione per applicare gli effetti dei trabocchetti
static void applica_effetto_trabocchetto(Giocatore *giocatore) {
    Stanza *stanza = giocatore->posizione;
    if(stanza->trabocchetto_attivato == 0){
        if(giocatore->classe == PRINCIPE){
            static int principe_immune = 1;
            if(principe_immune){
                principe_immune = 0;
                printf("Il principe e' immune al primo trabocchetto!\n");
                stanza->trabocchetto_attivato = 1;
                return;
            }
        }
    
    
        switch (stanza->trabocchetto) {
            case NESSUNO:
                printf("Nessun trabocchetto qui.\n");
                break;
            case TEGOLA:
                printf("Sei stato colpito da una tegola!\n");
                giocatore->p_vita--;
                printf("Hai perso 1 punto vita.\n");
                break;
            case LAME:
                printf("Sei stato colpito dalle lame!\n");
                giocatore->p_vita -= 2;
                printf("Hai perso 2 punti vita.\n");
                break;
            case CADUTA:
                printf("Sei caduto in una trappola!\n");
                if (rand() % 2 == 0) {
                    giocatore->p_vita--;
                    printf("Hai perso 1 punto vita.\n");
                } else {
                    giocatore->p_vita -= 2;
                    printf("Hai perso 2 punti vita.\n");
                }
                break;
            case BURRONE:
                printf("Sei caduto in un burrone!\n");
                giocatore->p_vita = 0;
                printf("Sei morto!\n");
                break;
        }
        stanza->trabocchetto_attivato = 1;
    } else {
        printf("Il trabocchetto e' gia' stato attivato!\n");
    }
}

// Funzione per applicare gli effetti dei tesori
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro) {
    switch (tesoro) {
        case NESSUN_TESORO:
            printf("Nessun tesoro qui.\n");
            break;
        case VERDE_VELENO:
            printf("Hai raccolto del veleno verde!\n");
            giocatore->p_vita--;
            printf("Hai perso 1 punto vita.\n");
            break;
        case BLU_GUARIGIONE:
            printf("Hai raccolto una pozione curativa blu!\n");
            if (giocatore->p_vita < giocatore->p_vita_max) {
                giocatore->p_vita++;
                printf("Hai guadagnato 1 punto vita.\n");
            } else {
                printf("Hai gia' il massimo dei punti vita.\n");
            }
            break;
        case ROSSO_AUMENTA_VITA:
            printf("Hai raccolto una pozione che aumenta la vita massima!\n");
            giocatore->p_vita_max++;
            giocatore->p_vita = giocatore->p_vita_max;
            printf("I tuoi punti vita massimi sono aumentati di 1.\n");
            printf("I tuoi punti vita sono stati ripristinati al massimo.\n");
            break;
        case SPADA_TAGLIENTE:
            printf("Hai raccolto una spada tagliente!\n");
            giocatore->dadi_attacco++;
            printf("I tuoi dadi attacco sono aumentati di 1.\n");
            break;
        case SCUDO:
            printf("Hai raccolto uno scudo!\n");
            giocatore->dadi_difesa++;
            printf("I tuoi dadi difesa sono aumentati di 1.\n");
            break;
    }
}

// Funzione per liberare la memoria allocata per la mappa
static void libera_mappa() {
    if (primaStanza == NULL) return;

    Stanza *current = primaStanza;
    Stanza *next = NULL;
    
    while(current != NULL){
        next = NULL;
        if(current->stanza_destra != NULL){
            next = current->stanza_destra;
            current->stanza_destra = NULL;
        } else if (current->stanza_sotto != NULL){
            next = current->stanza_sotto;
            current->stanza_sotto = NULL;
        } else if (current->stanza_sinistra != NULL){
            next = current->stanza_sinistra;
            current->stanza_sinistra = NULL;
        } else if (current->stanza_sopra != NULL){
            next = current->stanza_sopra;
            current->stanza_sopra = NULL;
        }
        
        
        free(current);
        current = next;
    }
    primaStanza = NULL;
    mappa_creata = 0;
}

// Funzione per lanciare un dado a 6 facce
static int lancia_dado() {
    return (rand() % 6) + 1;
}

// Funzione per inizializzare i giocatori
static void inizializza_giocatori(){
    // Chiedi il numero di giocatori
    do {
        printf("Inserisci il numero di giocatori (1-3): ");
        if(scanf("%d", &num_giocatori) != 1){
            printf("Errore: inserisci un numero valido.\n");
            while (getchar() != '\n');
            continue;
        }
    } while (num_giocatori < 1 || num_giocatori > MAX_PLAYERS);

    // Inizializza i giocatori
    int principe_presente = 0;
    for (int i = 0; i < num_giocatori; i++) {
        giocatori[i] = (Giocatore *)malloc(sizeof(Giocatore));
        if (giocatori[i] == NULL) {
            printf("Errore: impossibile allocare memoria per il giocatore.\n");
            return;
        }

        printf("Inserisci il nome del giocatore %d: ", i + 1);
        scanf(" %[^\n]", giocatori[i]->nome_giocatore);

        // Scegli la classe del giocatore
        
        if (!principe_presente) {
            printf("Scegli la classe del giocatore %d (0 - PRINCIPE, 1 - DOPPLEGANGER): ", i + 1);
            int temp;
            if(scanf("%d", &temp) != 1 || (temp != 0 && temp != 1)){
                printf("Errore: inserisci un numero valido (0-1).\n");
                while (getchar() != '\n');
                free(giocatori[i]);
                i--;
                continue;
            }
            giocatori[i]->classe = (Tipo_giocatore)temp;
            if (giocatori[i]->classe == PRINCIPE) {
                principe_presente = 1;
            }
        } else {
            printf("Il giocatore %d sara' un doppleganger.\n", i + 1);
            giocatori[i]->classe = DOPPLEGANGER;
        }

        // Inizializza le statistiche del giocatore
        giocatori[i]->p_vita_max = 5;
        giocatori[i]->p_vita = giocatori[i]->p_vita_max;
        giocatori[i]->dadi_attacco = 2;
        giocatori[i]->dadi_difesa = 2;
        giocatori[i]->posizione = NULL;
        giocatori[i]->fuggito = 0;
    }
}

// Funzione per spostare un giocatore in una nuova stanza
static void sposta_giocatore(Giocatore * giocatore, Stanza * nuova_stanza){
    giocatore->posizione = nuova_stanza;
}

// Funzione per stampare il menu di imposta gioco
static void stampa_menu_imposta_gioco(){
    printf("Imposta Gioco:\n");
    printf("1 - Inserisci stanza\n");
    printf("2 - Cancella stanza\n");
    printf("3 - Stampa stanze\n");
    printf("4 - Genera mappa casuale\n");
    printf("5 - Chiudi mappa\n");
}
// Funzione per terminare il gioco
void termina_gioco() {
    int giocatori_vivi = 0;
    for(int i = 0; i < num_giocatori; i++){
        if(giocatori[i]->p_vita > 0){
            giocatori_vivi++;
        }
    }
    if(giocatori_vivi != 0){
        printf("Non puoi terminare il gioco se c'e' ancora qualche giocatore vivo!\n");
        return;
    }
    printf("Grazie per aver giocato!\n");
    libera_mappa();
    for(int i = 0; i < num_giocatori; i++){
        free(giocatori[i]);
    }
    exit(0);
}

// Funzione per visualizzare i crediti
void crediti() {
    printf("Prince of Inertia - Sviluppato da ChatGPT\n");
    printf("Vincitori delle ultime tre partite:\n");
    for (int i = 0; i < 3; i++) {
        if(strcmp(nomi_vincitori[i], "") == 0){
            printf("%d: Nessun vincitore\n", i + 1);
        } else {
            printf("%d: %s\n", i + 1, nomi_vincitori[i]);
        }
    }
}

// Funzione per scegliere una direzione
static int scegli_direzione(Stanza *stanza){
    int scelta;
    do{
        printf("Scegli una direzione:\n");
        if(stanza->stanza_sopra != NULL){
            printf("0 - Sopra\n");
        }
        if(stanza->stanza_sotto != NULL){
            printf("1 - Sotto\n");
        }
        if(stanza->stanza_sinistra != NULL){
            printf("2 - Sinistra\n");
        }
        if(stanza->stanza_destra != NULL){
            printf("3 - Destra\n");
        }
        if(scanf("%d", &scelta) != 1){
            printf("Errore: inserisci un numero valido.\n");
            while (getchar() != '\n');
            continue;
        }
    } while ((scelta == 0 && stanza->stanza_sopra == NULL) || (scelta == 1 && stanza->stanza_sotto == NULL) || (scelta == 2 && stanza->stanza_sinistra == NULL) || (scelta == 3 && stanza->stanza_destra == NULL));
    return scelta;
}

// Funzione per creare una stanza con valori casuali
static Stanza *crea_stanza_random(){
    Stanza *nuova_stanza = (Stanza *)malloc(sizeof(Stanza));
    if (nuova_stanza == NULL) {
        printf("Errore: impossibile allocare memoria per la nuova stanza.\n");
        return NULL;
    }

    // Generazione casuale dei valori della stanza
    nuova_stanza->tipo = rand() % 10;

    int random_trabocchetto = rand() % 100;
    if(random_trabocchetto <= 65){
        nuova_stanza->trabocchetto = NESSUNO;
    } else if (random_trabocchetto > 65 && random_trabocchetto <= 75){
        nuova_stanza->trabocchetto = TEGOLA;
    } else if (random_trabocchetto > 75 && random_trabocchetto <= 84){
        nuova_stanza->trabocchetto = LAME;
    } else if (random_trabocchetto > 84 && random_trabocchetto <= 92){
        nuova_stanza->trabocchetto = CADUTA;
    } else {
        nuova_stanza->trabocchetto = BURRONE;
    }

    int random_tesoro = rand() % 100;
    if(random_tesoro <= 20){
        nuova_stanza->tesoro = NESSUN_TESORO;
    } else if (random_tesoro > 20 && random_tesoro <= 40){
        nuova_stanza->tesoro = VERDE_VELENO;
    } else if (random_tesoro > 40 && random_tesoro <= 60){
        nuova_stanza->tesoro = BLU_GUARIGIONE;
    } else if (random_tesoro > 60 && random_tesoro <= 75){
        nuova_stanza->tesoro = ROSSO_AUMENTA_VITA;
    } else if (random_tesoro > 75 && random_tesoro <= 90){
        nuova_stanza->tesoro = SPADA_TAGLIENTE;
    } else {
        nuova_stanza->tesoro = SCUDO;
    }

    nuova_stanza->stanza_destra = NULL;
    nuova_stanza->stanza_sinistra = NULL;
    nuova_stanza->stanza_sopra = NULL;
    nuova_stanza->stanza_sotto = NULL;

    //Inizializza a 0 il flag trabocchetto_attivato e tentativi_porta
    nuova_stanza->trabocchetto_attivato = 0;
    for(int i = 0; i < 4; i++){
        nuova_stanza->tentativi_porta[i] = 0;
    }

    return nuova_stanza;
}

//Funzione per collegare due stanze
static void collega_stanza(Stanza *stanza_precedente, Stanza *nuova_stanza){
    int tentativi = 0;
    while(tentativi < 100){
                
        int direzione = rand() % 4;
        if(direzione == 0 && stanza_precedente->stanza_sopra == NULL){
            stanza_precedente->stanza_sopra = nuova_stanza;
            break;
        } else if (direzione == 1 && stanza_precedente->stanza_sotto == NULL){
            stanza_precedente->stanza_sotto = nuova_stanza;
            break;
        } else if (direzione == 2 && stanza_precedente->stanza_sinistra == NULL){
            stanza_precedente->stanza_sinistra = nuova_stanza;
            break;
        } else if (direzione == 3 && stanza_precedente->stanza_destra == NULL){
            stanza_precedente->stanza_destra = nuova_stanza;
            break;
        }
        tentativi++;
    }
}

//Funzione per contare le stanze
static int conta_stanze(){
    Stanza * current = primaStanza;
    int count = 0;
    while(current != NULL){
        count++;
        if(current->stanza_destra != NULL){
            current = current->stanza_destra;
        } else if (current->stanza_sotto != NULL){
            current = current->stanza_sotto;
        } else if (current->stanza_sinistra != NULL){
            current = current->stanza_sinistra;
        } else if (current->stanza_sopra != NULL){
            current = current->stanza_sopra;
        } else {
            current = NULL;
        }
    }
    return count;
}

//Funzione per mescolare un array
static void mescola_array(int arr[], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}
