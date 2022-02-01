/*
 * Esercitazione 3 - Morra Cinese
 *
 * Scrivere un programma multi-thread che simuli il gioco della morra cinese.
 * In tale programma ci devono essere 3 thread:
 *      - 2 thread simulano i giocatori;
 *      - 1 thread simula l'arbitro.
*/

/* Opzioni di Gioco */
#define CARTA   0
#define SASSO   1
#define FORBICE 2

#define SEED 3101

char *nomi_mosse[3] = {"CARTA", "SASSO", "FORBICE"};
char *risultati[3] = {"PARI", "GIOCATORE 1", "GIOCATORE 2"};

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

/* Struct per la Gestione del Gioco */
struct morra_cinese_t{
    pthread_mutex_t mutex;
    pthread_cond_t attesa_via;              // Condition Variable per i Giocatori in Attesa del Via
    pthread_cond_t attesa_arbitro;          // Condition Variable per l'Arbitro in Attesa dei Giocatori
    pthread_cond_t attesa_risultati;        // Condition Variable per l'Arbitro in Attesa che entrambi i giocatori facciano la loro mossa
    pthread_cond_t attesa_nuova_partita;    // Condition Variable per i Giocatori in Attesa di una Nuova Partita

    int coda_giocatori;

    int prima_mossa;
    int seconda_mossa;
    int primo_giocatore;
    int secondo_giocatore;
    int vincitore;
} morra_cinese;

/* Date le diverse opzioni (mosse), restituisce il numero della Mossa Vincente */
int calcola_risultato(int prima_mossa, int seconda_mossa, struct morra_cinese_t *mc){
    // FORBICE batte CARTA
    if ((prima_mossa == CARTA && seconda_mossa == FORBICE) || (prima_mossa == FORBICE && seconda_mossa == CARTA)){
        if (prima_mossa == FORBICE) return mc->primo_giocatore;
        else return mc->secondo_giocatore;
    }
    // SASSO batte FORBICE
    if ((prima_mossa == SASSO && seconda_mossa == FORBICE) || (prima_mossa == FORBICE && seconda_mossa == SASSO)){
        if (prima_mossa == SASSO) return mc->primo_giocatore;
        else return mc->secondo_giocatore;
    }
    // CARTA batte SASSO
    if ((prima_mossa == SASSO && seconda_mossa == CARTA) || (prima_mossa == CARTA && seconda_mossa == SASSO)){
        if (prima_mossa == CARTA) return mc->primo_giocatore;
        else return mc->secondo_giocatore;
    }
    // STESSA MOSSA - Pari
    else {
        return 0;
    }
}

/* Resetta le Variabili Utili alla Gestione della Paritita */
void reset_partita(struct morra_cinese_t *mc){
    mc->coda_giocatori = 0;         // Inizializza la Coda Giocatori a 0

    mc->prima_mossa = -1;           // Inizilizza la prima mossa a -1 (non ancora scelta)
    mc->seconda_mossa = -1;         // Inizilizza la seconda mossa a -1 (non ancora scelta)
    mc->primo_giocatore = 0;        // Inizilizza il primo giocatore che effettua la prima mossa
    mc->secondo_giocatore = 0;      // Inizilizza il secondo giocatore che effettua la seconda mossa
    mc->vincitore = -1;             // Inizializza il Vincitore a -1
}

/* Inizializza il Gioco */
void init_morra_cinese(struct morra_cinese_t *mc){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutex_init(&mc->mutex, &mutex_attr);
    pthread_cond_init(&mc->attesa_via, &cond_attr);
    pthread_cond_init(&mc->attesa_arbitro, &cond_attr);
    pthread_cond_init(&mc->attesa_risultati, &cond_attr);
    pthread_cond_init(&mc->attesa_nuova_partita, &cond_attr);

    reset_partita(mc);
}

/*
    Da il via al gioco sbloccando i giocatori in attesa
*/
void via(struct morra_cinese_t *mc){
    pthread_mutex_lock(&mc->mutex);
    pthread_cond_broadcast(&mc->attesa_nuova_partita);

    while(mc->coda_giocatori < 2){
        printf("A : Attendo Giocatori\n");
        pthread_cond_wait(&mc->attesa_arbitro,&mc->mutex);
    }
    printf("A : Via!\n");
    pthread_cond_broadcast(&mc->attesa_via);

    pthread_mutex_unlock(&mc->mutex);
}

/* 
    Permette all'Arbitro di Aspettare le Mosse di entrambi i Giocatori 
*/
void aspetta_mosse(struct morra_cinese_t *mc){
    pthread_mutex_lock(&mc->mutex);

    while((mc->seconda_mossa == -1) || (mc->seconda_mossa == -1)){
        pthread_cond_wait(&mc->attesa_risultati, &mc->mutex);
    }

    pthread_mutex_unlock(&mc->mutex);
}

/* 
    Permette all'Arbitro di Annunciare il Vincitore
*/
void risultato_gioco(struct morra_cinese_t *mc){
    pthread_mutex_lock(&mc->mutex);

    int risultato = calcola_risultato(mc->prima_mossa, mc->seconda_mossa, mc);
    if (risultato == 0){
        printf("A : E' Patta!\n");
    }
    else{
        printf("A : Il vincitore e' il %s !\n", risultati[risultato]);
    }
    reset_partita(mc);

    pthread_mutex_unlock(&mc->mutex);
}

/*
    Permette ai Giocatori di Attendere il Via dell'Arbitro
*/
void aspetta_via(struct morra_cinese_t *mc, int numero_giocatore){
    pthread_mutex_lock(&mc->mutex);

    mc->coda_giocatori++;
    if(mc->coda_giocatori == 2){
        printf("G%d: Avviso l'Arbitro\n", numero_giocatore);
        pthread_cond_signal(&mc->attesa_arbitro);
    }
    printf("G%d: Pronto\n", numero_giocatore);
    pthread_cond_wait(&mc->attesa_via, &mc->mutex);

    pthread_mutex_unlock(&mc->mutex);
}

/*
    Permette ai Giocatori di Scegliere una Mossa in maniera pseudo-casuale e thread-safe 
*/
void scegli_mossa(struct morra_cinese_t *mc, int numero_giocatore){
    pthread_mutex_lock(&mc->mutex);

    srand((time(NULL)+numero_giocatore+SEED)^getpid());
    int choice = rand() % 3;
    if (mc->primo_giocatore == 0){
        mc->primo_giocatore = numero_giocatore;
        mc->prima_mossa = choice;
    }
    else {
        mc->secondo_giocatore = numero_giocatore;
        mc->seconda_mossa = choice;
        pthread_cond_signal(&mc->attesa_risultati);
    }
    printf("G%d: %s !\n", numero_giocatore, nomi_mosse[choice]);
    pthread_cond_wait(&mc->attesa_nuova_partita, &mc->mutex);

    pthread_mutex_unlock(&mc->mutex);
}

void *arbitro(){
    while(1){
        via(&morra_cinese);
        aspetta_mosse(&morra_cinese);
        risultato_gioco(&morra_cinese);
        printf("\n------------------\n\n");
        system("PAUSE");
    }
}

void *giocatore(void *arg){
    int numero_giocatore = (int) arg;
    while(1){
        aspetta_via(&morra_cinese, numero_giocatore);
        scegli_mossa(&morra_cinese, numero_giocatore);
    }
}

int main()
{
    pthread_t arbitro_t, giocatore1, giocatore2;
    pthread_attr_t p_attr;

    pthread_attr_init(&p_attr);

    /* Inizializzo il Gioco */
    init_morra_cinese(&morra_cinese);

    pthread_create(&arbitro_t, &p_attr, arbitro, NULL);
    pthread_create(&giocatore1, &p_attr, giocatore, (void *) 1);
    pthread_create(&giocatore2, &p_attr, giocatore, (void *) 2);

    /* Join di tutti i Thread */
    pthread_join(arbitro_t, NULL);
    pthread_join(giocatore1, NULL);
    pthread_join(giocatore2, NULL);

    pthread_attr_destroy(&p_attr);

    return 0;
}
