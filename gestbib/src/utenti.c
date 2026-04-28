#include <stdio.h>
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include "../include/libri.h"
#include <stdlib.h>
#include <string.h>

void inizializzaElenco(ElencoUtenti *elenco) {
    elenco->num = 0;
    elenco->capacita = 10;
    elenco->utenti = malloc(elenco->capacita * sizeof(Utente));
}

void aggiungiUtente(ElencoUtenti *elenco){
    char nome[50], cognome[50], email[100];
    printf("Nome: ");
    scanf(" %[^\n]", nome);
    printf("Cognome: ");
    scanf(" %[^\n]", cognome);
    printf("Email: ");
    scanf(" %[^\n]", email);
    
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


void stampaListaUtenti(ElencoUtenti *elenco){
    if(elenco->num == 0){
        printf("Nessun utente presente.\n");
        return;
    }

    for(int i=0; i< elenco->num; i++){
        printf("ID: %d, Nome: %s, Cognome: %s, Email: %s\n", elenco->utenti[i].id, elenco->utenti[i].nome, elenco->utenti[i].cognome, elenco->utenti[i].email);

        NodoPrestito* corrente = elenco->utenti[i].prestiti;
        
        if(corrente == NULL){
            printf("  Nessun prestito per questo utente.\n");
        } else {
            printf("  Prestiti:\n");
            while(corrente != NULL){
                printf(" - Titolo: %s, Data di prestito: %ld\n", corrente->titolo_libro, (long)corrente->data_prestito);
                corrente = corrente->next;
            }
        }
    }
}

void eliminaUtente(ElencoUtenti *elenco){
    int idCercato;
    printf("ID dell'utente da eliminare: ");
    scanf("%d", &idCercato);
    
    int pos = -1;

    for(int i=0; i<elenco->num; i++){
        if(elenco->utenti[i].id == idCercato){
            pos = i;
            break;
        }
    }
    if(pos == -1){
        printf("Utente con ID %d non trovato.\n", idCercato);
        return;
    }
    // Elimina l'utente dall'array
    for(int i=pos; i<elenco->num-1; i++){
        elenco->utenti[i] = elenco->utenti[i+1];
    }
    elenco->num--;
    printf("Utente eliminato con successo.\n");
}

