#include <stdio.h>                    
#include <stdlib.h>                  
#include <string.h>                  
#include "../include/libri.h"         

// duplica una stringa allocando dinamicamente la memoria necessaria
char *duplicaStringa(const char *s) {       
    char *p = malloc(strlen(s) + 1);         
    if (p) strcpy(p, s);                    
    return p;                                 
}

// raddoppia la capacità del catalogo quando è pieno
void espandiCatalogo(CatalogoLibri *catalogo) {  
    catalogo->capacita *= 2;                      
                                                   
    catalogo->libri = realloc(catalogo->libri, catalogo->capacita * sizeof(Libro));  
}

// inizializza il catalogo con capacità iniziale di 10 libri
void inizializzaCatalogo(CatalogoLibri *catalogo) {  
    catalogo->capacita = 10;                           
    catalogo->num = 0;                                 
    catalogo->libri = malloc(catalogo->capacita * sizeof(Libro));  
}

// funzione per aggiungere un nuovo libro al catalogo
void aggiungiLibro(CatalogoLibri *catalogo) {  
    char buf[256];                                
                                                  
    if (catalogo->num == catalogo->capacita)     
        espandiCatalogo(catalogo);                // raddoppia la capacità del catalogo

    Libro l;                                     
    l.id = catalogo->num + 1;                    

    printf("Titolo: ");                          // stampa il prompt per il titolo
    scanf(" %[^\n]", buf);                       // legge l'intera riga inclusi gli spazi, ignorando il newline iniziale
    l.titolo = duplicaStringa(buf);              // Crea una copia dinamica della stringa letta e la assegna al campo titolo

    printf("Autore: ");                          
    scanf(" %[^\n]", buf);                       
    l.autore = duplicaStringa(buf);              

    printf("Genere: ");                          
    scanf(" %[^\n]", buf);                       
    l.genere = duplicaStringa(buf);              

    printf("Copie totali: ");                   
    scanf("%d", &l.copie_totali);                
    l.copie_disponibili = l.copie_totali;        
    catalogo->libri[catalogo->num] = l;          // Copia il libro nell'array nella posizione corrente (per valore)
    catalogo->num++;                              
    printf("Libro aggiunto con successo (ID: %d)\n", catalogo->libri[catalogo->num - 1].id);  
}

// Funzione che stampa tutti i libri del catalogo
void stampaListaLibri(CatalogoLibri *catalogo) {  
    if (catalogo->num == 0) {                      // Controlla se il catalogo è vuoto
        printf("Nessun libro nel catalogo.\n");    // Avvisa l'utente che non ci sono libri
        return;                                     
    }

    for (int i = 0; i < catalogo->num; i++) {     
        Libro *l = &catalogo->libri[i];            // Ottiene il puntatore al libro corrente
        printf("ID: %d | Titolo: %s | Autore: %s | Genere: %s | Copie: %d/%d\n",
               l->id, l->titolo, l->autore, l->genere,
               l->copie_disponibili, l->copie_totali);  // Stampa campi del libro
    }
}

// Funzione per modificare i dati di un libro esistente
void modificaLibro(CatalogoLibri *catalogo) { 
    if (catalogo->num == 0) {                     
        printf("Nessun libro nel catalogo.\n");  
        return;                                    
    }

    int id;                                        
    printf("ID del libro da modificare: ");      // Chiede all'utente l'ID del libro da modificare
    scanf("%d", &id);                             

                                                   
    Libro *l = NULL;                              // Inizializza il puntatore a NULL 
    for (int i = 0; i < catalogo->num; i++) {    
        if (catalogo->libri[i].id == id) {       // Confronta gli ID
            l = &catalogo->libri[i];              // Salva il puntatore
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
        case 1:                                    // Modifica il titolo
            printf("Nuovo titolo: ");             // Chiede il nuovo titolo
            scanf(" %[^\n]", buf);                
            free(l->titolo);                      // Libera la vecchia stringa prima di sovrascriverla
            l->titolo = duplicaStringa(buf);      // Assegna la nuova stringa duplicata al campo titolo
            break;                                 
        case 2:                                    // Modifica l'autore
            printf("Nuovo autore: ");             // Chiede il nuovo autore
            scanf(" %[^\n]", buf);                
            free(l->autore);                      // Libera la memoria del vecchio autore
            l->autore = duplicaStringa(buf);      // Assegna il nuovo autore
            break;                                 
        case 3:                                    // Modifica il genere
            printf("Nuovo genere: ");             // Chiede il nuovo genere
            scanf(" %[^\n]", buf);                
            free(l->genere);                      // Libera la memoria del vecchio genere
            l->genere = duplicaStringa(buf);      // Assegna il nuovo genere
            break;                                 
        case 4:                                    // Modifica il numero di copie totali
            printf("Nuove copie totali: ");       // Chiede il nuovo numero di copie
            scanf("%d", &l->copie_totali);        // Legge e aggiorna direttamente il campo copie_totali
            break;                                 
        default:                                   
            printf("Scelta non valida.\n");      
            return;                                
    }
    printf("Libro modificato con successo.\n");  // Conferma la modifica avvenuta con successo
}

// Funzione per rimuovere un libro dal catalogo
void eliminaLibro(CatalogoLibri *catalogo) {  
    if (catalogo->num == 0) {                   
        printf("Nessun libro nel catalogo.\n");
        return;                                  
    }

    int id;                                      
    printf("ID del libro da eliminare: ");     
    scanf("%d", &id);                           

                                                 
    int pos = -1;                               // Inizializza la posizione a -1 (non trovato)
    for (int i = 0; i < catalogo->num; i++) {  
        if (catalogo->libri[i].id == id) {     // Controlla se l'ID corrisponde
            pos = i;                             // Salva l'indice del libro trovato
            break;                             
        }
    }
    if (pos == -1) {                            // Se pos è ancora -1 il libro non è stato trovato
        printf("Libro non trovato.\n");       
        return;                                  
    }

                                        
    free(catalogo->libri[pos].titolo);          
    free(catalogo->libri[pos].autore);          
    free(catalogo->libri[pos].genere);         

// Sposta tutti i libri successivi di una posizione indietro per compattare l'array
    for (int i = pos; i < catalogo->num - 1; i++)  
        catalogo->libri[i] = catalogo->libri[i + 1];  // Sovrascrive ogni elemento con quello successivo

    catalogo->num--;                             // Decrementa il contatore dei libri presenti
    printf("Libro eliminato con successo.\n"); 
}

// Funzione per cercare libri nel catalogo 
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
        Libro *l = &catalogo->libri[i];        // Puntatore al libro corrente
        char *campo = NULL;                     // Puntatore al campo su cui cercare

        // Seleziona il campo su cui effettuare la ricerca
        switch (scelta) {                       
            case 1: campo = l->titolo; break;  
            case 2: campo = l->autore; break; 
            case 3: campo = l->genere; break; 
            default: printf("Scelta non valida.\n"); return;  
        }

        // strstr restituisce NULL se la parola chiave non è stata trovata
        if (strstr(campo, buf) != NULL) {      // Cerca la parola chiave all'interno del campo selezionato
            printf("ID: %d | %s | %s | %s | copie: %d/%d\n",
                   l->id, l->titolo, l->autore, l->genere,
                   l->copie_disponibili, l->copie_totali);  // Stampa il libro trovato
            trovati++;                          // Incrementa il contatore dei risultati trovati
        }
    }

    if (trovati == 0)                          // Se non è stato trovato nessun libro
        printf("Nessun libro trovato.\n");    
    else                                       
        printf("\n%d libro/i trovato/i.\n", trovati);  // Stampa il numero totale di libri trovati
}