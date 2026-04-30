#include <stdio.h>        
#include <stdlib.h>       
#include <string.h>       
#include "../include/prestiti.h"  


Utente* cercaUtente(ElencoUtenti *elenco, int id){
    for(int i=0; i<elenco->num; i++)         
        if(elenco->utenti[i].id == id){      
            return &elenco->utenti[i];       // restituisce il puntatore all'utente trovato
        }
    return NULL;                             
}


Libro* cercaLibro(CatalogoLibri *catalogo, int id){
    for(int i=0; i<catalogo->num; i++)       
        if(catalogo->libri[i].id == id){     
            return &catalogo->libri[i];      // restituisce il puntatore al libro trovato
        }
    return NULL;                             
}


void stampData(time_t t){
    struct tm *info = localtime(&t);         // converte il timestamp in struttura con giorno/mese/anno
    printf("%02d/%02d/%04d",                 // formato con zeri iniziali: es. 05/03/2025
        info->tm_mday,                       // giorno del mese (1-31)
        info->tm_mon + 1,                    // mese: tm_mon va da 0 a 11, quindi si aggiunge 1
        info->tm_year + 1900);               // anno: tm_year conta da 1900, quindi si aggiunge 1900
}


void aggiungiNodoPrestito(Utente *u, int id_libro, char *titolo){
    NodoPrestito *nuovo = (NodoPrestito *)malloc(sizeof(NodoPrestito));  // alloca memoria per il nuovo nodo
    if(nuovo==NULL){                                   
        printf("Errore di allocazione memoria.\n");    
        return;                                        
    }

    nuovo->id_libro = id_libro;                        // salva l'id del libro nel nodo
    strncpy(nuovo->titolo_libro, titolo, 99);          // copia al massimo 99 caratteri del titolo
    nuovo->titolo_libro[99] = '\0';                    // aggiunge manualmente il terminatore di stringa nella 99esima posizione

    nuovo->data_prestito = time(NULL);                 // registra la data attuale come data di prestito
    nuovo->data_scadenza = nuovo->data_prestito        // calcola la scadenza
                           +(30 * 24 * 60 * 60);       // aggiunge 30 giorni espressi in secondi (30g × 24h × 60m × 60s)
    nuovo->restituito = 0;                             // flag: 0=libro non ancora restituito
    nuovo->next = u->prestiti;                         // il nuovo nodo punta al vecchio primo nodo (inserimento in testa)
    u->prestiti = nuovo;                               // il puntatore alla lista ora punta al nuovo nodo
    u->num_prestiti++;                                 // incrementa il contatore dei prestiti attivi dell'utente
}


void nuovoPrestito(CatalogoLibri *catalogo, ElencoUtenti *elenco){
    int id_utente, id_libro;                           

    // stampa l'intestazione della tabella dei libri
    printf("\n--- Lista Libri ---\n");
    printf("%-5s %-30s %-20s %-15s %-10s\n",          // formattazione colonne con larghezza fissa
        "ID", "Titolo", "Autore", "Genere", "Disponibili");
    printf("-------------------------------------------------------------------------------------\n");

    for (int i = 0; i < catalogo->num; i++) {          
        printf("%-5d %-30s %-20s %-15s %-10d\n",       // stampa ogni libro allineato nelle colonne
            catalogo->libri[i].id,
            catalogo->libri[i].titolo,
            catalogo->libri[i].autore,
            catalogo->libri[i].genere,
            catalogo->libri[i].copie_disponibili);
    }

    printf("\nInserisci il tuo ID utente: ");
    scanf("%d", &id_utente);                           // legge l'id utente da tastiera

    Utente *utente_scelto = cercaUtente(elenco, id_utente);  // cerca l'utente nell'elenco
    if (!utente_scelto) {                              // se non trovato
        printf("Utente non trovato!\n");
        return;                                        
    }

    printf("Inserisci l'ID del libro che vuoi prendere in prestito: ");
    scanf("%d", &id_libro);                            // legge l'id libro da tastiera

    Libro *libro_scelto = cercaLibro(catalogo, id_libro);    // cerca il libro nel catalogo
    if(!libro_scelto){                                 // se non trovato
        printf("Libro non trovato.\n");
        return;                                        
    }

    if(libro_scelto->copie_disponibili<=0) {           // controlla se esistono copie disponibili
        printf("Non ci sono copie disponibili per \"%s\".\n", libro_scelto->titolo);
        return;                                        
    }

    // scorre la lista dei prestiti dell'utente per vedere se ha già quel libro
    for(NodoPrestito *tmp = utente_scelto->prestiti; tmp; tmp = tmp->next){
        if(tmp->id_libro == id_libro && tmp->restituito == 0){  // stesso libro e non ancora restituito
            printf("Hai gia' questo libro in prestito.\n");
            return;                                    
        }
    }

    aggiungiNodoPrestito(utente_scelto, libro_scelto->id, libro_scelto->titolo);
    libro_scelto->copie_disponibili--;                 // decrementa le copie disponibili nel catalogo

    // stampa il riepilogo del prestito appena creato
    printf("\nPrestito effettuato con successo!\n");
    printf("Libro: %s\n", libro_scelto->titolo);
    printf("Data prestito: ");
    stampData(utente_scelto->prestiti->data_prestito); // data odierna (nodo in testa = appena inserito)
    printf("\nData scadenza: ");
    stampData(utente_scelto->prestiti->data_scadenza); // data odierna + 30 giorni
    printf("\n");
}

void restituzioneLibro(CatalogoLibri *catalogo, ElencoUtenti *elenco){
    int id_utente, id_libro;

    printf("\n---Restituzione Libro---\n");
    printf("Inserisci il tuo ID utente: ");
    scanf("%d", &id_utente);                           

    Utente *utente_scelto = cercaUtente(elenco, id_utente);  // cerca l'utente
    if (!utente_scelto) {                              
        printf("Utente non trovato!\n");
        return;
    }

    printf("\nPrestiti attivi di %s %s:\n",
        utente_scelto->nome, utente_scelto->cognome);  // nome e cognome dell'utente

    NodoPrestito *tmp = utente_scelto->prestiti;       // scorre la lista
    int ha_prestiti = 0;                               // flag per controllare se sono stati trovati prestiti

    while(tmp){                                        
        if(!tmp->restituito){                          // stampa solo i prestiti non ancora restituiti
            printf("%d - %s\n", tmp->id_libro, tmp->titolo_libro);
            ha_prestiti = 1;                           
        tmp=tmp->next;                                 // passa al nodo successivo
        }
    }

    if(!ha_prestiti){                                  // se nessun prestito attivo trovato
        printf("Non hai prestiti attivi.\n");
        return;                                        
    }

    printf("\nInserisci l'ID del libro da restituire: ");
    scanf("%d", &id_libro);                            

    // scorre di nuovo la lista cercando il prestito corrispondente
    for(tmp = utente_scelto->prestiti; tmp; tmp = tmp->next){
        if(tmp->id_libro == id_libro && !tmp->restituito){   // se trova il prestito attivo corretto
            tmp->restituito = 1;                             // segna il prestito come restituito
            utente_scelto->num_prestiti--;                   // decrementa il contatore dei prestiti attivi

            Libro *libro = cercaLibro(catalogo, id_libro);   // cerca il libro nel catalogo
            if(libro){
                libro->copie_disponibili++;                  // aumenta le copie disponibili dopo che è stato reso
            }

            printf("\nLibro \"%s\" restituito con successo.\n", tmp->titolo_libro);
            return;                                          
        }
    }

    printf("Prestito non trovato o gia' restituito.\n");     // il libro non era in prestito o era già reso
}

void stampaListaPrestitiScaduti(ElencoUtenti *elenco){
    time_t oggi = time(NULL);                          // ottiene la data odierna
    int trovati = 0;                                   

    printf("\n---Prestiti Scaduti---\n");
    printf("%-20s %-20s %-30s %-15s\n",                // intestazione tabella con colonne fisse
        "Nome", "Cognome", "Titolo Libro", "Scaduto il");
    printf("------------------------------------------------------------------------------\n");

    for(int i=0; i<elenco->num; i++){                  
        for(NodoPrestito *tmp = elenco->utenti[i].prestiti; tmp; tmp = tmp->next){ 
            if(!tmp->restituito && tmp->data_scadenza < oggi){  // prestito non restituito E scadenza già passata
                printf("%-20s %-20s %-30s ",
                    elenco->utenti[i].nome,
                    elenco->utenti[i].cognome,
                    tmp->titolo_libro);
                stampData(tmp->data_scadenza);         // stampa la data di scadenza formattata
                printf("\n");
                trovati++;                             // incrementa il contatore
            }
        }
    }

    if(!trovati){
        printf("Nessun prestito scaduto.\n");          // messaggio se non ci sono prestiti scaduti
    }else{
        printf("\nTotale prestiti scaduti: %d\n", trovati);  // prestiti scaduti trovati
    }
}