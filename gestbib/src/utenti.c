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
    strncpy(listaUtenti[numeroUtenti].nome, nomeInserito, 50);
    strncpy(listaUtenti[numeroUtenti].cognome, cognomeInserito, 50);
    strncpy(listaUtenti[numeroUtenti].email, emailInserito, 100);// Copia il nome inserito, assicurandosi di non superare la dimensione massima
    listaUtenti[numeroUtenti].prestiti = NULL;// Inizializza la lista dei prestiti a NULL

    numeroUtenti++; // Incrementa il numero di utenti
}


void stampaListaUtenti(){
    // Implementazione della funzione per stampare la lista degli utenti

    if(numeroUtenti == 0){//controllo se l'array è vuoto
        printf("Nessun utente presente.\n");
        return;
    }

    for(int i=0; i< numeroUtenti; i++){
        printf("ID: %d, Nome: %s, Cognome: %s, Email: %s\n", listaUtenti[i].id, listaUtenti[i].nome, listaUtenti[i].cognome, listaUtenti[i].email);

        NodoPrestito* corrente = listaUtenti[i].prestiti;// Puntatore per scorrere la lista dei prestiti dell'utente
        
        if(corrente == NULL){
            printf("  Nessun prestito per questo utente.\n");
        } else {
            printf("  Prestiti:\n");
            while(corrente != NULL){
                printf(" - Titolo: %s, Data di prestito: %ld\n", corrente->prestito.titolo, (long)corrente->prestito.data_prestito);// Stampa il titolo del libro e la data di prestito
                corrente = corrente->next; // Passa al prestito successivo
            }
        }
    }
}

void eliminaUtente(int idCercato){
    int pos = -1;
    // Implementazione della funzione per eliminare un utente
    for(int i=0; i<numeroUtenti; i++){
        if(listaUtenti[i].id == idCercato){
            pos = i;// Salva la posizione dell'utente da eliminare
            break;// Esce dal ciclo una volta trovato l'utente

        }
    }
    if(pos == -1){
        printf("Utente con ID %d non trovato.\n", idCercato);
        return;
    }
    // Elimina l'utente dall'array
    for(int i=pos; i<numeroUtenti-1; i++){
        listaUtenti[i] = listaUtenti[i+1];
    }
    numeroUtenti--;
    if(numeroUtenti == 0){
        free(listaUtenti);// Libera la memoria se non ci sono più utenti
        listaUtenti = NULL;
    } else {
        Utente* temp = (Utente*)realloc(listaUtenti, numeroUtenti * sizeof(Utente));
        if (temp != NULL) {
            listaUtenti = temp;// Aggiorna il puntatore all'array di utenti
        }
    }
    

}

