/*
 * Esercitazione 5 - Negozio di Barbieri
 *
 * Un negozio di barbieri ha tre barbieri, e tre poltrone su cui siedono i clienti
 * quando vengono per tagliarsi la barba.
 *
 * C'è una sala d'aspetto con un divano (max 4 persone; gli altri aspettano fuori dalla porta).
 * C'e' un cassiere, che può servire solamente un cliente (con barba tagliata) alla volta.
 * Scrivere il processo cliente che cerca di entrare nel negozio per farsi tagliare la barba,
 *
 * RISORSE: - 3 Barbieri
 *          - 1 Cassiere
 *          - 1 Divano (4 posti)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

/* Macros */
#define N_BARBIERI      3
#define N_CASSIERI      1
#define N_POSTI_DIVANO  4
#define N_CLIENTI       8

/* Struct per la Gestione della Barberia */
struct barbiere_t {
    pthread_mutex_t mutex;
    pthread_cond_t attesa_fuori_negozio;    // Condition Variable per Clienti in Attesa Fuori dal Negozio
    pthread_cond_t attesa_divano;           // Condition Variable per Clienti in Attesa sul Divano
    pthread_cond_t attesa_cassiere;         // Condition Variable per Clienti in Attesa di Pagare

    int coda_fuori_negozio;                 // Contatore Coda Fuori dal Negozio
    int posti_divano;                       // Numero di Posti Disponibili sul Divano
    int barbieri;                           // Numero di Barbieri Disponibili
    int cassieri;                           // Numero di Cassieri Disponibili
} barbiere;

/* Inizializza la Barberia */
void init_barbiere(struct barbiere_t *b){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutex_init(&b->mutex, &mutex_attr);
    pthread_cond_init(&b->attesa_fuori_negozio, &cond_attr);
    pthread_cond_init(&b->attesa_divano, &cond_attr);
    pthread_cond_init(&b->attesa_cassiere, &cond_attr);

    b->coda_fuori_negozio = 0;
    b->posti_divano = N_POSTI_DIVANO;
    b->barbieri = N_BARBIERI;
    b->cassieri = N_CASSIERI;
}


/*
 * Permette a un Cliente di Entrare nella Barberia:
 *
 * Il cliente:
 *   - Si mette in coda fuori se non c'è posto dentro
 *   - Prende posto sul Divano quando non ci sono barbieri disponibili
 *   - Si fa tagliare la barba quando un barbiere è disponibile
 */
void entra_negozio(struct barbiere_t *b, int id_cliente){
    pthread_mutex_lock(&b->mutex);

    while(b->posti_divano == 0){
        printf("C%d: Attendo fuori!\n", id_cliente);
        b->coda_fuori_negozio++;
        pthread_cond_wait(&b->attesa_fuori_negozio, &b->mutex);
        b->coda_fuori_negozio--;
    }

    printf("C%d: Entro in Negozio!\n", id_cliente);
    while (b->posti_divano > 0 && b->barbieri == 0){
        b->posti_divano--;
        printf("C%d: Prendo posto sul divano!\n", id_cliente);
        pthread_cond_wait(&b->attesa_divano, &b->mutex);
        b->posti_divano++;
    }
    if (b->coda_fuori_negozio) pthread_cond_signal(&b->attesa_fuori_negozio);
    b->barbieri--;
    printf("C%d: Taglio la barba!\n", id_cliente);

    pthread_mutex_unlock(&b->mutex);
}

/* Libera il Barbiere notificando eventuali altri clienti in attesa */
void fine_taglio_barba(struct barbiere_t *b, int id_cliente){
    pthread_mutex_lock(&b->mutex);

    b->barbieri++;
    printf("C%d: Fatto!\n", id_cliente);
    pthread_cond_signal(&b->attesa_divano);

    pthread_mutex_unlock(&b->mutex);
}

/* Effettua il Pagamento alla Reception.
 * Se il commesso non è disponibile attende in coda.
 */
void inizio_pagamento(struct barbiere_t *b, int id_cliente){
    pthread_mutex_lock(&b->mutex);

    while(b->cassieri == 0){
        printf("C%d: In coda alla cassa!\n", id_cliente);
        pthread_cond_wait(&b->attesa_cassiere, &b->mutex);
    }
    b->cassieri--;
    printf("C%d: Sto pagando!\n", id_cliente);

    pthread_mutex_unlock(&b->mutex);
}

/* Libera il Commesso e notifica gli altri clienti in attesa di pagare */
void fine_pagamento(struct barbiere_t *b, int id_cliente){
    pthread_mutex_lock(&b->mutex);

    b->cassieri++;
    printf("C%d: Esco!\n", id_cliente);
    pthread_cond_signal(&b->attesa_cassiere);

    pthread_mutex_unlock(&b->mutex);
}

void *cliente(void *arg){
    int id_cliente = (int) arg;
    entra_negozio(&barbiere, id_cliente);
    //inizio_taglio_barba(&barbiere, id_cliente);
    sleep(3);   // Simulazione Taglio Barba
    fine_taglio_barba(&barbiere, id_cliente);
    inizio_pagamento(&barbiere, id_cliente);
    sleep(1);   // Simulazione Pagamento
    fine_pagamento(&barbiere, id_cliente);
    return 0;
}

int main()
{
    pthread_t *clienti_t;
    pthread_attr_t p_attr;

    pthread_attr_init(&p_attr);

    init_barbiere(&barbiere);

    int i;

    /*
     * Allocazione Array Thread
     */
    clienti_t=(pthread_t *) malloc(N_CLIENTI * sizeof(pthread_t));

    for (i = 1; i <= N_CLIENTI; i++){
        pthread_create(&clienti_t[i], &p_attr, cliente, (void*) i);
    }
    for (i = 1; i <= N_CLIENTI; i++){
        pthread_join(clienti_t[i], NULL);
    }

    pthread_attr_destroy(&p_attr);

    return 0;
}
