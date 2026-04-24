#include <stdio.h>
#include "utenti.h"
#include <stdlib.h>
#include <string.h>
int ID = 0;

void aggiungiUtente(const char* nomeInserito){
    Utente* temp = (Utente*)realloc(ElencoUtenti, (numeroUtenti +1) * sizeof(Utente));
    if (temp == NULL) {
        printf("Errore di allocazione della memoria.\n");
        return;
    }
    ElencoUtenti = temp;// Aggiorna il puntatore all'array di utenti


        
}
voidstampaListaUtenti(GestioneUtente *gu){
    // Implementazione della funzione per stampare la lista degli utenti
   
    for (int i = 0; i < gu->num; i++) {
        if (gu->utenti[i].id == id) {

        
            printf("ID:%d\n",  u->id);
            printf("Nome: %s\n",  u->nome);
            printf("Cognome: %s\n",  u->cognome);
            printf("Email: %s\n",  u->email);
            printf("Prestiti: %d\n",  u->num_prestiti);
            

            return;
        }
    }

    printf("Utente con ID %d non trovato.\n", id);
    }
}

void eliminaUtente(){

}

