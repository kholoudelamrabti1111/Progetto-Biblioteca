#pragma once

#include <stdlib.h>
// Struttura che rappresenta un utente della biblioteca
typedef struct NodoPrestito NodoPrestito;

typedef struct {
    int id;
    char nome[50];
    char cognome[50];
    char email[100];
    NodoPrestito *prestiti;
    int  num_prestiti;
} Utente;

typedef struct {
    Utente *utenti;
    int num;
    int capacita;
} ElencoUtenti;

// dichiarazione funzioni
void inizializzaElenco(ElencoUtenti *elenco);
void aggiungiUtente(ElencoUtenti *elenco);
void stampaListaUtenti(ElencoUtenti *elenco);
void eliminaUtente(ElencoUtenti *elenco);

