/*
 * File: utenti.c
 * Gestione degli utenti della biblioteca.
 * Include funzioni per inizializzare, aggiungere, stampare ed eliminare utenti.
 */

#include <stdio.h>
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include "../include/libri.h"
#include <stdlib.h>
#include <string.h>

/*
 * Inizializza l'elenco degli utenti allocando memoria iniziale.
 */
void inizializzaElenco(ElencoUtenti *elenco) {
    elenco->num = 0;
    elenco->capacita = 10;
    elenco->utenti = malloc(elenco->capacita * sizeof(Utente));
}

/*
 * Aggiunge un nuovo utente all'elenco chiedendo input da tastiera.
 * Gestisce il ridimensionamento dinamico dell'array se necessario.
 */
void aggiungiUtente(ElencoUtenti *elenco){
    char nome[50], cognome[50], email[100];
    printf("Nome: ");
    scanf(" %[^\n]", nome);
    printf("Cognome: ");
    scanf(" %[^\n]", cognome);
    printf("Email: ");
    scanf(" %[^\n]", email);
    
    // Ridimensiona l'array se pieno
    if (elenco->num == elenco->capacita) {
        elenco->capacita *= 2;
        elenco->utenti = realloc(elenco->utenti, elenco->capacita * sizeof(Utente));
    }
    
    elenco->utenti[elenco->num].id = elenco->num + 1;
    strcpy(elenco->utenti[elenco->num].nome, nome);
    strcpy(elenco->utenti[elenco->num].cognome, cognome);
    strcpy(elenco->utenti[elenco->num].email, email);
    elenco->utenti[elenco->num].prestiti = NULL;
    elenco->utenti[elenco->num].num_prestiti = 0;

    elenco->num++;
    printf("Utente aggiunto con successo (ID: %d)\n", elenco->utenti[elenco->num-1].id);
}

/*
 * Stampa la lista di tutti gli utenti con i loro dettagli e prestiti attivi.
 */
void stampaListaUtenti(ElencoUtenti *elenco){ /* Funzione per stampare tutti gli utenti e i loro prestiti */
    if(elenco->num == 0){ /* Se non ci sono utenti nell'elenco */
        printf("Nessun utente presente.\n"); /* Stampa messaggio di elenco vuoto */
        return; /* Esce dalla funzione */
    }

    for(int i=0; i< elenco->num; i++){ /* Ciclo su tutti gli utenti presenti */
        printf("ID: %d, Nome: %s, Cognome: %s, Email: %s\n", elenco->utenti[i].id, elenco->utenti[i].nome, elenco->utenti[i].cognome, elenco->utenti[i].email); /* Stampa i dettagli dell'utente corrente */

        NodoPrestito* corrente = elenco->utenti[i].prestiti; /* Puntatore al primo prestito dell'utente */
        
        if(corrente == NULL){ /* Se l'utente non ha prestiti */
            printf("  Nessun prestito per questo utente.\n"); /* Messaggio di nessun prestito */
        } else { /* Altrimenti, se ci sono prestiti */
            printf("  Prestiti:\n"); /* Intestazione per la lista dei prestiti */
            while(corrente != NULL){ /* Ciclo su tutti i prestiti dell'utente */
                printf(" - Titolo: %s, Data di prestito: ", corrente->titolo_libro); /* Stampa il titolo del libro prestato */
                stampData(corrente->data_prestito); /* Chiama la funzione per stampare la data */
                printf("\n"); /* Nuova linea dopo la data */
                corrente = corrente->next; /* Passa al prestito successivo */
            }
        }
    }
}

/*
 * Elimina un utente dall'elenco in base all'ID fornito.
 * Libera anche la memoria dei prestiti associati.
 */
void eliminaUtente(ElencoUtenti *elenco){ /* Funzione per eliminare un utente dall'elenco */
    int idCercato; /* Variabile per memorizzare l'ID dell'utente da eliminare */
    printf("ID dell'utente da eliminare: "); 
    scanf("%d", &idCercato); /* Legge l'ID dall'input */
    
    int pos = -1; /* Inizializza la posizione a -1 (non trovata) */

    // Trova la posizione dell'utente
    for(int i=0; i<elenco->num; i++){ /* Ciclo per cercare l'utente con l'ID specificato */
        if(elenco->utenti[i].id == idCercato){ /* Se l'ID corrisponde */
            pos = i; /* Salva la posizione trovata */
            break; /* Esce dal ciclo */
        }
    }
    if(pos == -1){ /* Se l'utente non è stato trovato */
        printf("Utente con ID %d non trovato.\n", idCercato); /* Messaggio di errore */
        return; /* Esce dalla funzione */
    }

    // Libera la lista dei prestiti dell'utente
    NodoPrestito *tmp = elenco->utenti[pos].prestiti; /* Puntatore temporaneo al primo prestito */
    while(tmp != NULL){ /* Ciclo per liberare tutti i prestiti */
        NodoPrestito *next = tmp->next; /* Salva il puntatore al prossimo nodo */
        free(tmp); /* Libera la memoria del nodo corrente */
        tmp = next; /* Passa al prossimo nodo */
    }

    // Sposta gli elementi successivi per riempire il vuoto
    for(int i=pos; i<elenco->num-1; i++){ /* Ciclo per spostare gli elementi successivi */
        elenco->utenti[i] = elenco->utenti[i+1]; /* Copia l'elemento successivo nella posizione corrente */
    }
    elenco->num--; /* Decrementa il numero di utenti */
    printf("Utente eliminato con successo.\n"); /* Conferma l'eliminazione */
}