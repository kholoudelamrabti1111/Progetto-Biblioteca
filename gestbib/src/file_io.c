#include "include/file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int salva_libri(const char *filename, Libro *lista, int conteggio) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) return 0;

    for (int i = 0; i < conteggio; i++){
        fprintf(fp, "%d,%s,%s,%s\n", lista[i].id, lista[i].titolo, lista[i].autore, lista[i].genere);
    }

    fclose(fp);
    return 1;
}

// CARICAMENTO: Legge il file e riempie l'array in memoria
int carica_libri(const char *filename, Libro *lista, int *conteggio) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) return 0;

    char linea[256];// Buffer per leggere ogni riga
    *conteggio = 0;

    while (fgets(linea, sizeof(linea), fp)) {
        
        // %[^,] significa: leggi tutto fino alla virgola
        sscanf(linea, "%d,%[^,],%[^,\n],%[^,\n]", 
               &lista[*conteggio].id, 
               lista[*conteggio].titolo, 
               lista[*conteggio].autore,
               lista[*conteggio].genere);
        
        (*conteggio)++;
    }

    fclose(fp);
    return 1;
}
// SALVATAGGIO PRESTITI (o Storico)
int salva_prestiti(const char *filename, Prestito *lista, int conteggio) {
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    for (int i = 0; i < conteggio; i++) {
        // Formato: id_prestito, id_libro, id_utente, data_inizio, data_fine
        fprintf(fp, "%d,%d,%d,%s,%s\n", 
                lista[i].id, 
                lista[i].id_libro, 
                lista[i].id_utente, 
                lista[i].data_prestito, 
                lista[i].data_scadenza);
    }
    fclose(fp);
    return 1;
}

// CARICAMENTO PRESTITI (o Storico)
int carica_prestiti(const char *filename, Prestito *lista, int *conteggio) {/*Quando carichi i dati dal file, la funzione legge il CSV e scopre quante righe ci sono*/ 
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char linea[256];
    *conteggio = 0;
    while (fgets(linea, sizeof(linea), fp)) {
        sscanf(linea, "%d,%d,%d,%[^,],%[^\n]", 
               &lista[*conteggio].id, 
               &lista[*conteggio].id_libro, 
               &lista[*conteggio].id_utente, 
               lista[*conteggio].data_prestito, 
               lista[*conteggio].data_scadenza);
        (*conteggio)++;
    }
    fclose(fp);
    return 1;
}

int salva_utenti(const char *filename, Utente *lista, int conteggio) {// Quando salvi i dati, la funzione scrive ogni utente su una riga del CSV, con i campi separati da virgole
    FILE *fp = fopen(filename, "w");
    if (!fp) return 0;

    for (int i = 0; i < conteggio; i++) {
        // Formato: id, nome, cognome, email
        fprintf(fp, "%d,%s,%s,%s\n", 
                lista[i].id, 
                lista[i].nome, 
                lista[i].cognome, 
                lista[i].email);
    }
    fclose(fp);
    return 1;
}

int carica_utenti(const char *filename, Utente *lista, int *conteggio) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char linea[256];
    *conteggio = 0;
    while (fgets(linea, sizeof(linea), fp)) {
        sscanf(linea, "%d,%[^,],%[^,],%[^\n]", 
               &lista[*conteggio].id, 
               lista[*conteggio].nome, 
               lista[*conteggio].cognome, 
               lista[*conteggio].email);
        (*conteggio)++;
    }
    fclose(fp);
    return 1;
}