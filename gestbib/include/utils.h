#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libri.h"
#include "utenti.h"
#include "prestiti.h"

void libroPiuPrestato(CatalogoLibri *catalogo, ElencoUtenti *elenco);
void utentePiuPrestiti(ElencoUtenti *elenco);
void storicoPrestiti(ElencoUtenti *elenco);
void tassoRestituzione(ElencoUtenti *elenco);
void generiPiuRichiesti(CatalogoLibri *catalogo, ElencoUtenti *elenco);