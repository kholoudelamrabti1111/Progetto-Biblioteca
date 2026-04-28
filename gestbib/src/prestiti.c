#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/prestiti.h"
 
// cerca un utente nell'elenco tramite il suo id e restituisce il puntatore
Utente* cercaUtente(ElencoUtenti *elenco, int id){
    for(int i=0; i<elenco->num; i++)
        if(elenco->utenti[i].id == id){
            return &elenco->utenti[i];
        }
    return NULL;
}
 
// cerca un libro nel catalogo tramite il suo id e restituisce il puntatore
Libro* cercaLibro(CatalogoLibri *catalogo, int id){
    for(int i=0; i<catalogo->num; i++)
        if(catalogo->libri[i].id == id){
            return &catalogo->libri[i];
        }
    return NULL;
}
 
// stampa una data in formato dd/mm/yyyy partendo da un time_t
void stampData(time_t t){
    struct tm *info = localtime(&t);
    printf("%02d/%02d/%04d", info->tm_mday, info->tm_mon + 1, info->tm_year + 1900);
}
 
// crea un nuovo nodo prestito e lo inserisce in testa alla lista dell'utente
void aggiungiNodoPrestito(Utente *u, int id_libro, char *titolo){
    NodoPrestito *nuovo = (NodoPrestito *)malloc(sizeof(NodoPrestito));
    if(nuovo==NULL){
        printf("Errore di allocazione memoria.\n");
        return;
    }
 
    nuovo->id_libro = id_libro;
    strncpy(nuovo->titolo_libro, titolo, 99);
    nuovo->titolo_libro[99] = '\0'; // terminatore di sicurezza
 
    nuovo->data_prestito = time(NULL); // data di oggi
    nuovo->data_scadenza = nuovo->data_prestito+(30 * 24 * 60 * 60); // oggi + 30 giorni in secondi
    nuovo->restituito = 0; // il libro non e' ancora stato restituito
    nuovo->next = u->prestiti; // inserimento in testa alla lista
    u->prestiti = nuovo;
    u->num_prestiti++;
}
 
// gestisce il prestito di un libro ad un utente
void nuovoPrestito(CatalogoLibri *catalogo, ElencoUtenti *elenco){
    int id_utente, id_libro;
 
    // stampo tutti i libri del catalogo con le copie disponibili
    printf("\n--- Lista Libri ---\n");
    printf("%-5s %-30s %-20s %-15s %-10s\n", "ID", "Titolo", "Autore", "Genere", "Disponibili");
    printf("-------------------------------------------------------------------------------------\n");
 
    for (int i = 0; i < catalogo->num; i++) {
        printf("%-5d %-30s %-20s %-15s %-10d\n",
            catalogo->libri[i].id,
            catalogo->libri[i].titolo,
            catalogo->libri[i].autore,
            catalogo->libri[i].genere,
            catalogo->libri[i].copie_disponibili);
    }
 
    // chiedo l'id dell'utente e lo cerco nell'elenco
    printf("\nInserisci il tuo ID utente: ");
    scanf("%d", &id_utente);
 
    Utente *utente_scelto = cercaUtente(elenco, id_utente);
    if (!utente_scelto) {
        printf("Utente non trovato!\n");
        return;
    }
 
    // chiedo l'id del libro e lo cerco nel catalogo
    printf("Inserisci l'ID del libro che vuoi prendere in prestito: ");
    scanf("%d", &id_libro);
 
    Libro *libro_scelto=cercaLibro(catalogo, id_libro);
    if(!libro_scelto){
        printf("Libro non trovato.\n");
        return;
    }
 
    // controllo se ci sono copie disponibili
    if(libro_scelto->copie_disponibili<=0) {
        printf("Non ci sono copie disponibili per \"%s\".\n", libro_scelto->titolo);
        return;
    }
 
    // controllo se gia' in prestito
    for(NodoPrestito *tmp = utente_scelto->prestiti; tmp; tmp = tmp->next){
        if(tmp->id_libro == id_libro && tmp->restituito == 0){
            printf("Hai gia' questo libro in prestito.\n");
            return;
        }
    }
 
    // aggiungo il prestito e aggiorno le copie disponibili
    aggiungiNodoPrestito(utente_scelto, libro_scelto->id, libro_scelto->titolo);
    libro_scelto->copie_disponibili--;
 
    // stampo il riepilogo del prestito
    printf("\nPrestito effettuato con successo!\n");
    printf("Libro: %s\n", libro_scelto->titolo);
    printf("Data prestito: ");
    stampData(utente_scelto->prestiti->data_prestito);
    printf("\nData scadenza: ");
    stampData(utente_scelto->prestiti->data_scadenza);
    printf("\n");
}
 
 
// gestisce la restituzione di un libro da parte di un utente
void restituzioneLibro(CatalogoLibri *catalogo, ElencoUtenti *elenco){
    int id_utente, id_libro;
 
    // chiedo l'id dell'utente e lo cerco nell'elenco
    printf("\n---Restituzione Libro---\n");
    printf("Inserisci il tuo ID utente: ");
    scanf("%d", &id_utente);
 
    Utente *utente_scelto = cercaUtente(elenco, id_utente);
    if (!utente_scelto) {
        printf("Utente non trovato!\n");
        return;
    }
 
    // stampo i prestiti attivi dell'utente
    printf("\nPrestiti attivi di %s %s:\n", utente_scelto->nome, utente_scelto->cognome);
 
    NodoPrestito *tmp = utente_scelto->prestiti;
    int ha_prestiti = 0;
 
    while(tmp){
        if(!tmp->restituito){
            printf("%d - %s\n", tmp->id_libro, tmp->titolo_libro);
            ha_prestiti = 1;
        }
        tmp=tmp->next;
    }
 
    // se non ha prestiti attivi esco
    if(!ha_prestiti){
        printf("Non hai prestiti attivi.\n");
        return;
    }
 
    // chiedo quale libro vuole restituire
    printf("\nInserisci l'ID del libro da restituire: ");
    scanf("%d", &id_libro);
 
    // cerco il prestito nella lista e lo segno come restituito
    for(tmp = utente_scelto->prestiti; tmp; tmp = tmp->next){
        if(tmp->id_libro == id_libro && !tmp->restituito){
            tmp->restituito = 1;
            utente_scelto->num_prestiti--;
 
            // aggiorno le copie disponibili nel catalogo
            Libro *libro = cercaLibro(catalogo, id_libro);
            if(libro){
                libro->copie_disponibili++;
            }
 
            printf("\nLibro \"%s\" restituito con successo.\n", tmp->titolo_libro);
            return;
        }
    }
 
    printf("Prestito non trovato o gia' restituito.\n");
}
 
 
// stampa tutti i prestiti scaduti e non ancora restituiti
void stampaListaPrestitiScaduti(ElencoUtenti *elenco){
    time_t oggi = time(NULL); // prendo la data di oggi
    int trovati = 0;
 
    printf("\n---Prestiti Scaduti---\n");
    printf("%-20s %-20s %-30s %-15s\n", "Nome", "Cognome", "Titolo Libro", "Scaduto il");
    printf("------------------------------------------------------------------------------\n");
 
    // scorro tutti gli utenti e tutti i loro prestiti
    for(int i=0; i<elenco->num; i++){
        for(NodoPrestito *tmp = elenco->utenti[i].prestiti; tmp; tmp = tmp->next){
            // stampo solo i prestiti non restituiti e con data scadenza passata
            if(!tmp->restituito && tmp->data_scadenza < oggi){
                printf("%-20s %-20s %-30s ",
                    elenco->utenti[i].nome,
                    elenco->utenti[i].cognome,
                    tmp->titolo_libro);
                stampData(tmp->data_scadenza);
                printf("\n");
                trovati++;
            }
        }
    }
 
    if(!trovati){
        printf("Nessun prestito scaduto.\n");
    }else{
        printf("\nTotale prestiti scaduti: %d\n", trovati);
    }
}
