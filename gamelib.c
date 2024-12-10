#include "gamelib.h"

// Variabili globali
static Stanza *pFirst = NULL; // Puntatore alla prima stanza della mappa
static Giocatore *giocatori[MAX_PLAYERS]; // Array di puntatori ai giocatori
static int mappa_creata = 0; // Flag per indicare se la mappa è stata creata
static char nomi_vincitori[3][MAX_NAME_LENGTH] = {"", "", ""}; // Nomi degli ultimi tre vincitori
static int turno_giocatore = 0; // Indice del giocatore corrente
static int num_giocatori = 0;

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
static void passa();
static void applica_effetto_trabocchetto(Giocatore *giocatore);
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro);
static void libera_mappa();
static int lancia_dado();
static void inizializza_giocatori();
static void sposta_giocatore(Giocatore * giocatore, Stanza * nuova_stanza);
static void stampa_menu_imposta_gioco();
static void scegli_direzione(Stanza ** direzione);

// Funzione per impostare il gioco
void imposta_gioco() {
    if(mappa_creata){
        libera_mappa();
    }

    
    inizializza_giocatori();

    int scelta;
    do {
        stampa_menu_imposta_gioco();
        scanf("%d", &scelta);

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
                if(pFirst != NULL){
                    int count = 0;
                    Stanza * current = pFirst;
                    while(current != NULL){
                        count++;
                        if(current->stanza_destra != NULL){
                            current = current->stanza_destra;
                        } else if (current->stanza_sotto != NULL){
                            current = current->stanza_sotto;
                        } else if (current->stanza_sopra != NULL){
                            current = current->stanza_sopra;
                        } else if (current->stanza_sinistra != NULL){
                            current = current->stanza_sinistra;
                        } else {
                            break;
                        }
                    }
                    if(count < MAX_ROOMS){
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

    // Inserimento valori della stanza da tastiera
    printf("Inserisci il tipo della stanza:\n");
    printf("0 - corridoio\n1 - scala\n2 - sala_banchetto\n3 - magazzino\n");
    printf("4 - posto_guardia\n5 - prigione\n6 - armeria\n7 - moschea\n");
    printf("8 - torre\n9 - bagni\n");
    scanf("%u", (unsigned int*)&nuova_stanza->tipo);

    printf("Inserisci il tipo di trabocchetto:\n");
    printf("0 - nessuno\n1 - tegola\n2 - lame\n3 - caduta\n4 - burrone\n");
    scanf("%u", (unsigned int*)&nuova_stanza->trabocchetto);

    printf("Inserisci il tipo di tesoro:\n");
    printf("0 - nessun_tesoro\n1 - verde_veleno\n2 - blu_guarigione\n");
    printf("3 - rosso_aumenta_vita\n4 - spada_tagliente\n5 - scudo\n");
    scanf("%u", (unsigned int*)&nuova_stanza->tesoro);

    nuova_stanza->stanza_destra = NULL;
    nuova_stanza->stanza_sinistra = NULL;
    nuova_stanza->stanza_sopra = NULL;
    nuova_stanza->stanza_sotto = NULL;

    // Inserimento in coda
    if (pFirst == NULL) {
        pFirst = nuova_stanza;
    } else {
        Stanza *current = pFirst;
        while (current->stanza_destra != NULL) {
            current = current->stanza_destra;
        }
        printf("Scegli dove inserire la stanza:\n");
        printf("1 - sopra\n2 - sotto\n3 - sinistra\n4 - destra\n");
        int direzione;
        scanf("%d", &direzione);

        switch(direzione){
            case 1:
                if(current->stanza_sopra == NULL){
                    current->stanza_sopra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 2:
                if(current->stanza_sotto == NULL){
                    current->stanza_sotto = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 3:
                if(current->stanza_sinistra == NULL){
                    current->stanza_sinistra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            case 4:
                if(current->stanza_destra == NULL){
                    current->stanza_destra = nuova_stanza;
                } else {
                    printf("Direzione occupata\n");
                    free(nuova_stanza);
                }
            break;
            default:
            printf("Direzione non valida, la stanza non verra' aggiunta\n");
            free(nuova_stanza);
            break;
        }
    }
}

// Funzione per cancellare l'ultima stanza inserita
static void canc_stanza() {
    if (pFirst == NULL) {
        printf("Nessuna stanza da cancellare.\n");
        return;
    }

    if (pFirst->stanza_destra == NULL && pFirst->stanza_sotto == NULL && pFirst->stanza_sopra == NULL && pFirst->stanza_sinistra == NULL) {
        free(pFirst);
        pFirst = NULL;
        return;
    }

    Stanza *current = pFirst;
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
    } else {
        prev->stanza_sinistra = NULL;
    }

    free(current);
}

// Funzione per stampare le stanze
static void stampa_stanze() {
    if (pFirst == NULL) {
        printf("Nessuna stanza creata.\n");
        return;
    }

    Stanza *current = pFirst;
    int i = 0;
    while (current != NULL) {
        printf("Stanza %d:\n", i + 1);
        printf("Tipo: %u\n", current->tipo);
        printf("Trabocchetto: %u\n", current->trabocchetto);
        printf("Tesoro: %u\n", current->tesoro);
        printf("Stanza destra: %p\n", (void *)current->stanza_destra);
        printf("Stanza sinistra: %p\n", (void *)current->stanza_sinistra);
        printf("Stanza sopra: %p\n", (void *)current->stanza_sopra);
        printf("Stanza sotto: %p\n", (void *)current->stanza_sotto);
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
        Stanza *nuova_stanza = (Stanza *)malloc(sizeof(Stanza));
        if (nuova_stanza == NULL) {
            printf("Errore: impossibile allocare memoria per la nuova stanza.\n");
            return;
        }

        // Generazione casuale dei valori della stanza
        nuova_stanza->tipo = rand() % 10;

        int random_trabocchetto = rand() % 100;
        if(random_trabocchetto <= 65){
            nuova_stanza->trabocchetto = nessuno;
        } else if (random_trabocchetto > 65 && random_trabocchetto <= 75){
            nuova_stanza->trabocchetto = tegola;
        } else if (random_trabocchetto > 75 && random_trabocchetto <= 84){
            nuova_stanza->trabocchetto = lame;
        } else if (random_trabocchetto > 84 && random_trabocchetto <= 92){
            nuova_stanza->trabocchetto = caduta;
        } else {
            nuova_stanza->trabocchetto = burrone;
        }

        int random_tesoro = rand() % 100;
        if(random_tesoro <= 20){
            nuova_stanza->tesoro = nessun_tesoro;
        } else if (random_tesoro > 20 && random_tesoro <= 40){
            nuova_stanza->tesoro = verde_veleno;
        } else if (random_tesoro > 40 && random_tesoro <= 60){
            nuova_stanza->tesoro = blu_guarigione;
        } else if (random_tesoro > 60 && random_tesoro <= 75){
            nuova_stanza->tesoro = rosso_aumenta_vita;
        } else if (random_tesoro > 75 && random_tesoro <= 90){
            nuova_stanza->tesoro = spada_tagliente;
        } else {
            nuova_stanza->tesoro = scudo;
        }

        nuova_stanza->stanza_destra = NULL;
        nuova_stanza->stanza_sinistra = NULL;
        nuova_stanza->stanza_sopra = NULL;
        nuova_stanza->stanza_sotto = NULL;

        // Inserimento in coda
        if (pFirst == NULL) {
            pFirst = nuova_stanza;
        } else {
            Stanza *current = pFirst;
            Stanza *previous = NULL;

            // Trova l'ultima stanza
            while (current != NULL) {
                previous = current;
                current = current->stanza_destra;
            }
            
             //Adesso current e' null, previous e' l'ultima stanza
            //Si prova ad attaccare la nuova stanza in una direzione a caso
            int tentativi = 0;
            int direzione_libera = 0; // Flag per indicare se almeno una direzione è libera
            while(tentativi < 100){
                
                int direzione = rand() % 4;
                if(direzione == 0 && previous->stanza_sopra == NULL){
                    previous->stanza_sopra = nuova_stanza;
                    direzione_libera = 1;
                    break;
                } else if (direzione == 1 && previous->stanza_sotto == NULL){
                    previous->stanza_sotto = nuova_stanza;
                    direzione_libera = 1;
                    break;
                } else if (direzione == 2 && previous->stanza_sinistra == NULL){
                    previous->stanza_sinistra = nuova_stanza;
                    direzione_libera = 1;
                    break;
                } else if (direzione == 3 && previous->stanza_destra == NULL){
                    previous->stanza_destra = nuova_stanza;
                    direzione_libera = 1;
                    break;
                }
                tentativi++;
            }

                //Se una direzione è libera, collega la nuova stanza e passa alla prossima iterazione
                if(previous->stanza_sopra == nuova_stanza || previous->stanza_sotto == nuova_stanza || previous->stanza_sinistra == nuova_stanza || previous->stanza_destra == nuova_stanza){
                    break;
                }
            }
            
             //Se non si riesce a collegare la stanza in nessun modo la si elimina
            if(direzione_libera == 0){
                free(nuova_stanza);
                printf("Impossibile collegare la stanza, verra eliminata.\n");
                break;
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
    if (!mappa_creata || pFirst == NULL) { 
        printf("Errore: la mappa non è ancora stata creata o è vuota.\n");
        return;
    }


    // Resetta i punti vita dei giocatori
    for(int i = 0; i < num_giocatori; i++){
        giocatori[i]->p_vita = giocatori[i]->p_vita_max;
    }

    // Posiziona i giocatori nella prima stanza
    for (int i = 0; i < num_giocatori; i++) {
        giocatori[i]->posizione = pFirst;
    }

    // Ciclo di gioco
    int gioco_finito = 0;
    while (!gioco_finito) {
        Giocatore *giocatore_corrente = giocatori[turno_giocatore];
        
        printf("Turno di %s\n", giocatore_corrente->nome_giocatore);

        int scelta;
        int ha_avanzato = 0;
        do {
            printf("Scegli un'azione:\n");
            printf("1 - Avanza\n");
            printf("2 - Combatti\n");
            printf("3 - Scappa\n");
            printf("4 - Stampa giocatore\n");
            printf("5 - Stampa zona\n");
            printf("6 - Prendi tesoro\n");
            printf("7 - Cerca porta segreta\n");
            printf("8 - Passa\n");
            scanf("%d", &scelta);

            switch (scelta) {
                case 1:
                    if(!ha_avanzato){
                        avanza(giocatore_corrente);
                        ha_avanzato = 1;
                    } else {
                        printf("Hai gia' avanzato in questo turno!\n");
                    }
                    break;
                case 2:
                   if(combatti(giocatore_corrente)){
                        gioco_finito = 1;
                   }
                    break;
                case 3:
                    scappa(giocatore_corrente);
                    break;
                case 4:
                    stampa_giocatore(giocatore_corrente);
                    break;
                case 5:
                    stampa_zona(giocatore_corrente);
                    break;
                case 6:
                    prendi_tesoro(giocatore_corrente);
                    break;
                case 7:
                    if(!ha_avanzato){
                        cerca_porta_segreta(giocatore_corrente);
                        ha_avanzato = 1; // Considera la ricerca come un avanzamento
                    } else {
                        printf("Hai gia' avanzato in questo turno!\n");
                    }
                    
                    break;
                case 8:
                    passa();
                    break;
                default:
                    printf("Comando non valido.\n");
            }
        } while (scelta != 8 && !gioco_finito);
        if(giocatore_corrente->p_vita <= 0){
            printf("%s e' morto!\n", giocatore_corrente->nome_giocatore);
            gioco_finito = 1;
            for(int i = 0; i < num_giocatori; i++){
                if(giocatori[i]->p_vita > 0){
                    gioco_finito = 0;
                }
            }
        }
        // Passa al prossimo giocatore
        if(!gioco_finito){
            int giocatori_rimasti[MAX_PLAYERS];
            int count = 0;
            for(int i = 0; i < num_giocatori; i++){
                if(giocatori[i]->p_vita > 0){
                    giocatori_rimasti[count] = i;
                    count++;
                }
            }

            if(count > 0){
                turno_giocatore = giocatori_rimasti[rand() % count];
            } else {
                gioco_finito = 1; // Tutti i giocatori sono morti
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
    
    printf("Puoi avanzare in queste direzioni: \n");
    if(stanza_corrente->stanza_sopra != NULL){
        printf("Sopra\n");
    }
    if(stanza_corrente->stanza_sotto != NULL){
        printf("Sotto\n");
    }
    if(stanza_corrente->stanza_sinistra != NULL){
        printf("Sinistra\n");
    }
    if(stanza_corrente->stanza_destra != NULL){
        printf("Destra\n");
    }

    
    int direzione;
    do{
        printf("Scegli una direzione (1-Sopra, 2-Sotto, 3-Sinistra, 4-Destra): ");
        scanf("%d", &direzione);
        switch (direzione){
            case 1:
                prossima_stanza = stanza_corrente->stanza_sopra;
                break;
            case 2:
                prossima_stanza = stanza_corrente->stanza_sotto;
                break;
            case 3:
                prossima_stanza = stanza_corrente->stanza_sinistra;
                break;
            case 4:
                prossima_stanza = stanza_corrente->stanza_destra;
                break;
            default:
                printf("Direzione non valida.\n");
        }
    } while (prossima_stanza == NULL);

    
    sposta_giocatore(giocatore, prossima_stanza);
    
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
                }
                return tipo_nemico == 2; // Ritorna true se il nemico sconfitto è jafar
            }
        }
    }
    return false;
}

// Funzione per la fuga
static void scappa(Giocatore *giocatore) {
    if(giocatore->classe == principe){
        if(giocatore->posizione != pFirst){
            printf("%s e' tornato alla stanza precedente.\n", giocatore->nome_giocatore);
            Stanza * current = pFirst;
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
            giocatore->posizione = prev;
        } else {
            printf("Non puoi scappare dalla prima stanza!\n");
        }
    } else {
        if(giocatore->posizione != pFirst){
            printf("%s e' tornato alla stanza precedente.\n", giocatore->nome_giocatore);
            Stanza * current = pFirst;
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
            giocatore->posizione = prev;
        } else {
            printf("Non puoi scappare dalla prima stanza!\n");
        }
    }
}

// Funzione per stampare le informazioni del giocatore
static void stampa_giocatore(Giocatore *giocatore) {
    printf("Nome: %s\n", giocatore->nome_giocatore);
    printf("Classe: %d\n", giocatore->classe);
    printf("Posizione: %p\n", (void *)giocatore->posizione);
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
    printf("Tesoro presente: %s\n", stanza->tesoro != nessun_tesoro ? "Si" : "No");
}

// Funzione per prendere il tesoro
static void prendi_tesoro(Giocatore *giocatore) {
    Stanza *stanza = giocatore->posizione;
    if (stanza->tesoro != nessun_tesoro) {
        applica_effetto_tesoro(giocatore, stanza->tesoro);
        stanza->tesoro = nessun_tesoro; // Rimuovi il tesoro dalla stanza
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

    if (rand() % 3 == 0) { // 33% di probabilità di trovare una porta segreta
        printf("Hai trovato una porta segreta!\n");

        // Crea una nuova stanza
        Stanza *nuova_stanza = (Stanza *)malloc(sizeof(Stanza));
        if (nuova_stanza == NULL) {
            printf("Errore: impossibile allocare memoria per la nuova stanza.\n");
            return;
        }

        // Generazione casuale dei valori della stanza
        nuova_stanza->tipo = rand() % 10;
        nuova_stanza->trabocchetto = nessuno;
        nuova_stanza->tesoro = nessun_tesoro;
        nuova_stanza->stanza_destra = NULL;
        nuova_stanza->stanza_sinistra = NULL;
        nuova_stanza->stanza_sopra = NULL;
        nuova_stanza->stanza_sotto = NULL;

        // Collega la nuova stanza in una direzione casuale
        int direzione;
        do {
            direzione = rand() % 4; // 0: sopra, 1: sotto, 2: sinistra, 3: destra
        } while ((direzione == 0 && stanza_corrente->stanza_sopra != NULL) ||
                 (direzione == 1 && stanza_corrente->stanza_sotto != NULL) ||
                 (direzione == 2 && stanza_corrente->stanza_sinistra != NULL) ||
                 (direzione == 3 && stanza_corrente->stanza_destra != NULL));

        switch (direzione) {
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
        
        sposta_giocatore(giocatore, stanza_corrente);

        printf("Sei tornato alla stanza precedente.\n");
    } else {
        printf("Non hai trovato nessuna porta segreta.\n");
    }
}

// Funzione per passare il turno
static void passa() {
    printf("Turno passato.\n");
}

// Funzione per applicare gli effetti dei trabocchetti
static void applica_effetto_trabocchetto(Giocatore *giocatore) {
    if(giocatore->classe == principe){
        static int principe_immune = 1;
        if(principe_immune){
            principe_immune = 0;
            printf("Il principe e' immune al primo trabocchetto!\n");
            return;
        }
    }
    
    
    Stanza *stanza = giocatore->posizione;
    switch (stanza->trabocchetto) {
        case nessuno:
            printf("Nessun trabocchetto qui.\n");
            break;
        case tegola:
            printf("Sei stato colpito da una tegola!\n");
            giocatore->p_vita--;
            printf("Hai perso 1 punto vita.\n");
            break;
        case lame:
            printf("Sei stato colpito dalle lame!\n");
            giocatore->p_vita -= 2;
            printf("Hai perso 2 punti vita.\n");
            break;
        case caduta:
            printf("Sei caduto in una trappola!\n");
            if (rand() % 2 == 0) {
                giocatore->p_vita--;
                printf("Hai perso 1 punto vita.\n");
            } else {
                giocatore->p_vita -= 2;
                printf("Hai perso 2 punti vita.\n");
            }
            break;
        case burrone:
            printf("Sei caduto in un burrone!\n");
            giocatore->p_vita = 0;
            printf("Sei morto!\n");
            break;
    }
}

// Funzione per applicare gli effetti dei tesori
static void applica_effetto_tesoro(Giocatore *giocatore, Tipo_tesoro tesoro) {
    switch (tesoro) {
        case nessun_tesoro:
            printf("Nessun tesoro qui.\n");
            break;
        case verde_veleno:
            printf("Hai raccolto del veleno verde!\n");
            giocatore->p_vita--;
            printf("Hai perso 1 punto vita.\n");
            break;
        case blu_guarigione:
            printf("Hai raccolto una pozione curativa blu!\n");
            if (giocatore->p_vita < giocatore->p_vita_max) {
                giocatore->p_vita++;
                printf("Hai guadagnato 1 punto vita.\n");
            } else {
                printf("Hai gia' il massimo dei punti vita.\n");
            }
            break;
        case rosso_aumenta_vita:
            printf("Hai raccolto una pozione che aumenta la vita massima!\n");
            giocatore->p_vita_max++;
            giocatore->p_vita = giocatore->p_vita_max;
            printf("I tuoi punti vita massimi sono aumentati di 1.\n");
            printf("I tuoi punti vita sono stati ripristinati al massimo.\n");
            break;
        case spada_tagliente:
            printf("Hai raccolto una spada tagliente!\n");
            giocatore->dadi_attacco++;
            printf("I tuoi dadi attacco sono aumentati di 1.\n");
            break;
        case scudo:
            printf("Hai raccolto uno scudo!\n");
            giocatore->dadi_difesa++;
            printf("I tuoi dadi difesa sono aumentati di 1.\n");
            break;
    }
}

// Funzione per liberare la memoria allocata per la mappa
static void libera_mappa() {
    if (pFirst == NULL) return;

    Stanza *current = pFirst;
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
    pFirst = NULL;
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
        scanf("%d", &num_giocatori);
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
            printf("Scegli la classe del giocatore %d (0 - principe, 1 - doppleganger): ", i + 1);
            scanf("%d", (int *)&giocatori[i]->classe);
            if (giocatori[i]->classe == principe) {
                principe_presente = 1;
            }
        } else {
            giocatori[i]->classe = doppleganger;
        }

        // Inizializza le statistiche del giocatore
        giocatori[i]->p_vita_max = 5;
        giocatori[i]->p_vita = giocatori[i]->p_vita_max;
        giocatori[i]->dadi_attacco = 2;
        giocatori[i]->dadi_difesa = 2;
        giocatori[i]->posizione = NULL;
    }

    //Se principe non e' presente, fallo inserire
    if(!principe_presente){
        printf("Deve essere presente almeno un principe per poter giocare.\n");
        free(giocatori[num_giocatori - 1]);
        giocatori[num_giocatori - 1] = (Giocatore *)malloc(sizeof(Giocatore));
        printf("Inserisci il nome del giocatore %d: ", num_giocatori);
        scanf(" %[^\n]", giocatori[num_giocatori - 1]->nome_giocatore);
        giocatori[num_giocatori - 1]->classe = principe;
        giocatori[num_giocatori - 1]->p_vita_max = 5;
        giocatori[num_giocatori - 1]->p_vita = giocatori[num_giocatori - 1]->p_vita_max;
        giocatori[num_giocatori - 1]->dadi_attacco = 2;
        giocatori[num_giocatori - 1]->dadi_difesa = 2;
        giocatori[num_giocatori - 1]->posizione = NULL;
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
        printf("%d: %s\n", i + 1, nomi_vincitori[i]);
    }
}
