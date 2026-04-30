#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// dichiarazione funzioni
char *duplicaStringa(const char *s);
void espandiCatalogo(CatalogoLibri *catalogo);
void inizializzaCatalogo(CatalogoLibri *catalogo);
void aggiungiLibro(CatalogoLibri *catalogo);
void stampaListaLibri(CatalogoLibri *catalogo);
void modificaLibro(CatalogoLibri *catalogo);
void eliminaLibro(CatalogoLibri *catalogo);
void ricercaLibro(CatalogoLibri *catalogo);

