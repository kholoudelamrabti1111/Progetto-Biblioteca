#include <stdio.h> /* Libreria standard per I/O (printf, scanf) */
#include <stdlib.h> /* Libreria per gestione memoria (malloc, realloc, free) */
#include <string.h> /* Libreria per operazioni su stringhe (strcpy, strstr, strlen) */
#include "../include/libri.h" /* Header del progetto con le definizioni delle strutture Libro e CatalogoLibri */

/* Duplica una stringa allocando dinamicamente la memoria necessaria */
char *duplicaStringa(const char *s) { /* Funzione che riceve una stringa costante e restituisce una copia allocata dinamicamente */
    char *p = malloc(strlen(s) + 1); /* Alloca tanti byte quanti sono i caratteri della stringa più il terminatore '\0' */
    if (p) strcpy(p, s); /* Se l'allocazione è riuscita (p != NULL), copia la stringa s in p */
    return p; /* Restituisce il puntatore alla nuova stringa (o NULL se malloc ha fallito) */
}

/* Raddoppia la capacità del catalogo quando è pieno */
void espandiCatalogo(CatalogoLibri *catalogo) { /* Funzione che riceve il puntatore al catalogo da espandere */
    catalogo->capacita *= 2; /* Moltiplica per 2 la capacità attuale del catalogo */
 /* Ridimensiona l'array dei libri alla nuova capacità */
    catalogo->libri = realloc(catalogo->libri, catalogo->capacita * sizeof(Libro)); /* Rialloca l'array libri con la nuova dimensione in byte */
}

/* Inizializza il catalogo con capacità iniziale di 10 libri */
void inizializzaCatalogo(CatalogoLibri *catalogo) { /* Funzione che prepara il catalogo per l'uso */
    catalogo->capacita = 10; /* Imposta la capacità iniziale a 10 elementi */
    catalogo->num = 0; /* Nessun libro presente all'inizio */
    catalogo->libri = malloc(catalogo->capacita * sizeof(Libro)); /* Alloca l'array iniziale capace di contenere 10 libri */
}

void aggiungiLibro(CatalogoLibri *catalogo) { /* Funzione per aggiungere un nuovo libro al catalogo */
    char buf[256]; /* Buffer temporaneo per leggere l'input dell'utente */

 /* Se il catalogo è pieno, espandi prima di aggiungere */
    if (catalogo->num == catalogo->capacita) /* Controlla se il numero di libri ha raggiunto la capacità massima */
        espandiCatalogo(catalogo); /* In caso affermativo, raddoppia la capacità del catalogo */

    Libro l; /* Alloca temporaneamente un libro */

    l.id = catalogo->num + 1; /* L'ID è basato sulla posizione corrente: primo libro ha ID 1, secondo ID 2, ecc. */

    printf("Titolo: "); /* Stampa il prompt per il titolo */
    scanf(" %[^\n]", buf); /* Legge l'intera riga inclusi gli spazi, ignorando il newline iniziale */
    l.titolo = duplicaStringa(buf); /* Crea una copia dinamica della stringa letta e la assegna al campo titolo */

    printf("Autore: "); /* Stampa il prompt per l'autore */
    scanf(" %[^\n]", buf); /* Legge l'intera riga inclusi gli spazi */
    l.autore = duplicaStringa(buf); /* Crea una copia dinamica della stringa e la assegna al campo autore */

    printf("Genere: "); /* Stampa il prompt per il genere */
    scanf(" %[^\n]", buf); /* Legge l'intera riga inclusi gli spazi */
    l.genere = duplicaStringa(buf); /* Crea una copia dinamica della stringa e la assegna al campo genere */

    printf("Copie totali: "); /* Stampa il prompt per il numero di copie */
    scanf("%d", &l.copie_totali); /* Legge un intero e lo salva in copie_totali */
    l.copie_disponibili = l.copie_totali; /* All'inizio tutte le copie sono disponibili, quindi disponibili = totali */

    catalogo->libri[catalogo->num] = l; /* Copia il libro nell'array nella posizione corrente (per valore) */
    catalogo->num++; /* Incrementa il contatore dei libri presenti nel catalogo */
    printf("Libro aggiunto con successo (ID: %d)\n", catalogo->libri[catalogo->num - 1].id); /* Conferma con l'ID del libro appena inserito */
}

void stampaListaLibri(CatalogoLibri *catalogo) { /* Funzione che stampa tutti i libri presenti nel catalogo */
    if (catalogo->num == 0) { /* Controlla se il catalogo è vuoto */
        printf("Nessun libro nel catalogo.\n"); /* Avvisa l'utente che non ci sono libri */
        return; /* Esce anticipatamente dalla funzione */
    }

    for (int i = 0; i < catalogo->num; i++) { /* Itera su tutti i libri presenti nel catalogo */
        Libro *l = &catalogo->libri[i]; /* Ottiene il puntatore al libro i-esimo per evitare copie inutili */
        printf("ID: %d | Titolo: %s | Autore: %s | Genere: %s | Copie: %d/%d\n",
               l->id, l->titolo, l->autore, l->genere,
               l->copie_disponibili, l->copie_totali); /* Stampa tutti i campi del libro formattati */
    }
}

void modificaLibro(CatalogoLibri *catalogo) { /* Funzione per modificare i dati di un libro esistente */
    if (catalogo->num == 0) { /* Controlla se il catalogo è vuoto */
        printf("Nessun libro nel catalogo.\n"); /* Avvisa l'utente che non ci sono libri da modificare */
        return; /* Esce anticipatamente dalla funzione */
    }

    int id; /* Variabile per memorizzare l'ID del libro da modificare */
    printf("ID del libro da modificare: "); /* Chiede all'utente l'ID del libro */
    scanf("%d", &id); /* Legge l'ID inserito dall'utente */

 /* Cerca il libro con l'ID specificato */
    Libro *l = NULL; /* Inizializza il puntatore a NULL (libro non trovato per default) */
    for (int i = 0; i < catalogo->num; i++) { /* Scorre tutti i libri del catalogo */
        if (catalogo->libri[i].id == id) { /* Confronta l'ID del libro corrente con quello cercato */
            l = &catalogo->libri[i]; /* Salva il puntatore al libro trovato */
            break; /* Interrompe il ciclo perché il libro è stato trovato */
        }
    }
    if (l == NULL) { /* Se il puntatore è ancora NULL, il libro non è stato trovato */
        printf("Libro non trovato.\n"); /* Informa l'utente */
        return; /* Esce dalla funzione */
    }

    char buf[256]; /* Buffer per leggere il nuovo valore dall'utente */
    int scelta; /* Variabile per memorizzare la scelta del campo da modificare */
    printf("Cosa vuoi modificare?\n"); /* Mostra il menu di modifica */
    printf("1. Titolo\n2. Autore\n3. Genere\n4. Copie totali\n"); /* Elenca le opzioni disponibili */
    printf("Scelta: "); /* Chiede la scelta all'utente */
    scanf("%d", &scelta); /* Legge la scelta dell'utente */

    switch (scelta) { /* Esegue l'azione in base al campo scelto */
        case 1: /* Modifica il titolo */
            printf("Nuovo titolo: "); /* Chiede il nuovo titolo */
            scanf(" %[^\n]", buf); /* Legge il nuovo valore inclusi gli spazi */
            free(l->titolo); /* Libera la vecchia stringa prima di sovrascriverla per evitare memory leak */
            l->titolo = duplicaStringa(buf); /* Assegna la nuova stringa duplicata al campo titolo */
            break; /* Esce dallo switch */
        case 2: /* Modifica l'autore */
            printf("Nuovo autore: "); /* Chiede il nuovo autore */
            scanf(" %[^\n]", buf); /* Legge il nuovo valore */
            free(l->autore); /* Libera la memoria del vecchio autore */
            l->autore = duplicaStringa(buf); /* Assegna il nuovo autore */
            break; /* Esce dallo switch */
        case 3: /* Modifica il genere */
            printf("Nuovo genere: "); /* Chiede il nuovo genere */
            scanf(" %[^\n]", buf); /* Legge il nuovo valore */
            free(l->genere); /* Libera la memoria del vecchio genere */
            l->genere = duplicaStringa(buf); /* Assegna il nuovo genere */
            break; /* Esce dallo switch */
        case 4: /* Modifica il numero di copie totali */
            printf("Nuove copie totali: "); /* Chiede il nuovo numero di copie */
            scanf("%d", &l->copie_totali); /* Legge e aggiorna direttamente il campo copie_totali */
            break; /* Esce dallo switch */
        default: /* Gestisce qualsiasi scelta non valida */
            printf("Scelta non valida.\n"); /* Avvisa l'utente */
            return; /* Esce dalla funzione senza modificare nulla */
    }
    printf("Libro modificato con successo.\n"); /* Conferma la modifica avvenuta con successo */
}

void eliminaLibro(CatalogoLibri *catalogo) { /* Funzione per rimuovere un libro dal catalogo */
    if (catalogo->num == 0) { /* Controlla se il catalogo è vuoto */
        printf("Nessun libro nel catalogo.\n"); /* Avvisa che non ci sono libri da eliminare */
        return; /* Esce dalla funzione */
    }

    int id; /* Variabile per memorizzare l'ID del libro da eliminare */
    printf("ID del libro da eliminare: "); /* Chiede l'ID all'utente */
    scanf("%d", &id); /* Legge l'ID inserito */

 /* Cerca la posizione del libro nell'array */
    int pos = -1; /* Inizializza la posizione a -1 (non trovato) */
    for (int i = 0; i < catalogo->num; i++) { /* Scorre tutti i libri */
        if (catalogo->libri[i].id == id) { /* Controlla se l'ID corrisponde */
            pos = i; /* Salva l'indice del libro trovato */
            break; /* Interrompe la ricerca */
        }
    }
    if (pos == -1) { /* Se pos è ancora -1, il libro non è stato trovato */
        printf("Libro non trovato.\n"); /* Avvisa l'utente */
        return; /* Esce dalla funzione */
    }

 /* Libera la memoria delle stringhe allocate dinamicamente */
    free(catalogo->libri[pos].titolo); /* Dealloca la stringa titolo del libro da eliminare */
    free(catalogo->libri[pos].autore); /* Dealloca la stringa autore del libro da eliminare */
    free(catalogo->libri[pos].genere); /* Dealloca la stringa genere del libro da eliminare */

 /* Sposta tutti i libri successivi di una posizione indietro per compattare l'array */
    for (int i = pos; i < catalogo->num - 1; i++) /* Itera dalla posizione eliminata fino al penultimo elemento */
        catalogo->libri[i] = catalogo->libri[i + 1]; /* Sovrascrive ogni elemento con quello successivo (copia per valore) */

    catalogo->num--; /* Decrementa il contatore dei libri presenti */
    printf("Libro eliminato con successo.\n"); /* Conferma l'eliminazione */
}

void ricercaLibro(CatalogoLibri *catalogo) { /* Funzione per cercare libri nel catalogo in base a una parola chiave */
    if (catalogo->num == 0) { /* Controlla se il catalogo è vuoto */
        printf("Nessun libro nel catalogo.\n"); /* Avvisa che non ci sono libri da cercare */
        return; /* Esce dalla funzione */
    }

    int scelta; /* Variabile per il tipo di campo su cui effettuare la ricerca */
    char buf[256]; /* Buffer per la parola chiave di ricerca */
    printf("Cerca per:\n1. Titolo\n2. Autore\n3. Genere\n"); /* Mostra le opzioni di ricerca */
    printf("Scelta: "); /* Chiede la scelta all'utente */
    scanf("%d", &scelta); /* Legge la scelta */

    printf("Parola chiave: "); /* Chiede la parola chiave di ricerca */
    scanf(" %[^\n]", buf); /* Legge la parola chiave inclusi gli spazi */

    int trovati = 0; /* Contatore dei libri trovati, inizializzato a 0 */
    for (int i = 0; i < catalogo->num; i++) { /* Scorre tutti i libri del catalogo */
        Libro *l = &catalogo->libri[i]; /* Puntatore al libro corrente */
        char *campo = NULL; /* Puntatore al campo su cui cercare, inizializzato a NULL */

 /* Seleziona il campo su cui effettuare la ricerca */
        switch (scelta) { /* In base alla scelta dell'utente */
            case 1: campo = l->titolo; break; /* Imposta campo al titolo del libro */
            case 2: campo = l->autore; break; /* Imposta campo all'autore del libro */
            case 3: campo = l->genere; break; /* Imposta campo al genere del libro */
            default: printf("Scelta non valida.\n"); return; /* Scelta non valida: avvisa ed esce */
        }

 /* strstr restituisce NULL se la parola chiave non è presente nel campo */
        if (strstr(campo, buf) != NULL) { /* Cerca la parola chiave all'interno del campo selezionato */
            printf("ID: %d | %s | %s | %s | copie: %d/%d\n",
                   l->id, l->titolo, l->autore, l->genere,
                   l->copie_disponibili, l->copie_totali); /* Stampa il libro trovato */
            trovati++; /* Incrementa il contatore dei risultati trovati */
        }
    }

    if (trovati == 0) /* Se non è stato trovato nessun libro */
        printf("Nessun libro trovato.\n"); /* Avvisa l'utente */
    else /* Altrimenti */
        printf("\n%d libro/i trovato/i.\n", trovati); /* Stampa il numero totale di libri trovati */
}