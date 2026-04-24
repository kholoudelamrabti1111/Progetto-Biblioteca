#pragma once


#include <stdlib.h>
#include <time.h>
#include "libri.h"
#include "utenti.h"

typedef struct NodoPrestito NodoPrestito;

typedef struct {
    int     id;
    int     id_libro;
    int     id_utente;
    time_t  data_prestito; 
    time_t  data_scadenza;
    int     restituito;
} Prestito;

struct NodoPrestito {
    Prestito            prestito;
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

