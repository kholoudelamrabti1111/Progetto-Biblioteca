#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_io.h"

// ─────────────────────────────────────────────
//  LIBRI
// ─────────────────────────────────────────────

// Salva tutti i libri del catalogo nel file CSV
void salvaLibri(CatalogoLibri *catalogo) {
    // Apre il file dei libri in modalità scrittura
    FILE *fp = fopen(FILE_LIBRI, "w");
    // Se il file non può essere aperto, stampa un errore ed esce dalla funzione
    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_LIBRI);
        return;
    }

    // Cicla attraverso tutti i libri presenti nel catalogo
    for (int i = 0; i < catalogo->num; i++) {
        Libro *l = &catalogo->libri[i];
        // Scrive i dati del libro nel formato: id,titolo,autore,genere,copie_tot,copie_disponibili
        fprintf(fp, "%d,%s,%s,%s,%d,%d\n",
                l->id, l->titolo, l->autore, l->genere,
                l->copie_totali, l->copie_disponibili);
    }

    // Chiude il file
    fclose(fp);
}

// Carica i libri dal file CSV e li inserisce nella struttura dati del catalogo
void caricaLibri(CatalogoLibri *catalogo) {
    // Apre il file dei libri in modalità lettura
    FILE *fp = fopen(FILE_LIBRI, "r");
    // Se il file non esiste, termina la funzione
    if (!fp) return;

    char linea[512];
    char titolo[256], autore[256], genere[256];
    int id, copie_totali, copie_disponibili;

    // Legge il file riga per riga
    while (fgets(linea, sizeof(linea), fp)) {
        // Rimuove il carattere di nuova riga (\n) alla fine della stringa
        linea[strcspn(linea, "\n")] = '\0';

        // Estrae i dati dalla riga CSV; se non sono 6 campi, salta la riga
        if (sscanf(linea, "%d,%255[^,],%255[^,],%255[^,],%d,%d",
                   &id, titolo, autore, genere,
                   &copie_totali, &copie_disponibili) != 6)
            continue;

        // Gestisce il ridimensionamento dinamico della memoria se il catalogo è pieno
        if (catalogo->num == catalogo->capacita) {
            catalogo->capacita *= 2;
            catalogo->libri = realloc(catalogo->libri,
                                      catalogo->capacita * sizeof(Libro));
        }

        // Punta all'indice corrente del catalogo e assegna i valori caricati
        Libro *l            = &catalogo->libri[catalogo->num];
        l->id               = id;
        l->titolo           = duplicaStringa(titolo); // Alloca e copia la stringa
        l->autore           = duplicaStringa(autore);
        l->genere           = duplicaStringa(genere);
        l->copie_totali     = copie_totali;
        l->copie_disponibili = copie_disponibili;
        // Incrementa il contatore dei libri caricati
        catalogo->num++;
    }

    // Chiude il file
    fclose(fp);
}

// ─────────────────────────────────────────────
//  UTENTI
// ─────────────────────────────────────────────

// Salva i dati anagrafici di tutti gli utenti nel file CSV
void salvaUtenti(ElencoUtenti *elenco) {
    // Apre il file utenti in scrittura
    FILE *fp = fopen(FILE_UTENTI, "w");
    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_UTENTI);
        return;
    }

    // Cicla l'elenco degli utenti e scrive id, nome, cognome ed email
    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];
        fprintf(fp, "%d,%s,%s,%s\n",
                u->id, u->nome, u->cognome, u->email);
    }

    // Chiude il file
    fclose(fp);
}

// Carica l'elenco degli utenti dal file CSV
void caricaUtenti(ElencoUtenti *elenco) {
    FILE *fp = fopen(FILE_UTENTI, "r");
    if (!fp) return;

    char linea[512];
    int id;
    char nome[50], cognome[50], email[100];

    // Legge ogni riga del file
    while (fgets(linea, sizeof(linea), fp)) {
        linea[strcspn(linea, "\n")] = '\0';

        // Analizza i 4 campi richiesti
        if (sscanf(linea, "%d,%49[^,],%49[^,],%99[^\n]",
                   &id, nome, cognome, email) != 4)
            continue;

        // Ridimensiona l'array dinamico se necessario
        if (elenco->num == elenco->capacita) {
            elenco->capacita *= 2;
            elenco->utenti = realloc(elenco->utenti,
                                     elenco->capacita * sizeof(Utente));
        }

        // Inizializza l'utente nell'elenco
        Utente *u       = &elenco->utenti[elenco->num];
        u->id           = id;
        u->prestiti     = NULL;       // La lista dei prestiti verrà caricata dopo
        u->num_prestiti = 0;
        // Copia in sicurezza le stringhe nei campi a dimensione fissa
        strncpy(u->nome,    nome,    sizeof(u->nome)    - 1);
        strncpy(u->cognome, cognome, sizeof(u->cognome) - 1);
        strncpy(u->email,   email,   sizeof(u->email)   - 1);
        elenco->num++;
    }

    fclose(fp);
}

// ─────────────────────────────────────────────
//  PRESTITI
// ─────────────────────────────────────────────

// Esporta tutti i prestiti attivi e passati nel file CSV
void salvaPrestiti(ElencoUtenti *elenco) {
    FILE *fp = fopen(FILE_PRESTITI, "w");
    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_PRESTITI);
        return;
    }

    // Per ogni utente, percorre la lista collegata dei prestiti
    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];
        for (NodoPrestito *nodo = u->prestiti; nodo != NULL; nodo = nodo->next) {
            // Salva id utente, id libro, titolo e i timestamp delle date
            fprintf(fp, "%d,%d,%s,%ld,%ld,%d\n",
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
        linea[strcspn(linea, "\n")] = '\0';

        // Legge i campi del prestito
        if (sscanf(linea, "%d,%d,%99[^,],%ld,%ld,%d",
                   &id_utente, &id_libro, titolo_libro,
                   &data_prestito, &data_scadenza, &restituito) != 6)
            continue;

        // Cerca l'utente a cui appartiene il prestito tramite ID
        Utente *u = NULL;
        for (int i = 0; i < elenco->num; i++) {
            if (elenco->utenti[i].id == id_utente) {
                u = &elenco->utenti[i];
                break;
            }
        }
        // Se l'utente non viene trovato (es. rimosso), ignora il prestito
        if (!u) continue;

        // Alloca un nuovo nodo per la lista dei prestiti dell'utente
        NodoPrestito *nuovo = malloc(sizeof(NodoPrestito));
        if (!nuovo) continue;

        // Assegna i dati al nuovo nodo
        nuovo->id_libro      = id_libro;
        nuovo->data_prestito = (time_t)data_prestito;
        nuovo->data_scadenza = (time_t)data_scadenza;
        nuovo->restituito    = restituito;
        strncpy(nuovo->titolo_libro, titolo_libro, 99);
        nuovo->titolo_libro[99] = '\0';

        // Inserisce il nodo in testa alla lista collegata dell'utente
        nuovo->next = u->prestiti;
        u->prestiti = nuovo;
        
        // Se il libro non è stato ancora restituito, incrementa il contatore prestiti attivi
        if (!restituito) u->num_prestiti++;
    }

    fclose(fp);
}