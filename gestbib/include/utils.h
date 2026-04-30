#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dichiarazione funzioni
// utils.h
void libroPiuPrestato(CatalogoLibri *catalogo, ElencoUtenti *elenco);
void utentePiuPrestiti(ElencoUtenti *elenco);
void storicoPrestiti(ElencoUtenti *elenco);
void tassoRestituzione(CatalogoLibri *catalogo);
void generiPiuRichiesti(CatalogoLibri *catalogo);