/*
 * Esercitazione 4 - Urna
 *
 * Ogni thread:
 *      - esprime un voto, che puo' essere 0 o 1, invocando la funzione vota(), la quale registra il voto in una
 *          struttura dati condivisa che per comodita' chiameremo "urna";
 *      - aspetta l'esito della votazione invocando la funzione risultato(), la quale controlla l'urna e ritorna
 *          0 o 1 a seconda che ci sia una maggioranza di voti 0 oppure di voti 1.
 *      - se l'esito della votazione e' uguale al proprio voto, stampa a video la stringa "Ho vinto", altrimenti
 *          stampa la stringa "Ho perso";
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define SEED 126

/* Struct per la Gestione dell'Urna */
struct urna_t {
    pthread_mutex_t mutex;
    pthread_cond_t aspetta_esito;   // Condition Variable per i Thread che Aspettano l'Esito

    int coda_threads;               // Numero di Thread in Coda

    int voto_t1;
    int voto_t2;
    int voto_t3;
} urna;

/* Inizializzazione Urna */
void init_urna(struct urna_t *u){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutex_init(&u->mutex, &mutex_attr);
    pthread_cond_init(&u->aspetta_esito, &cond_attr);

    u->coda_threads = 0;            // Inizilizzo il Numero di Thread in Coda a 0
    u->voto_t1 = -1;                // Inizilizzo il Voto del primo Thread a -1 (non ancora votato)
    u->voto_t2 = -1;                // Inizilizzo il Voto del secondo Thread a -1 (non ancora votato)
    u->voto_t3 = -1;                // Inizilizzo il Voto del terzo Thread a -1 (non ancora votato)
}

/* Permette al thread di Votare */
void vota(struct urna_t *u, int v, int thread_id){
    pthread_mutex_lock(&u->mutex);

    if (u->voto_t1 == -1){
        u->voto_t1 = v;
    }
    else if (u->voto_t2 == -1){
        u->voto_t2 = v;
    }
    else u->voto_t3 = v;
    printf("T%d: Voto %d!\n", thread_id, v);

    u->coda_threads++;
    if (u->coda_threads < 3) pthread_cond_wait(&u->aspetta_esito, &u->mutex);
    else pthread_cond_broadcast(&u->aspetta_esito);

    pthread_mutex_unlock(&u->mutex);
}

/* Ritorna 0 se hanno vinto gli 0, 1 altrimenti */
int risultato(struct urna_t *u){
    if((u->voto_t1 + u->voto_t2 + u->voto_t3) < 2) return 0;
    else return 1;
}

void *thread(void* arg){
    int thread_id = (int) arg;
    srand((time(NULL)+thread_id+SEED)^getpid());
    int voto = rand() % 2;
    vota(&urna, voto, thread_id);
    if (voto == risultato(&urna)) printf("T%d: Ho vinto!\n", thread_id);
    else printf("T%d: Ho perso!\n", thread_id);

    return 0;
}

int main()
{
    pthread_t t_1, t_2, t_3;
    pthread_attr_t p_attr;

    pthread_attr_init(&p_attr);

    /* Inizializzo il Gioco */
    init_urna(&urna);

    pthread_create(&t_1, &p_attr, thread, (void *) 1);
    pthread_create(&t_2, &p_attr, thread, (void *) 2);
    pthread_create(&t_3, &p_attr, thread, (void *) 3);

    /* Join di tutti i Thread */
    pthread_join(t_1, NULL);
    pthread_join(t_2, NULL);
    pthread_join(t_3, NULL);

    pthread_attr_destroy(&p_attr);

    return 0;
}
