#pragma once

#include <stdlib.h>
#include "prestiti.h"

typedef struct {
    int id;
    char *nome;
    char*cognome;
    char *email;
    NodoPrestito *prestiti;
    int  num_prestiti;
} Utente;

typedef struct {
    Utente *utenti;
    int num;
    int capacita;
} ElencoUtenti;

