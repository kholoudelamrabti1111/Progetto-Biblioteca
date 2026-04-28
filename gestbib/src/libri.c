#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libri.h"

/* Duplica una stringa allocando dinamicamente la memoria necessaria */
char *duplicaStringa(const char *s) {
    char *p = malloc(strlen(s) + 1); /* +1 per il carattere terminatore '\0' */
    if (p) strcpy(p, s);             /* Copia la stringa solo se malloc ha avuto successo */
    return p;
}

/* Raddoppia la capacità del catalogo quando è pieno */
void espandiCatalogo(CatalogoLibri *catalogo) {
    catalogo->capacita *= 2;
    /* Ridimensiona l'array dei libri alla nuova capacità */
    catalogo->libri = realloc(catalogo->libri, catalogo->capacita * sizeof(Libro));
}

/* Inizializza il catalogo con capacità iniziale di 10 libri */
void inizializzaCatalogo(CatalogoLibri *catalogo) {
    catalogo->capacita = 10;
    catalogo->num = 0;                                              /* Nessun libro presente all'inizio */
    catalogo->libri = malloc(catalogo->capacita * sizeof(Libro));   /* Alloca l'array iniziale */
}

void aggiungiLibro(CatalogoLibri *catalogo) {
    char buf[256];

    /* Se il catalogo è pieno, espandi prima di aggiungere */
    if (catalogo->num == catalogo->capacita)
        espandiCatalogo(catalogo);

    Libro l; /* Alloca temporaneamente un libro */

    l.id = catalogo->num + 1; /* L'ID è basato sulla posizione corrente */

    printf("Titolo: ");
    scanf(" %[^\n]", buf); /* Legge l'intera riga inclusi gli spazi */
    l.titolo = duplicaStringa(buf);

    printf("Autore: ");
    scanf(" %[^\n]", buf);
    l.autore = duplicaStringa(buf);

    printf("Genere: ");
    scanf(" %[^\n]", buf);
    l.genere = duplicaStringa(buf);

    printf("Copie totali: ");
    scanf("%d", &l.copie_totali);
    l.copie_disponibili = l.copie_totali; /* All'inizio tutte le copie sono disponibili */

    catalogo->libri[catalogo->num] = l; /* Copia il libro nell'array (per valore) */
    catalogo->num++;
    printf("Libro aggiunto con successo (ID: %d)\n", catalogo->libri[catalogo->num - 1].id);
}

void stampaListaLibri(CatalogoLibri *catalogo) {
    if (catalogo->num == 0) {
        printf("Nessun libro nel catalogo.\n");
        return;
    }

    for (int i = 0; i < catalogo->num; i++) {
        Libro *l = &catalogo->libri[i];
        printf("ID: %d | Titolo: %s | Autore: %s | Genere: %s | Copie: %d/%d\n",
               l->id, l->titolo, l->autore, l->genere,
               l->copie_disponibili, l->copie_totali);
    }
}

void modificaLibro(CatalogoLibri *catalogo) {
    if (catalogo->num == 0) {
        printf("Nessun libro nel catalogo.\n");
        return;
    }

    int id;
    printf("ID del libro da modificare: ");
    scanf("%d", &id);

    /* Cerca il libro con l'ID specificato */
    Libro *l = NULL;
    for (int i = 0; i < catalogo->num; i++) {
        if (catalogo->libri[i].id == id) {
            l = &catalogo->libri[i]; /* Salva il puntatore al libro trovato */
            break;
        }
    }
    if (l == NULL) {
        printf("Libro non trovato.\n");
        return;
    }

    char buf[256];
    int scelta;
    printf("Cosa vuoi modificare?\n");
    printf("1. Titolo\n2. Autore\n3. Genere\n4. Copie totali\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    switch (scelta) {
        case 1:
            printf("Nuovo titolo: ");
            scanf(" %[^\n]", buf);
            free(l->titolo);              /* Libera la vecchia stringa prima di sovrascriverla */
            l->titolo = duplicaStringa(buf);
            break;
        case 2:
            printf("Nuovo autore: ");
            scanf(" %[^\n]", buf);
            free(l->autore);
            l->autore = duplicaStringa(buf);
            break;
        case 3:
            printf("Nuovo genere: ");
            scanf(" %[^\n]", buf);
            free(l->genere);
            l->genere = duplicaStringa(buf);
            break;
        case 4:
            printf("Nuove copie totali: ");
            scanf("%d", &l->copie_totali);
            break;
        default:
            printf("Scelta non valida.\n");
            return;
    }
    printf("Libro modificato con successo.\n");
}

void eliminaLibro(CatalogoLibri *catalogo) {
    if (catalogo->num == 0) {
        printf("Nessun libro nel catalogo.\n");
        return;
    }

    int id;
    printf("ID del libro da eliminare: ");
    scanf("%d", &id);

    /* Cerca la posizione del libro nell'array */
    int pos = -1;
    for (int i = 0; i < catalogo->num; i++) {
        if (catalogo->libri[i].id == id) {
            pos = i;
            break;
        }
    }
    if (pos == -1) {
        printf("Libro non trovato.\n");
        return;
    }

    /* Libera la memoria delle stringhe allocate dinamicamente */
    free(catalogo->libri[pos].titolo);
    free(catalogo->libri[pos].autore);
    free(catalogo->libri[pos].genere);

    /* Sposta tutti i libri successivi di una posizione indietro per compattare l'array */
    for (int i = pos; i < catalogo->num - 1; i++)
        catalogo->libri[i] = catalogo->libri[i + 1];

    catalogo->num--;
    printf("Libro eliminato con successo.\n");
}

void ricercaLibro(CatalogoLibri *catalogo) {
    if (catalogo->num == 0) {
        printf("Nessun libro nel catalogo.\n");
        return;
    }

    int scelta;
    char buf[256];
    printf("Cerca per:\n1. Titolo\n2. Autore\n3. Genere\n");
    printf("Scelta: ");
    scanf("%d", &scelta);

    printf("Parola chiave: ");
    scanf(" %[^\n]", buf);

    int trovati = 0;
    for (int i = 0; i < catalogo->num; i++) {
        Libro *l = &catalogo->libri[i];
        char *campo = NULL;

        /* Seleziona il campo su cui effettuare la ricerca */
        switch (scelta) {
            case 1: campo = l->titolo; break;
            case 2: campo = l->autore; break;
            case 3: campo = l->genere; break;
            default: printf("Scelta non valida.\n"); return;
        }

        /* strstr restituisce NULL se la parola chiave non è presente nel campo */
        if (strstr(campo, buf) != NULL) {
            printf("ID: %d | %s | %s | %s | copie: %d/%d\n",
                   l->id, l->titolo, l->autore, l->genere,
                   l->copie_disponibili, l->copie_totali);
            trovati++;
        }
    }

    if (trovati == 0)
        printf("Nessun libro trovato.\n");
    else
        printf("\n%d libro/i trovato/i.\n", trovati);
}