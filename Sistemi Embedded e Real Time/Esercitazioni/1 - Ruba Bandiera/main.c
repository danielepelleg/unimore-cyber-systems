/*
    Simulazione d'Esame - 12.01.2022

    Il sistema modella un turno del gioco delle bandierine. Ci sono due giocatori che al "via"
    devono tentare di prendere la bandierina tenuta dal giudice. Il primo che riesce a prendere
    la bandierina deve ritornare alla base, mentre l'altro deve cercare di raggiungerlo.

    Il Sistema consiste in 3 thread: 1 per il Giudice, ed 1 per Ciascun Giocatore.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define NUMERO_GIOCATORI    2

/* Stati del Sistema */
#define IN_PREPARAZIONE     0
#define VIA                 1
#define FLAG_PRESA          2
#define FINE                3

/* Struct per la Gestione del Gioco Ruba Bandiera */
struct bandierine_t{
    pthread_mutex_t mutex;              // Semaforo per Gestione del Gioco
    pthread_cond_t attesa_del_via;      // Condition Variable per i Giocatori in Attesa del Via
    pthread_cond_t attesa_giocatori;    // Condition Variable per il Giudice in Attesa dei Giocatori
    pthread_cond_t attesa_fine;         // Condition Variable per il Giudice in Attesa della Fine

    int giocatori_in_attesa;            // Numero di Giocatori in Attesa

    int stato;                          // Stato del Sistema
    int giocatore_in_vantaggio;         // ID del Giocatore in Vantaggio
} bandierine;

/* Inizializza il Gioco */
void init_bandierine(struct bandierine_t *t){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutex_init(&t->mutex, &mutex_attr);
    pthread_cond_init(&t->attesa_del_via, &cond_attr);
    pthread_cond_init(&t->attesa_giocatori, &cond_attr);
    pthread_cond_init(&t->attesa_fine, &cond_attr);

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    t->giocatori_in_attesa = 0;         // Inizializza il Numero di Giocatori in Attesa a 0
    t->stato = IN_PREPARAZIONE;         // Inizializza lo Stato del Sistema
    t->giocatore_in_vantaggio = -1;     // Inizializza il Giocatore in Vantaggio a -1 (Nessun Giocatore in Vantaggio)
}

/*
    Da il via al gioco sbloccando i giocatori in attesa
*/
void via(struct bandierine_t *b){
    pthread_mutex_lock(&b->mutex);

    b->stato = VIA;
    printf("G: VIA!\n");
    pthread_cond_broadcast(&b->attesa_del_via);

    // Attendi la Fine del Gioco
    while(b->stato != FINE){
        pthread_cond_wait(&b->attesa_fine, &b->mutex);
    }

    pthread_mutex_unlock(&b->mutex);
}

/*
    Blocca i giocatori che attendono il via
*/
void attendi_il_via(struct bandierine_t *b, int n){
    pthread_mutex_lock(&b->mutex);

    b->giocatori_in_attesa++;
    if (b->giocatori_in_attesa == 2){
        printf("P%d: Avviso il Giudice!\n", n);
        pthread_cond_signal(&b->attesa_giocatori);
    }
    printf("P%d: Pronto!\n", n);
    pthread_cond_wait(&b->attesa_del_via, &b->mutex);
    printf("P%d: Corro!\n", n);

    pthread_mutex_unlock(&b->mutex);
}

/*
    Prende la bandierina (se disponibile).
        1 -> Se il Giocatore prende la bandierina
        0 -> Se il Giocatore NON prende la bandierina
*/
int bandierina_presa(struct bandierine_t *b, int n){
    int retval = 0;
    pthread_mutex_lock(&b->mutex);

    if (b->stato == VIA) {
        printf("P%d: Presa!\n", n);
        b->stato = FLAG_PRESA;
        b->giocatore_in_vantaggio = n;
        retval = 1;
    }

    pthread_mutex_unlock(&b->mutex);
    return retval;
}

/*
    1 -> Se il Giocatore NON è stato preso1
    0 -> Se il Giocatore è stato preso
*/
int sono_salvo(struct bandierine_t *b, int n){
    int retval = 0;
    pthread_mutex_lock(&b->mutex);

    // Se hai la bandierina corri alla base
    if(b->stato == FLAG_PRESA && b->giocatore_in_vantaggio == n){
        b->stato = FINE;
        pthread_cond_signal(&b->attesa_fine);
        retval = 1;
    }

    pthread_mutex_unlock(&b->mutex);
    return retval;
}

/*
    1 -> Se il Giocatore NON si è ancora salvato
    0 -> Se il Giocatore si è già salvato
*/
int ti_ho_preso(struct bandierine_t *b, int n){
    int retval = 0;
    pthread_mutex_lock(&b->mutex);

    if (b->stato == FLAG_PRESA && b->giocatore_in_vantaggio != n){
        b->giocatore_in_vantaggio = n;
        b->stato = FINE;
        pthread_cond_signal(&b->attesa_fine);
        retval = 1;
    }

    pthread_mutex_unlock(&b->mutex);
    return retval;
}

/*
    Ritorna il Numero del Vincitore
*/
int risultato_gioco(struct bandierine_t *b){
    pthread_mutex_lock(&b->mutex);

    return b->giocatore_in_vantaggio;

    pthread_mutex_unlock(&b->mutex);
}

/*
    Attende che entrambi i giocatori siano pronti in attesa prima di dare il via
*/
void attendi_giocatori(struct bandierine_t *b){
    pthread_mutex_lock(&b->mutex);

    while(b->giocatori_in_attesa != 2){
        printf("J: Attendo Giocatori\n", b->giocatori_in_attesa);
        pthread_cond_wait(&b->attesa_giocatori, &b->mutex);
    }

    pthread_mutex_unlock(&b->mutex);
}

void *giocatore(void *arg){
    int numero_giocatore = (int) arg;

    attendi_il_via(&bandierine, numero_giocatore);

    sleep(2);       // corri e tenta di prendere la bandierina

    if (bandierina_presa(&bandierine, numero_giocatore)){
        sleep(2);   // corri alla base
        if (sono_salvo(&bandierine, numero_giocatore)) printf("P%d: Sono Salvo!\n", numero_giocatore);
    }
    else {
        sleep(2); // cerca di acchiappare l'altro giocatore
        if (ti_ho_preso(&bandierine, numero_giocatore)) printf("P%d: Ti ho preso!\n", numero_giocatore);
    }
    return 0;
}

void *giudice(void *arg){
    attendi_giocatori(&bandierine);
    usleep(10);        // pronti, attenti
    via(&bandierine);
    printf("G: Il Vincitore e' il Giocatore %d!\n\n", risultato_gioco(&bandierine));
    return 0;
}

int main(){
    pthread_t player1, player2, judge;
    pthread_attr_t p_attr;

    pthread_attr_init(&p_attr);

    /* Inizializzo il Gioco */
    init_bandierine(&bandierine);

    pthread_create(&player1, &p_attr, giocatore, (void *) 1);
    //printf("Creato il Player1\n");
    pthread_create(&player2, &p_attr, giocatore, (void *) 2);
    //printf("Creato il Player2\n");
    pthread_create(&judge, &p_attr, giudice, NULL);
    //printf("Creato il Giudice\n");

    /* Join di tutti i Thread */
    pthread_join(player1, NULL);
    pthread_join(player2, NULL);
    pthread_join(judge, NULL);

    pthread_attr_destroy(&p_attr);
    return 0;
}
