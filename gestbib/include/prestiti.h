#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libri.h"
#include "utenti.h"

void stampData(time_t t);

typedef struct NodoPrestito NodoPrestito;

typedef struct {
    char   titolo[100];
    int     id;
    int     id_libro;
    int     id_utente;
    time_t  data_prestito; 
    time_t  data_scadenza;
    int     restituito;
} Prestito;

struct NodoPrestito {
    int id_libro;
    char titolo_libro[100];
    time_t data_prestito;
    time_t data_scadenza;
    int restituito;
    struct NodoPrestito *next;
};

typedef struct NodoNotifica {
    char               *messaggio;
    struct NodoNotifica *next;
} NodoNotifica;

typedef struct {
    NodoNotifica *testa;
    NodoNotifica *coda;
} CodaNotifiche;

// dichiarazione funzioni
void nuovoPrestito(CatalogoLibri *catalogo, ElencoUtenti *elenco);
void restituzioneLibro(CatalogoLibri *catalogo, ElencoUtenti *elenco);
void stampaListaPrestitiScaduti(ElencoUtenti *elenco);

