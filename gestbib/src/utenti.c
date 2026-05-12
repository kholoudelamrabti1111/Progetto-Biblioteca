#include <stdio.h>
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include "../include/libri.h"
#include <stdlib.h>
#include <string.h>

//Inizializza elenco di utenti 
void inizializzaElenco(ElencoUtenti *elenco) {
    elenco->num = 0;              
    elenco->capacita = 10;        
    elenco->utenti = malloc(elenco->capacita * sizeof(Utente)); // Alloca memoria per 10 struct Utente
}

//Aggiunge un nuovo utente all'elenco
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
    
    // Inizializza i campi del nuovo utente
    elenco->utenti[elenco->num].id = elenco->num + 1;                        //Assegna un ID
    strcpy(elenco->utenti[elenco->num].nome, nome);                          //Copia il nome
    strcpy(elenco->utenti[elenco->num].cognome, cognome);                    //Copia il cognome
    strcpy(elenco->utenti[elenco->num].email, email);                        //Copia l'email
    elenco->utenti[elenco->num].prestiti = NULL;                             //Inizializza lista prestiti vuota
    elenco->utenti[elenco->num].num_prestiti = 0;                            //Imposta numero di prestiti a 0

    elenco->num++;
    printf("Utente aggiunto con successo (ID: %d)\n", elenco->utenti[elenco->num-1].id);
}


//Stampa la lista di tutti gli utenti 
void stampaListaUtenti(ElencoUtenti *elenco){ 
    if(elenco->num == 0){ 
        printf("Nessun utente presente.\n"); 
        return;
    }

    for(int i=0; i< elenco->num; i++){ 
        printf("ID: %d, Nome: %s, Cognome: %s, Email: %s\n", elenco->utenti[i].id, elenco->utenti[i].nome, elenco->utenti[i].cognome, elenco->utenti[i].email); //Stampa i dettagli dell'utente

        NodoPrestito* corrente = elenco->utenti[i].prestiti;        //Puntatore al primo prestito dell'utente
        
        if(corrente == NULL){ 
            printf("  Nessun prestito per questo utente.\n"); 
        } else { 
            printf("  Prestiti:\n"); 
            while(corrente != NULL){ 
                printf(" - Titolo: %s, Data di prestito: ", corrente->titolo_libro); // Stampa il titolo del libro prestato
                stampData(corrente->data_prestito);                 //funzione per stampare la data
                printf("\n");
                corrente = corrente->next;                          // Passa al prestito successivo
            }
        }
    }
}


//Elimina un utente dall'elenco
void eliminaUtente(ElencoUtenti *elenco){
    int idCercato;
    printf("ID dell'utente da eliminare: ");
    scanf("%d", &idCercato);                         
    
    int pos = -1;                           //Inizializza la posizione a -1 (non trovata)

    // Trova posizione dell'utente
    for(int i=0; i<elenco->num; i++){ 
        if(elenco->utenti[i].id == idCercato){ 
            pos = i;                        // Salva la posizione trovata 
            break;
        }
    }
    if(pos == -1){ 
        printf("Utente con ID %d non trovato.\n", idCercato); // Messaggio di errore se l'utente non è stato trovato
        return; 
    }

    // Libera la lista dei prestiti dell'utente
    NodoPrestito *tmp = elenco->utenti[pos].prestiti;   
    while(tmp != NULL){                                 
        NodoPrestito *next = tmp->next;                 // Salva il puntatore al prossimo nodo
        free(tmp);                                      // Libera la memoria del nodo corrente
        tmp = next;                                     // Passa al prossimo nodo
    }

    // Sposta gli elementi successivi
    for(int i=pos; i<elenco->num-1; i++){ 
        elenco->utenti[i] = elenco->utenti[i+1];        // Copia l'elemento successivo nella posizione corrente
    }
    elenco->num--;                                      //Decrementa numero utenti
    printf("Utente eliminato con successo.\n"); 
}