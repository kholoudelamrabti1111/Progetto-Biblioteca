#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/libri.h"

char *duplicaStringa(const char *s) {
    char *p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

void espandiCatalogo(CatalogoLibri *catalogo) {
    catalogo->capacita *= 2;
    catalogo->libri = realloc(catalogo->libri, catalogo->capacita * sizeof(Libro));
}

void inizializzaCatalogo(CatalogoLibri *catalogo) {
    catalogo->capacita = 10;
    catalogo->num = 0;
    catalogo->libri = malloc(catalogo->capacita * sizeof(Libro));
}

void aggiungiLibro(CatalogoLibri *catalogo) {
    char buf[256];

    if (catalogo->num == catalogo->capacita)
        espandiCatalogo(catalogo);

    Libro *l = malloc(sizeof(Libro));

    l->id = catalogo->num + 1;

    printf("Titolo: ");
    scanf(" %[^\n]", buf);
    l->titolo = duplicaStringa(buf);

    printf("Autore: ");
    scanf(" %[^\n]", buf);
    l->autore = duplicaStringa(buf);

    printf("Genere: ");
    scanf(" %[^\n]", buf);
    l->genere = duplicaStringa(buf);

    printf("Copie totali: ");
    scanf("%d", &l->copie_totali);
    l->copie_disponibili = l->copie_totali;

    catalogo->libri[catalogo->num] = *l;
    free(l);
    catalogo->num++;
    printf("Libro aggiunto con successo (ID: %d)\n", catalogo->libri[catalogo->num - 1].id);
}

void stampaListaLibri(CatalogoLibri *catalogo) {
    if (catalogo->num == 0) {
        printf("Nessun libro nel catalogo.\n");
        return;
    }
    printf("\n%-5s %-30s %-20s %-15s %-6s %-6s\n",
           "ID", "Titolo", "Autore", "Genere", "Totali", "Disp.");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < catalogo->num; i++) {
        Libro *l = &catalogo->libri[i];
        printf("%-5d %-30s %-20s %-15s %-6d %-6d\n",
               l->id, l->titolo, l->autore, l->genere,
               l->copie_totali, l->copie_disponibili);
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

    Libro *l = NULL;
    for (int i = 0; i < catalogo->num; i++) {
        if (catalogo->libri[i].id == id) {
            l = &catalogo->libri[i];
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
            free(l->titolo);
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

    free(catalogo->libri[pos].titolo);
    free(catalogo->libri[pos].autore);
    free(catalogo->libri[pos].genere);

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

        switch (scelta) {
            case 1: campo = l->titolo; break;
            case 2: campo = l->autore; break;
            case 3: campo = l->genere; break;
            default: printf("Scelta non valida.\n"); return;
        }

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