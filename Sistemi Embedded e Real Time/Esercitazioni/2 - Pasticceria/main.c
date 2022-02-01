/*
 * Simulazione d'Esame - La Pasticceria 08.06.2006
 *
 * Una pasticceria produce e vende al dettaglio delle torte. La pasticceria è gestita
 * da un cuoco, che cucina le torte con arte, e da un commesso, che prende le torte prodotte,
 * le incarta e le vende al dettaglio. Il pasticcere evita di iniziare a produrre torte se in
 * pasticceria ci sono più di N torte invendute. I clienti acquistano una torta alla volta.
 * La vendita di una torta da parte del commesso coincide con l'acquisto da parte di un cliente.
 *
 * Il sistema è modellato tramite un thread per il cuoco, uno per il commesso, e uno per ogni cliente.
 * Il numero di clienti non è specificato, e non è importante il loro ordine di accodamento.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 5             // MAX. Numero di Torte Invendute Disponibili
#define N_TORTE   5     // Numero di Torte Inizialmente Disponibili
#define N_CLIENTI 3     // Numero di Clienti nel Negozio

/* Stati del Sistema */
#define DEFAULT                     0
#define COMMESSO_ATTENDE_CLIENTE    1
#define COMMESSO_ATTENDE_TORTA      2
#define CLIENTE_ATTENDE_SERVIZIO    3

struct pasticceria_t{
    pthread_mutex_t mutex;
    pthread_cond_t attesa_produzione;   // Condition Variable per il Cuoco che attende per cominciare la preparazione della torta
    pthread_cond_t attesa_clienti;      // Condition Variable per il Commesso che attende un Cliente
    pthread_cond_t attesa_lavoro;       // Condition Variable per il Commesso che attende una torta
    pthread_cond_t attesa_servizio;     // Condition Variable per il Cliente in Attesa di essere servito
    pthread_cond_t coda_clienti;        // Condition Variable per i Clienti in Coda

    int torte_disponibili;              // Numero di Torte
    int torte_vendute;                  // Numero di Torte Vendute
    int servendo_cliente;               // 1 se si sta servendo un Cliente
    int stato;
} pasticceria;

/* Inizializza la Pasticceria */
void init_pasticceria(struct pasticceria_t *p){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutex_init(&p->mutex, &mutex_attr);
    pthread_cond_init(&p->attesa_produzione, &cond_attr);
    pthread_cond_init(&p->attesa_clienti, &cond_attr);
    pthread_cond_init(&p->attesa_lavoro, &cond_attr);
    pthread_cond_init(&p->attesa_servizio, &cond_attr);
    pthread_cond_init(&p->coda_clienti, &cond_attr);

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    p->torte_disponibili = N_TORTE;    // Inizializza il Numero di Torte Disponibili
    p->torte_vendute = 0;              // Inizilizza il Numero di Torte Vendute
    p->servendo_cliente = 0;           // Inizilizza lo Stato di Servizio di un Cliente
    p->stato = DEFAULT;
};

/**
 * @brief cuoco_inizio_torta
 * @param p
 *
 * Inizia la Preparazione di una Torta (può essere bloccante)
 */
void cuoco_inizio_torta(struct pasticceria_t *p){
    pthread_mutex_lock(&p->mutex);

    while (p->torte_disponibili == N){
        pthread_cond_wait(&p->attesa_produzione, &p->mutex);
    }
    printf("CUOCO\t: Inizio Preparazione Torta!\n");

    pthread_mutex_unlock(&p->mutex);
};

/**
 * @brief cuoco_fine_torta
 * @param p
 *
 * Finisce la Preparazione della Torta.
 */
void cuoco_fine_torta(struct pasticceria_t *p){
    pthread_mutex_lock(&p->mutex);

    p->torte_disponibili++;
    printf("CUOCO\t: Torta Pronta! Torte disponibili:%d\n", p->torte_disponibili);
    if(p->stato == COMMESSO_ATTENDE_TORTA){
        printf("CUOCO\t: Commesso, vieni!\n");
        pthread_cond_signal(&p->attesa_lavoro);
    }

    pthread_mutex_unlock(&p->mutex);
};

/*
 * @brief commesso_prendo_torta
 * @param p
 *
 * Permette al Commesso di Prendere una Torta.
 */
void commesso_prendo_torta(struct pasticceria_t *p){
    pthread_mutex_lock(&p->mutex);

    while(p->stato == DEFAULT){
        printf("COMMESSO: Attendo un cliente!\n");
        p->stato = COMMESSO_ATTENDE_CLIENTE;
        pthread_cond_wait(&p->attesa_clienti, &p->mutex);
    }

    while(p->torte_disponibili == 0){
        printf("COMMESSO: Attendo una torta!\n");
        p->stato = COMMESSO_ATTENDE_TORTA;
        pthread_cond_wait(&p->attesa_lavoro, &p->mutex);
    }
    p->stato = DEFAULT;

    pthread_mutex_unlock(&p->mutex);
};

/*
 * @brief commesso_vendo_torta
 * @param p
 *
 * Permette al Commesso di Vendere una Torta.
*/
void commesso_vendo_torta(struct pasticceria_t *p){
    pthread_mutex_lock(&p->mutex);

    p->torte_disponibili--;
    p->torte_vendute++;
    printf("COMMESSO: Torta venduta! Torte Disponibili:%d\n", p->torte_disponibili);
    pthread_cond_signal(&p->attesa_servizio);
    pthread_cond_wait(&p->attesa_clienti, &p->mutex);

    pthread_mutex_unlock(&p->mutex);
};

/*
 * @brief cliente_acquisto
 * @param p
 *
 * Permette al Cliente di Acquistare una Torta.
*/
void cliente_acquisto(struct pasticceria_t *p, int id_cliente){
    pthread_mutex_lock(&p->mutex);

    while(p->servendo_cliente == 1){
        printf("CLIENTE%d: In Coda\n", id_cliente);
        pthread_cond_wait(&p->coda_clienti, &p->mutex);
    }

    p->stato = CLIENTE_ATTENDE_SERVIZIO;
    while(p->stato == CLIENTE_ATTENDE_SERVIZIO){
        p->servendo_cliente = 1;
        printf("CLIENTE%d: Buongiorno, vorrei una Torta!\n", id_cliente);
        pthread_cond_signal(&p->attesa_clienti);
        pthread_cond_wait(&p->attesa_servizio, &p->mutex);
    }

    printf("CLIENTE%d: Grazie delle Torta!\n", id_cliente);
    if (p->torte_disponibili < N){
        p->servendo_cliente = 0;
        pthread_cond_signal(&p->attesa_produzione);
        pthread_cond_signal(&p->coda_clienti);
    }

    pthread_mutex_unlock(&p->mutex);
};

void *cuoco(void *arg){
    while(1){
        cuoco_inizio_torta(&pasticceria);               // 1 torta, può essere bloccante
        sleep(2);   // preparo torta
        cuoco_fine_torta(&pasticceria);                 // 1 torta, non bloccante
    }
}

void *commesso(void *arg){
    while(1){
        commesso_prendo_torta(&pasticceria);            // 1 torta; bloccante
        sleep(1);
        commesso_vendo_torta(&pasticceria);             // 1 torta; bloccante
    }
}

void *un_cliente(void *arg){
    int id_cliente = (int) arg;
    while(1){
        sleep(1); // vado in pasticceria
        cliente_acquisto(&pasticceria, id_cliente);     // 1 torta; bloccante
        sleep(1); // torno a casa e la mangio
    }
}

int main() {
    pthread_t cuoco_t, commesso_t;
    pthread_t *clienti_t;

    pthread_attr_t p_attr;

    pthread_attr_init(&p_attr);

    /* I Thread finiscono Insieme */
    pthread_attr_setdetachstate(&p_attr, PTHREAD_CREATE_DETACHED);

    /* Inizializzo la Pasticceria */
    init_pasticceria(&pasticceria);


    pthread_create(&cuoco_t, &p_attr, cuoco, NULL);
    //printf("Creato il Cuoco\n");
    pthread_create(&commesso_t, &p_attr, commesso, NULL);
    //printf("Creato il Commesso\n");

    /*
     * Allocazione Array Thread
     */
    clienti_t=(pthread_t *) malloc(N_CLIENTI * sizeof(pthread_t));
    int i;
    for (i = 1; i <= N_CLIENTI; i++){
        pthread_create(&clienti_t[i], &p_attr, un_cliente, (void*) i);
        //printf("Creato il Cliente%d\n", i);
    }

    /* aspetto 10 secondi prima di terminare tutti quanti */
    sleep(15);

    pthread_attr_destroy(&p_attr);

    printf("--- Torte Vendute: %d ---", pasticceria.torte_vendute);
    return 0;
}
