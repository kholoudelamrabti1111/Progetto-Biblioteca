#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_io.h"

// Salva tutti i libri del catalogo nel file CSV
void salvaLibri(CatalogoLibri *catalogo) {
    FILE *fp = fopen(FILE_LIBRI, "w");
    
    // Se il puntatore al file è NULL, significa che l'apertura è fallita
    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_LIBRI);
        return;
    }

    for (int i = 0; i < catalogo->num; i++) {
        Libro *l = &catalogo->libri[i];
        
        // Scrive i dati del libro nel file usando la virgola come separatore
        fprintf(fp, "%d,%s,%s,%s,%d,%d\n",
                l->id, l->titolo, l->autore, l->genere,
                l->copie_totali, l->copie_disponibili);
    }

    fclose(fp);
}

// Carica i libri dal file CSV e li inserisce nella struttura dati del catalogo
void caricaLibri(CatalogoLibri *catalogo) {
    FILE *fp = fopen(FILE_LIBRI, "r");
    
    // Se il file non esiste o non può essere aperto esce
    if (!fp) return;

    char linea[512];
    char titolo[256], autore[256], genere[256];
    int id, copie_totali, copie_disponibili;

    // Legge il file fino alla fine del documento
    while (fgets(linea, sizeof(linea), fp)) {
        linea[strcspn(linea, "\n")] = '\0';

        // Estrae i campi dalla stringa formattata. %255[^,] legge fino a 255 caratteri escludendo la virgola
        // sscanf restituisce il numero di campi convertiti con successo
        if (sscanf(linea, "%d,%255[^,],%255[^,],%255[^,],%d,%d",
                   &id, titolo, autore, genere,
                   &copie_totali, &copie_disponibili) != 6)
            // Se la riga non contiene esattamente 6 campi, viene scartata saltando al prossimo ciclo
            continue;

        if (catalogo->num == catalogo->capacita) {
            catalogo->capacita *= 2;                // Raddoppia il valore della capacità teorica del catalogo
            catalogo->libri = realloc(catalogo->libri,
                                      catalogo->capacita * sizeof(Libro));
        }

        // Ottiene il puntatore alla prima posizione libera dell'array nel catalogo
        Libro *l=&catalogo->libri[catalogo->num];
        l->id=id;
        // Alloca dinamicamente la memoria esatta e copia il testo per titolo, autore e genere
        l->titolo=duplicaStringa(titolo); 
        l->autore=duplicaStringa(autore);
        l->genere=duplicaStringa(genere);
        
        l->copie_totali = copie_totali;         // Assegna il numero di copie totali e disponibili caricate dal file
        l->copie_disponibili = copie_disponibili;

        catalogo->num++;
    }

    fclose(fp);
}

// Salva i dati anagrafici di tutti gli utenti nel file CSV
void salvaUtenti(ElencoUtenti *elenco) {
    FILE *fp = fopen(FILE_UTENTI, "w");

    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_UTENTI);
        return;
    }

    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];
        // Scrive sul file i 4 campi dell'utente separati da virgole
        fprintf(fp, "%d,%s,%s,%s\n",
                u->id, u->nome, u->cognome, u->email);
    }

    fclose(fp);
}

// Carica l'elenco degli utenti dal file CSV
void caricaUtenti(ElencoUtenti *elenco) {

    FILE *fp = fopen(FILE_UTENTI, "r");
    if (!fp) return;

    char linea[512];
    int id;
    char nome[50], cognome[50], email[100];

    while (fgets(linea, sizeof(linea), fp)) {
        linea[strcspn(linea, "\n")] = '\0';

        // estrae ID, stringhe fino a 49 caratteri per nome e cognome e fino a 99 caratteri per l'email
        if (sscanf(linea, "%d,%49[^,],%49[^,],%99[^\n]",
                   &id, nome, cognome, email) != 4)
            // Se non vengono identificati tutti e 4 i campi richiesti passa alla riga successiva
            continue;

        // Se l'array degli utenti è pieno espande lo spazio disponibile
        if (elenco->num == elenco->capacita) {
            // Raddoppia la capacità massima dell'elenco
            elenco->capacita *= 2;
            // Modifica la dimensione allocata per l'array degli utenti
            elenco->utenti = realloc(elenco->utenti,
                                     elenco->capacita * sizeof(Utente));
        }

        Utente *u=&elenco->utenti[elenco->num];             // Definisce il puntatore per il nuovo utente nella posizione corrente dell'elenco
        u->id=id;
        u->prestiti=NULL;       
        u->num_prestiti=0;
        
        strncpy(u->nome,nome,sizeof(u->nome)-1);            // sizeof(###)-1 fa rimanere sempre spazio per \0
        strncpy(u->cognome,cognome, sizeof(u->cognome)-1);
        strncpy(u->email,email,sizeof(u->email)-1);
        
        elenco->num++;
    }

    fclose(fp);
}

// Esporta tutti i prestiti attivi e passati nel file CSV
void salvaPrestiti(ElencoUtenti *elenco) {
    FILE *fp = fopen(FILE_PRESTITI, "w");

    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_PRESTITI);
        return;
    }

    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];
        
        for (NodoPrestito *nodo = u->prestiti; nodo != NULL; nodo = nodo->next) {
            // Salva nel file i dati convertendo i timestamp time_t in tipo long int
            fprintf(fp, "%d,%d,%s,%ld,%ld,%d\n",       // Campi: id_utente, id_libro, titolo_libro, data_prestito, data_scadenza, stato_restituito
                    u->id,
                    nodo->id_libro,
                    nodo->titolo_libro,
                    (long)nodo->data_prestito,
                    (long)nodo->data_scadenza,
                    nodo->restituito);
        }
    }

    fclose(fp);
}

// Carica i prestiti dal CSV e li associa agli utenti esistenti
void caricaPrestiti(ElencoUtenti *elenco) {
    FILE *fp = fopen(FILE_PRESTITI, "r");

    if (!fp) return;

    char linea[512];
    int id_utente, id_libro, restituito;
    long data_prestito, data_scadenza;
    char titolo_libro[100];

    while (fgets(linea, sizeof(linea), fp)) {
        linea[strcspn(linea, "\n")] = '\0';                 // elimina il carattere (\n)

        // prende i 6 campi del prestito e verifica l'esattezza della lettura
        if (sscanf(linea, "%d,%d,%99[^,],%ld,%ld,%d",
                   &id_utente, &id_libro, titolo_libro,
                   &data_prestito, &data_scadenza, &restituito) != 6)
            continue;

        Utente *u = NULL;
        // Cerca l'utente all'interno dell'elenco confrontando gli ID
        for (int i = 0; i < elenco->num; i++) {
            if (elenco->utenti[i].id == id_utente) {
                u = &elenco->utenti[i];                      //salva il puntatore all'utente trovato
                break;
            }
        }
        
        if (!u) continue;       // Se l'utente non esiste passa oltre

        NodoPrestito *nuovo = malloc(sizeof(NodoPrestito));         // Alloca memoria per un nuovo nodo della lista prestiti
        if (!nuovo) continue;

        nuovo->id_libro=id_libro;       // Copia i dati 
        // Converte i valori numerici long registrati nel file nel tipo time_t
        nuovo->data_prestito=(time_t)data_prestito;
        nuovo->data_scadenza=(time_t)data_scadenza;
        nuovo->restituito=restituito;
        
        // Copia il titolo del libro
        strncpy(nuovo->titolo_libro, titolo_libro, 99);
        nuovo->titolo_libro[99] = '\0';     // Inserisce il terminatore di stringa nell'ultimo byte

        nuovo->next = u->prestiti;  // il nuovo nodo punta alla vecchia testa della lista prestiti dell'utente
        u->prestiti = nuovo;        // il puntatore di testa dell utente diventa adesso il nuovo nodo appena creato
        
        // Se il flag è uguale a 0 incrementa
        if (!restituito) 
            u->num_prestiti++;
    }

    fclose(fp);
}