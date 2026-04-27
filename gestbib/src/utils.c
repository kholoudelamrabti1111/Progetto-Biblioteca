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
            if (strcmp(catalogo[j]->titolo, catalogo[min_idx]->titolo) < 0)
                min_idx = j;
        }
        // Scambio solo i puntatori
        Libro *temp = catalogo[min_idx];
        catalogo[min_idx] = catalogo[i];
        catalogo[i] = temp;
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
