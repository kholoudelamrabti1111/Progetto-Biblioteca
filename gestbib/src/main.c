#include <stdio.h>
#include <stdlib.h>

int main(){
    int scelta=0, scelta_utente=0,scelta_libri=0,scelta_prestiti=0, scelta_statistiche;

    do{
        printf("\n--- GESTBIB ---\n");
        printf("In che menù vuoi andare?\n");
        printf("1. Menù Utenti\n");
        printf("2. Menù Libri\n");
        printf("3. Menù Prestiti\n");
        printf("4. Menù Statistiche\n");
        printf("0. Esci\n");
        printf("Scelta: ");
        scanf("%d", &scelta);


        switch(scelta){

        case 1:
            do{
                printf("\n===Menù Utenti===\n");
                printf("1. Aggiungi utente\n");
                printf("2. Stampa lista utenti\n");
                printf("3. Eliminazione utente\n");
                printf("0. Esci\n");
                printf("Scelta: ");
                scanf("%d", &scelta_utente);

                switch(scelta_utente){
                    case 1:
                        break;

                    case 2:
                        break;

                    case 3:
                        break;

                    case 0:
                        break;
                }
            }while(scelta_utente!=0);
            break;

        case 2:
            do{
                printf("\n===Menù Libri===\n");
                printf("1. Aggiungi libro\n");
                printf("2. Stampa lista libri\n");
                printf("3. Modifica libro\n");
                printf("4. Eliminazione libro\n");
                printf("5. Ricerca Libro\n");
                printf("0. Esci\n");
                printf("Scelta: ");
                scanf("%d", &scelta_libri);

                switch(scelta_libri){
                    case 1:
                        break;

                    case 2:
                        break;

                    case 3:
                        break;

                    case 4:
                        break;

                    case 5:
                        break;

                    case 0:
                        break;
                }
            }while(scelta_libri!=0);
            break;

        case 3:
            do{
                printf("\n===Menù Prestiti===\n");
                printf("1. Nuovo prestito\n");
                printf("2. Restituzione libro\n");
                printf("3. Stampa lista prestiti scaduti\n");
                printf("0. Esci\n");
                printf("Scelta: ");
                scanf("%d", &scelta_prestiti);

                switch(scelta_prestiti){
                    case 1:
                        break;

                    case 2:
                        break;

                    case 3:
                        break;

                    case 0:
                        break;
                }
            }while(scelta_prestiti!=0);
            break;

        case 4:
            do{
                printf("\n===Menù Statistiche===\n");
                printf("1. Libro più prestato\n");
                printf("2. Utente con più prestiti\n");
                printf("3. Visualizza storico prestiti\n");
                printf("4. Visualizza tasso di restituzione\n");
                printf("5. Visualizza generi più richiesti\n");
                printf("0. Esci\n");
                printf("Scelta: ");
                scanf("%d", &scelta_statistiche);

                switch(scelta_statistiche){
                    case 1:
                        break;

                    case 2:
                        break;

                    case 3:
                        break;

                    case 4:
                        break;

                    case 5:
                        break;

                    case 0:
                        break;
                }
            }while(scelta_statistiche!=0);
            break;

        case 0:
            printf("Uscita dal programma");
            break;

        default:
            printf("Opzione non valida");
            break;
    }
    }while(scelta!=0);
return 0; 
}