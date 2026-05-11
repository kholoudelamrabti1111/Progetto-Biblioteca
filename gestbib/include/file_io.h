#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libri.h"
#include "utenti.h"
#include "prestiti.h"

// Percorsi dei file CSV
#define FILE_LIBRI    "../data/libri.csv"
#define FILE_UTENTI   "../data/utenti.csv"
#define FILE_PRESTITI "../data/prestiti.csv"

// Salvataggio
void salvaLibri(CatalogoLibri *catalogo);
void salvaPrestiti(ElencoUtenti *elenco);
void salvaUtenti(ElencoUtenti *elenco);


// Caricamento
void caricaLibri(CatalogoLibri *catalogo);
void caricaPrestiti(ElencoUtenti *elenco);
void caricaUtenti(ElencoUtenti *elenco);

