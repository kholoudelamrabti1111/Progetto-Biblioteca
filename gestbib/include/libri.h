#pragma once


#include <stdlib.h>

typedef struct {
    int   id;
    char *titolo;
    char *autore;
    char *genere;
    int   copie_totali;
    int   copie_disponibili;
} Libro;

typedef struct {
    Libro *libri;
    int    num;
    int    capacita;
} CatalogoLibri;

