#include <stdio.h>
#include "../include/utils.h"
#include "../include/libri.h"
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include <stdlib.h>
#include <string.h>

// Ordina il catalogo di libri in ordine alfabetico per titolo usando l'algoritmo Selection Sort
void ordina_libri(Libro **catalogo, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;                                                    // il libro con titolo più piccolo sia quello alla posizione i
        for (int j = i + 1; j < n; j++) {
            if (strcmp(catalogo[j]->titolo, catalogo[min_idx]->titolo) < 0) // Confronta i titoli dei libri usando strcmp
                min_idx = j;                                                // Aggiorna l'indice del libro con il titolo più piccolo trovato finora
        }
        Libro *temp = catalogo[min_idx];                                    // Salva temporaneamente il puntatore al libro con titolo più piccolo
        catalogo[min_idx] = catalogo[i];                                    // Sposta il puntatore del libro corrente nella posizione del minimo trovato
        catalogo[i] = temp;                                                 // Posiziona il libro con titolo più piccolo nella posizione corrente i
    }
}

// Calcola e stampa le statistiche generali del sistema: libri in catalogo, copie disponibili e prestiti attivi
void stampa_statistiche(Libro **catalogo, int n_libri, Utente *utenti, int n_utenti) {
    int totale_copie = 0;                                               
    int prestiti_attivi = 0;                                                

    // Somma le copie disponibili di ogni libro nel catalogo
    for (int i = 0; i < n_libri; i++) 
    {
        totale_copie += catalogo[i]->copie_disponibili;                     // aggiunge le copie disponibili al libro corrente del vettore 
    }

    // Conta i prestiti attivi 
    for (int i = 0; i < n_utenti; i++) {
        NodoPrestito *corr = utenti[i].prestiti;                            // Punta alla testa della lista 

        // Scorre la lista fino a NULL
        while (corr) {
            prestiti_attivi++;                                              // Incrementa il contatore per ogni nodo-prestito trovato
            corr = corr->next;                                              // Avanza al nodo successivo della lista
        }
    }

    printf("\n--- STATISTICHE SISTEMA ---\n");
    printf("Libri in catalogo: %d\n", n_libri);
    printf("Copie disponibili totali: %d\n", totale_copie);
    printf("Prestiti attualmente in corso: %d\n", prestiti_attivi);
    printf("---------------------------\n");
}

// Calcola statistiche sui prestiti: totale, restituiti e numero di volte che ogni libro è stato preso in prestito
void calcolaStatistiche(Libro** catalogo, int numLibri, Utente** utenti, int numUtenti) {
    int* conteggioLibri = calloc(numLibri, sizeof(int)); 
    int resituiti = 0, totali = 0; 

    for (int i = 0; i < numUtenti; i++) {
        NodoPrestito* corrente = utenti[i]->prestiti;                       // Punta alla testa della lista dell'utente corrente
        
        while (corrente != NULL) {
            totali++; 
            if (corrente->restituito) resituiti++;                          // Incrementa i restituiti se il flag è attivo

            // Cerca nel catalogo il libro corrispondente al prestito
            for (int j = 0; j < numLibri; j++) {
                if (catalogo[j]->id == corrente->id_libro) {                // Confronta l'id del libro nel catalogo con quello del prestito
                    conteggioLibri[j]++;                        
                    break; 
                }
            }
            corrente = corrente->next;                                      // Avanza al prestito successivo 
        }
    }

    free(conteggioLibri); 
}

// Stampa una barra di avanzamento basata sul rapporto tra valore e scala
void stampaBarra(int valore, int scala) {
    printf("["); 
    for (int i = 0; i < valore / scala; i++) { 
        printf("|");                                                         // Stampa un '|' per ogni unità della barra
    }
    printf("] (%d)\n", valore); 
}

// Individua e stampa il libro più prestato tra tutti gli utenti 
void libroPiuPrestato(CatalogoLibri *catalogo, ElencoUtenti *elenco) {
    // Controllo libri e utenti
    if (catalogo->num == 0 || elenco->num == 0) {
        printf("Nessun dato disponibile.\n");
        return;
    }

    int max = 0;                                                        // Numero massimo di prestiti 
    int idx = -1;                                                       // Indice del libro più prestato inizializzato a -1 (nessuno trovato)

    // Scorre ogni libro del catalogo per contare quante volte è stato prestato
    for (int j = 0; j < catalogo->num; j++) {
        int contatore = 0; 

        // Scorre utenti e i loro prestiti per contare quante volte il libro è stato prestato
        for (int i = 0; i < elenco->num; i++) {
            for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
                if (nodo->id_libro == catalogo->libri[j].id) 
                    contatore++; 
            }
        }

        // Aggiorna il massimo 
        if (contatore > max) {
            max = contatore; 
            idx = j; 
        }
    }

    // Stampa risultato o un messaggio se non ci sono prestiti
    if (idx == -1 || max == 0) {
        printf("Nessun prestito registrato.\n");
    } else {
        printf("\n--- Libro piu' prestato ---\n");
        printf("Titolo : %s\n", catalogo->libri[idx].titolo);
        printf("Autore : %s\n", catalogo->libri[idx].autore);
        printf("Prestiti totali: %d\n", max);
    }
}

// Individua e stampa l'utente che ha effettuato più prestiti
void utentePiuPrestiti(ElencoUtenti *elenco) {
    // controlla utente
    if (elenco->num == 0) {
        printf("Nessun utente presente.\n");
        return;
    }

    int max = 0; 
    int idx = -1; 

    // Scorre utenti e conta i nodi nella sua lista
    for (int i = 0; i < elenco->num; i++) {
        int contatore = 0; 

        // Atraversa la lista collegata dei prestiti dell'utente
        for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next)
            contatore++; 

        // Aggiorna il massimo se utente ha più prestiti del record attuale
        if (contatore > max) {
            max = contatore; 
            idx = i; 
        }
    }

    if (idx == -1 || max == 0) {
        printf("Nessun prestito registrato.\n");
    } else {
        printf("\n--- Utente con piu' prestiti ---\n");
        printf("Nome : %s %s\n", elenco->utenti[idx].nome, elenco->utenti[idx].cognome);
        printf("Email : %s\n", elenco->utenti[idx].email);
        printf("Prestiti totali: %d\n", max);
    }
}

// Stampa lo storico dei prestiti di tutti gli utenti
void storicoPrestiti(ElencoUtenti *elenco) {
    // Controlla utenti
    if (elenco->num == 0) {
        printf("Nessun utente presente.\n");
        return;
    }

    printf("\n--- Storico Prestiti ---\n");

    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];                                 // Puntatore all'utente corrente 

        printf("\n%s %s:\n", u->nome, u->cognome); 

        // Controlla prestiti
        if (u->prestiti == NULL) {
            printf(" Nessun prestito.\n");
            continue;                                                   // Salta al prossimo utente se non ci sono prestiti
        }

        // Atraversa la lista collegata dei prestiti e stampa 
        for (NodoPrestito *nodo = u->prestiti; nodo != NULL; nodo = nodo->next) {
            printf(" - %-30s | Prestito: ", nodo->titolo_libro);        // Stampa il titolo del libro con larghezza fissa
            stampData(nodo->data_prestito); 
            printf(" | Scadenza: ");
            stampData(nodo->data_scadenza); 
            printf(" | %s\n", nodo->restituito ? "Restituito" : "In corso");        // Indica se il libro è già stato restituito
        }
    }
}

// Calcola e stampa il tasso percentuale di restituzione dei prestiti
void tassoRestituzione(ElencoUtenti *elenco) {
    int totali = 0; 
    int restituiti = 0; 

    // Scorre utenti e le loro liste di prestiti
    for (int i = 0; i < elenco->num; i++) {
        for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
            totali++; 
            if (nodo->restituito)
                restituiti++;                                       // Incrementa restituiti se il flag è attivo
        }
    }

    // Evita la divisione per zero 
    if (totali == 0) {
        printf("Nessun prestito registrato.\n");
        return;
    }

    float tasso = (float)restituiti / totali * 100;                 // Calcola la percentuale di restituzione

    printf("\n--- Tasso di Restituzione ---\n");
    printf("Prestiti totali : %d\n", totali);
    printf("Restituiti : %d\n", restituiti);
    printf("Non restituiti : %d\n", totali - restituiti); 
    printf("Tasso : %.1f%%\n", tasso); 
}

// Stampa per ogni libro del catalogo quante volte è stato prestato raggruppando per genere
void generiPiuRichiesti(CatalogoLibri *catalogo, ElencoUtenti *elenco) {
    // Controlli
    if (catalogo->num == 0 || elenco->num == 0) {
        printf("Nessun dato disponibile.\n");
        return;
    }

    printf("\n--- Generi piu' richiesti ---\n");

    // Scorre libri per contarne i prestiti
    for (int j = 0; j < catalogo->num; j++) {
        int contatore = 0; 

                                                                    // Scorre tutti gli utenti e le loro liste di prestiti
        for (int i = 0; i < elenco->num; i++) {
            for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
                if (nodo->id_libro == catalogo->libri[j].id)        // Verifica se il prestito riguarda il libro j
                    contatore++; 
            }
        }

        printf("%-20s : %d prestiti\n", catalogo->libri[j].genere, contatore);
    }
}