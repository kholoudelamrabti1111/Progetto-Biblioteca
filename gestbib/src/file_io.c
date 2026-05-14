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

        // Analizza la riga: estrae ID, stringhe fino a 49 caratteri per nome e cognome, 
        // e fino a 99 caratteri per l'email (escludendo il carattere di a capo finale)
        if (sscanf(linea, "%d,%49[^,],%49[^,],%99[^\n]",
                   &id, nome, cognome, email) != 4)
            // Se non vengono identificati tutti e 4 i campi richiesti, passa alla riga successiva
            continue;

        // Se l'array degli utenti è pieno, espande lo spazio disponibile
        if (elenco->num == elenco->capacita) {
            // Raddoppia la capacità massima dichiarata nell'elenco
            elenco->capacita *= 2;
            // Modifica la dimensione della memoria allocata per l'array degli utenti
            elenco->utenti = realloc(elenco->utenti,
                                     elenco->capacita * sizeof(Utente));
        }

        // Definisce il puntatore per il nuovo utente nella posizione corrente dell'elenco
        Utente *u       = &elenco->utenti[elenco->num];
        // Assegna l'ID estratto
        u->id           = id;
        // Inizializza il puntatore della lista collegata dei prestiti a NULL (vuota)
        u->prestiti     = NULL;       
        // Imposta a 0 il numero di prestiti attivi per questo utente
        u->num_prestiti = 0;
        
        // Copia in modo sicuro le stringhe nei vettori a dimensione fissa della struttura Utente.
        // sizeof(campo) - 1 garantisce che rimanga sempre spazio per il carattere terminatore \0.
        strncpy(u->nome,    nome,    sizeof(u->nome)    - 1);
        strncpy(u->cognome, cognome, sizeof(u->cognome) - 1);
        strncpy(u->email,   email,   sizeof(u->email)   - 1);
        
        // Incrementa il contatore degli utenti totali inseriti nell'elenco
        elenco->num++;
    }

    // Chiude il file al termine della lettura
    fclose(fp);
}

// Esporta tutti i prestiti attivi e passati nel file CSV
void salvaPrestiti(ElencoUtenti *elenco) {
    // Apre il file dei prestiti in modalità scrittura ("w")
    FILE *fp = fopen(FILE_PRESTITI, "w");
    // Se l'apertura fallisce, stampa il messaggio di errore ed esce
    if (!fp) {
        printf("Errore: impossibile aprire %s per la scrittura.\n", FILE_PRESTITI);
        return;
    }

    // Scorre l'elenco di tutti gli utenti registrati nel sistema
    for (int i = 0; i < elenco->num; i++) {
        // Punta all'utente corrente
        Utente *u = &elenco->utenti[i];
        
        // Scorre la lista collegata dei prestiti dell'utente partendo dalla testa (u->prestiti)
        // Continua finché il puntatore al nodo corrente non diventa NULL
        for (NodoPrestito *nodo = u->prestiti; nodo != NULL; nodo = nodo->next) {
            // Salva nel file i dati convertendo i timestamp time_t in tipo long int
            // Campi: id_utente, id_libro, titolo_libro, data_prestito, data_scadenza, stato_restituito
            fprintf(fp, "%d,%d,%s,%ld,%ld,%d\n",
                    u->id,
                    nodo->id_libro,
                    nodo->titolo_libro,
                    (long)nodo->data_prestito,
                    (long)nodo->data_scadenza,
                    nodo->restituito);
        }
    }

    // Chiude il file scrivendo i dati su disco
    fclose(fp);
}

// Carica i prestiti dal CSV e li associa agli utenti esistenti
void caricaPrestiti(ElencoUtenti *elenco) {
    // Apre il file dei prestiti in modalità sola lettura ("r")
    FILE *fp = fopen(FILE_PRESTITI, "r");
    // Se il file non esiste, termina subito la funzione
    if (!fp) return;

    // Buffer per la lettura della riga di testo
    char linea[512];
    // Variabili per i dati estratti dal file
    int id_utente, id_libro, restituito;
    long data_prestito, data_scadenza;
    char titolo_libro[100];

    // Legge il file riga per riga fino alla fine
    while (fgets(linea, sizeof(linea), fp)) {
        // Taglia il carattere di a capo finale (\n)
        linea[strcspn(linea, "\n")] = '\0';

        // Estrae i 6 campi descrittivi del prestito verificando l'esattezza della lettura
        if (sscanf(linea, "%d,%d,%99[^,],%ld,%ld,%d",
                   &id_utente, &id_libro, titolo_libro,
                   &data_prestito, &data_scadenza, &restituito) != 6)
            // Se la riga è corrotta o incompleta, passa alla riga successiva
            continue;

        // Inizializza un puntatore a Utente per cercare il proprietario del prestito
        Utente *u = NULL;
        // Cerca l'utente all'interno dell'elenco confrontando gli ID
        for (int i = 0; i < elenco->num; i++) {
            if (elenco->utenti[i].id == id_utente) {
                // Se trova corrispondenza, salva il puntatore all'utente trovato
                u = &elenco->utenti[i];
                // Interrompe il ciclo di ricerca per ottimizzare i tempi
                break;
            }
        }
        // Se l'utente non esiste più nel sistema, ignora il prestito e passa oltre
        if (!u) continue;

        // Alloca dinamicamente nello heap la memoria per un nuovo nodo della lista prestiti
        NodoPrestito *nuovo = malloc(sizeof(NodoPrestito));
        // Se l'allocazione della memoria fallisce, passa alla riga successiva
        if (!nuovo) continue;

        // Copia i dati letti dal file all'interno dei campi del nuovo nodo allocato
        nuovo->id_libro      = id_libro;
        // Converte i valori numerici long registrati nel file nel tipo nativo time_t
        nuovo->data_prestito = (time_t)data_prestito;
        nuovo->data_scadenza = (time_t)data_scadenza;
        nuovo->restituito    = restituito;
        
        // Copia in sicurezza il titolo del libro nel vettore del nodo (massimo 99 caratteri)
        strncpy(nuovo->titolo_libro, titolo_libro, 99);
        // Inserisce forzatamente il terminatore di stringa nell'ultimo byte disponibile
        nuovo->titolo_libro[99] = '\0';

        // Logica di inserimento in testa alla lista collegata dell'utente:
        // Il nuovo nodo punta alla vecchia testa della lista prestiti dell'utente
        nuovo->next = u->prestiti;
        // Il puntatore di testa dell'utente diventa adesso il nuovo nodo appena creato
        u->prestiti = nuovo;
        
        // Se il flag 'restituito' è uguale a 0 (ovvero il libro è ancora in prestito)
        if (!restituito) 
            // Incrementa il numero dei prestiti attivi in carico a questo utente
            u->num_prestiti++;
    }

    // Chiude il file dopo aver completato l'intero caricamento
    fclose(fp);
}