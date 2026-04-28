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
            if (strcmp(catalogo[j]->titolo, catalogo[min_idx]->titolo) < 0)//Confronta i titoli dei libri usando strcmp, se il titolo del libro j è alfabeticamente precedente a quello del libro min_idx
                min_idx = j;//Trova l'indice del libro con il titolo più piccolo
        }
        // Scambio solo i puntatori
        Libro *temp = catalogo[min_idx];//Salva il puntatore al libro con titolo più piccolo
        catalogo[min_idx] = catalogo[i];//Sposta il puntatore del libro corrente al posto del libro con titolo più piccolo
        catalogo[i] = temp;//Posiziona il puntatore del libro con titolo più piccolo al posto del libro corrente
    }
}

void stampa_statistiche(Libro **catalogo, int n_libri, Utente *utenti, int n_utenti) {
    int totale_copie = 0;
    int prestiti_attivi = 0;

    for (int i = 0; i < n_libri; i++) 
    {
        totale_copie += catalogo[i]->copie_disponibili;//somma le copie rimaste fisicamente in biblioteca
    }

    for (int i = 0; i < n_utenti; i++) {//scorre la lista collegata di ogni utente

        NodoPrestito *corr = utenti[i].prestiti;//Punta alla testa della lista dei prestiti dell'utente i-esimo

        while (corr) {//Attraversa la lista finché non arriva a NULL (fine lista)

            prestiti_attivi++;//Incrementa il contatore per ogni prestito trovato

            corr = corr->next;//Passa al nodo successivo

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
            if (corrente->restituito) resituiti++; // Assumendo un flag restituito
            
            // Trova l'indice del libro nel catalogo per incrementare il contatore
            for(int j=0; j<numLibri; j++) {
                if (catalogo[j]->id == corrente->id_libro) {
                    conteggioLibri[j]++;
                    break;
                }
            }
            corrente = corrente->next;// Passa al prestito successivo
        }
    }

    // Qui cerchi il max in conteggioLibri e stampi...
    free(conteggioLibri);
}
void stampaBarra(int valore, int scala) {// Funzione per stampare una barra di testo proporzionale al valore dato, scalato da un fattore    
    printf("[");// Inizio della barra
    for (int i = 0; i < valore / scala; i++) {// Stampa un simbolo '|' per ogni unità di scala nel valore
        printf("|");//
    }
    printf("] (%d)\n", valore);// Fine della barra e stampa del valore numerico tra parentesi
}

void libroPiuPrestato() {
    printf("Funzione libroPiuPrestato non implementata.\n");
}

void utentePiuPrestiti() {
    printf("Funzione utentePiuPrestiti non implementata.\n");
}

void storicoPrestiti() {
    printf("Funzione storicoPrestiti non implementata.\n");
}

void tassoRestituzione() {
    printf("Funzione tassoRestituzione non implementata.\n");
}

void generiPiuRichiesti() {
    printf("Funzione generiPiuRichiesti non implementata.\n");
}
