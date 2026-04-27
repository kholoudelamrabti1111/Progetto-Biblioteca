#include <stdio.h>
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include "../include/libri.h"
#include <stdlib.h>
#include <string.h>
int ID = 0;
Utente* listaUtenti = NULL;
int numeroUtenti = 0;

void aggiungiUtente(const char* nomeInserito, const char* cognomeInserito, const char* emailInserito){
    Utente* temp = (Utente*)realloc(listaUtenti, (numeroUtenti +1) * sizeof(Utente));
    if (temp == NULL) {
        printf("Errore di allocazione della memoria.\n");
        return;
    }
    listaUtenti = temp;// Aggiorna il puntatore all'array di utenti
    listaUtenti[numeroUtenti].id = ID++;// Assegna un ID univoco
    strncpy(listaUtenti[numeroUtenti].nome, nomeInserito, 50);// Copia il nome inserito
    strncpy(listaUtenti[numeroUtenti].cognome, cognomeInserito, 50);// Copia il cognome inserito
    strncpy(listaUtenti[numeroUtenti].email, emailInserito, 100);// Copia l'email inserita

    numeroUtenti++; // Incrementa il numero di utenti
}


void stampaListaUtenti(){
    // Implementazione della funzione per stampare la lista degli utenti

    if(numeroUtenti == 0){//controllo se l'array è vuoto
        printf("Nessun utente presente.\n");
        return;
    }

    for(int i=0; i< numeroUtenti; i++){// Scorre l'array di utenti
        printf("ID: %d, Nome: %s, Cognome: %s, Email: %s\n", listaUtenti[i].id, listaUtenti[i].nome, listaUtenti[i].cognome, listaUtenti[i].email);// Stampa le informazioni dell'utente

        NodoPrestito* corrente = listaUtenti[i].prestiti;// Puntatore per scorrere la lista dei prestiti dell'utente
        
        if(corrente == NULL){// Se l'utente non ha prestiti, stampa un messaggio e continua con il prossimo utente
            printf("  Nessun prestito per questo utente.\n");
        } else {
            printf("  Prestiti:\n");
            while(corrente != NULL){// Scorre la lista dei prestiti dell'utente
                printf(" - Titolo: %s, Data di prestito: %ld\n", corrente->prestito.titolo, (long)corrente->prestito.data_prestito);// Stampa il titolo del libro e la data di prestito
                corrente = corrente->next; // Passa al prestito successivo
            }
        }
    }
}

void eliminaUtente(int idCercato){// Implementazione della funzione per eliminare un utente
    int pos = -1;// Variabile per memorizzare la posizione dell'utente da eliminare

    for(int i=0; i<numeroUtenti; i++){// Scorre l'array di utenti
        if(listaUtenti[i].id == idCercato){// Cerca l'utente con l'ID specificato
            pos = i;// Salva la posizione dell'utente da eliminare
            break;// Esce dal ciclo una volta trovato l'utente

        }
    }
    if(pos == -1){// Se l'utente non è stato trovato, stampa un messaggio
        printf("Utente con ID %d non trovato.\n", idCercato);
        return;
    }
    // Elimina l'utente dall'array
    for(int i=pos; i<numeroUtenti-1; i++){// Scorre gli utenti successivi a partire dalla posizione dell'utente da eliminare
        listaUtenti[i] = listaUtenti[i+1];// Sposta gli utenti successivi per coprire la posizione dell'utente eliminato
    }
    numeroUtenti--;// Decrementa il numero di utenti
    if(numeroUtenti == 0){// Se non ci sono più utenti, libera la memoria e imposta il puntatore a NULL
        free(listaUtenti);// Libera la memoria se non ci sono più utenti
        listaUtenti = NULL;
    } else {
        Utente* temp = (Utente*)realloc(listaUtenti, numeroUtenti * sizeof(Utente));// Ridimensiona l'array di utenti dopo l'eliminazione
        if (temp != NULL) {
            listaUtenti = temp;// Aggiorna il puntatore all'array di utenti
        }
    }
    

}

