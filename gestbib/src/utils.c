#include <stdio.h>
#include "../include/utils.h"
#include "../include/libri.h"
#include "../include/utenti.h"
#include "../include/prestiti.h"
#include <stdlib.h>
#include <string.h>

// Selection Sort per titoli di libri (ordinamento alfabetico)
void ordina_libri(Libro **catalogo, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (strcmp(catalogo[j]->titolo, catalogo[min_idx]->titolo) < 0)  // Confronta i titoli dei libri usando strcmp
                min_idx = j;                                               // Trova l'indice del libro con il titolo più piccolo
        }
        // Scambio solo i puntatori
        Libro *temp = catalogo[min_idx];                                 // Salva il puntatore al libro con titolo più piccolo
        catalogo[min_idx] = catalogo[i];                                 // Sposta il puntatore del libro corrente al posto del libro con titolo più piccolo
        catalogo[i] = temp;                                              // Posiziona il puntatore del libro con titolo più piccolo al posto del libro corrente
    }
}

void stampa_statistiche(Libro **catalogo, int n_libri, Utente *utenti, int n_utenti) {
    int totale_copie = 0;
    int prestiti_attivi = 0;

    for (int i = 0; i < n_libri; i++) 
    {
        totale_copie += catalogo[i]->copie_disponibili;                  // somma le copie rimaste fisicamente in biblioteca
    }

    for (int i = 0; i < n_utenti; i++) {                              // scorre la lista collegata di ogni utente
        NodoPrestito *corr = utenti[i].prestiti;                       // Punta alla testa della lista dei prestiti dell'utente i-esimo

        while (corr) {                                                 // Attraversa la lista finché non arriva a NULL (fine lista)
            prestiti_attivi++;                                         // Incrementa il contatore per ogni prestito trovato
            corr = corr->next;                                         // Passa al nodo successivo
        }
    }

    printf("\n--- STATISTICHE SISTEMA ---\n");
    printf("Libri in catalogo: %d\n", n_libri);
    printf("Copie disponibili totali: %d\n", totale_copie);
    printf("Prestiti attualmente in corso: %d\n", prestiti_attivi);
    printf("---------------------------\n");
}

void calcolaStatistiche(Libro** catalogo, int numLibri, Utente** utenti, int numUtenti) {
    int* conteggioLibri = calloc(numLibri, sizeof(int));
    int resituiti = 0, totali = 0;

    for (int i = 0; i < numUtenti; i++) {
        NodoPrestito* corrente = utenti[i]->prestiti;
        while (corrente != NULL) {
            totali++;
            if (corrente->restituito) resituiti++;                     // Assumendo un flag restituito
            
            // Trova l'indice del libro nel catalogo per incrementare il contatore
            for (int j = 0; j < numLibri; j++) {
                if (catalogo[j]->id == corrente->id_libro) {
                    conteggioLibri[j]++;
                    break;
                }
            }
            corrente = corrente->next;                                // Passa al prestito successivo
        }
    }

    // Qui cerchi il max in conteggioLibri e stampi...
    free(conteggioLibri);
}

void stampaBarra(int valore, int scala) {                                 // Funzione per stampare una barra di testo proporzionale al valore dato, scalato da un fattore
    printf("[");                                                          // Inizio della barra
    for (int i = 0; i < valore / scala; i++) {                               // Stampa un simbolo '|' per ogni unità di scala nel valore
        printf("|");                                                      // Disegna un segmento della barra
    }
    printf("] (%d)\n", valore);                                          // Fine della barra e stampa del valore numerico tra parentesi
}

void libroPiuPrestato(CatalogoLibri *catalogo, ElencoUtenti *elenco) {
    if (catalogo->num == 0 || elenco->num == 0) {
        printf("Nessun dato disponibile.\n");
        return;
    }

    int max = 0;
    int idx = -1;

    // per ogni libro conta quante volte è stato prestato
    for (int j = 0; j < catalogo->num; j++) {
        int contatore = 0;

        for (int i = 0; i < elenco->num; i++) {
            for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
                if (nodo->id_libro == catalogo->libri[j].id)
                    contatore++;
            }
        }

        if (contatore > max) {
            max = contatore;
            idx = j;
        }
    }

    if (idx == -1 || max == 0) {
        printf("Nessun prestito registrato.\n");
    } else {
        printf("\n--- Libro piu' prestato ---\n");
        printf("Titolo : %s\n", catalogo->libri[idx].titolo);
        printf("Autore : %s\n", catalogo->libri[idx].autore);
        printf("Prestiti totali: %d\n", max);
    }
}

void utentePiuPrestiti(ElencoUtenti *elenco) {
    if (elenco->num == 0) {
        printf("Nessun utente presente.\n");
        return;
    }

    int max = 0;
    int idx = -1;

    // per ogni utente conta i prestiti nella lista collegata
    for (int i = 0; i < elenco->num; i++) {
        int contatore = 0;

        for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next)
            contatore++;

        if (contatore > max) {
            max = contatore;
            idx = i;
        }
    }

    if (idx == -1 || max == 0) {
        printf("Nessun prestito registrato.\n");
    } else {
        printf("\n--- Utente con piu' prestiti ---\n");
        printf("Nome    : %s %s\n", elenco->utenti[idx].nome, elenco->utenti[idx].cognome);
        printf("Email   : %s\n", elenco->utenti[idx].email);
        printf("Prestiti totali: %d\n", max);
    }
}

void storicoPrestiti(ElencoUtenti *elenco) {
    if (elenco->num == 0) {
        printf("Nessun utente presente.\n");
        return;
    }

    printf("\n--- Storico Prestiti ---\n");

    for (int i = 0; i < elenco->num; i++) {
        Utente *u = &elenco->utenti[i];

        printf("\n%s %s:\n", u->nome, u->cognome);

        if (u->prestiti == NULL) {
            printf("  Nessun prestito.\n");
            continue;
        }

        for (NodoPrestito *nodo = u->prestiti; nodo != NULL; nodo = nodo->next) {
            printf("  - %-30s | Prestito: ", nodo->titolo_libro);
            stampData(nodo->data_prestito);
            printf(" | Scadenza: ");
            stampData(nodo->data_scadenza);
            printf(" | %s\n", nodo->restituito ? "Restituito" : "In corso");
        }
    }
}

void tassoRestituzione(ElencoUtenti *elenco) {
    int totali = 0;
    int restituiti = 0;

    for (int i = 0; i < elenco->num; i++) {
        for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
            totali++;
            if (nodo->restituito)
                restituiti++;
        }
    }

    if (totali == 0) {
        printf("Nessun prestito registrato.\n");
        return;
    }

    float tasso = (float)restituiti / totali * 100;

    printf("\n--- Tasso di Restituzione ---\n");
    printf("Prestiti totali  : %d\n", totali);
    printf("Restituiti       : %d\n", restituiti);
    printf("Non restituiti   : %d\n", totali - restituiti);
    printf("Tasso            : %.1f%%\n", tasso);
}

void generiPiuRichiesti(CatalogoLibri *catalogo, ElencoUtenti *elenco) {
    if (catalogo->num == 0 || elenco->num == 0) {
        printf("Nessun dato disponibile.\n");
        return;
    }

    printf("\n--- Generi piu' richiesti ---\n");

    // per ogni genere conta quante volte è stato prestato
    for (int j = 0; j < catalogo->num; j++) {
        int contatore = 0;

        for (int i = 0; i < elenco->num; i++) {
            for (NodoPrestito *nodo = elenco->utenti[i].prestiti; nodo != NULL; nodo = nodo->next) {
                if (nodo->id_libro == catalogo->libri[j].id)
                    contatore++;
            }
        }

        printf("%-20s : %d prestiti\n", catalogo->libri[j].genere, contatore);
    }
}
