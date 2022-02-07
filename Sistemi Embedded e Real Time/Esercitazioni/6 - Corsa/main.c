/*
 * Simulazione d'Esame - La Corsa 02.02.2022
 *
 * In uno stadio ci sono N corridori pronti a partire sulla pista dei 100 mt. Al via dell'arbitro, 
 * tutti i corridori iniziano a correre. Dopo alcuni secondi tutti i corridori arrivano al traguardo. 
 * Quando l'ultimo corridore raggiunge il traguardo l'arbitro annuncia il primo e l'ultimo classificato.
 * 
 * Il sistema è modellato con un thread per ogni corridore, ed un thread per l'arbitro. 
 * Il corridore arriva al punto di partenza ed aspetta il via dell'arbitro. 
 * Quando l'arbitro da il via, il corridore corre ed arriva al traguardo. 
 * L'arbitro arriva ed attende che tutti i corridori si siano posizionati. 
 * Dopo di che da il via alla gara, e si mette in attesa dell'arrivo dell'ultimo corridore. 
 * Non appena l'ultimo corridore arriva, l'arbitro comunica il primo e l'ultimo classificato della gara.
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define N_CORRIDORI 5

/* Struct per la Gestione della Corsa */
struct corsa_t{
    pthread_mutex_t mutex;              // Mutex per la Gestione della Crsa
    pthread_cond_t attesa_del_via;      // Condition Variable per i Corridori in Attesa del Via
    pthread_cond_t attesa_arbitro;      // CV per l'Arbitro: prima in attesa dei corridori pronti, poi dell'arrivo dell'ultimo corridore

    int corridori_pronti;               // Numero di Corridori Pronti
    int corridori_arrivati;             // Numero di Corridori Arrivati al traguardo
    int primo_classificato;             // ID Primo Classificato
    int ultimo_classificato;            // ID Secondo Classificato
} corsa;

/* Inizializzazione Corsa */
void init_corsa(struct corsa_t *s){
    pthread_mutexattr_t mutex_attr;
    pthread_condattr_t cond_attr;

    pthread_mutexattr_init(&mutex_attr);
    pthread_condattr_init(&cond_attr);

    pthread_mutex_init(&s->mutex, &mutex_attr);
    pthread_cond_init(&s->attesa_del_via, &cond_attr);
    pthread_cond_init(&s->attesa_arbitro, &cond_attr);

    pthread_mutexattr_destroy(&mutex_attr);
    pthread_condattr_destroy(&cond_attr);

    s->corridori_pronti = 0;
    s->corridori_arrivati = 0;
    s->primo_classificato = -1;
    s->ultimo_classificato = -1;
}

/* Attende che tutti i Corridori siano pronti per partire */
void arbitro_attendicorridori(struct corsa_t *corsa){
    pthread_mutex_lock(&corsa->mutex);

    while(corsa->corridori_pronti != N_CORRIDORI){
        printf("A : Attendo Corridori!\n");
        pthread_cond_wait(&corsa->attesa_arbitro, &corsa->mutex);
    }

    pthread_mutex_unlock(&corsa->mutex);
}

/* Blocca i corridori mettendoli in attesa del "via" dell'Arbitro */
void corridore_attendivia(struct corsa_t *corsa, int numerocorridore){
    pthread_mutex_lock(&corsa->mutex);

    corsa->corridori_pronti++;
    if(corsa->corridori_pronti == N_CORRIDORI){
        printf("C%d: Avviso l'Arbitro!\n", numerocorridore);
        pthread_cond_signal(&corsa->attesa_arbitro);
    }
    printf("C%d: Pronto!\n", numerocorridore);
    pthread_cond_wait(&corsa->attesa_del_via, &corsa->mutex);
    printf("C%d: Corro!\n", numerocorridore);

    pthread_mutex_unlock(&corsa->mutex);
}

/* Da il via al gioco sbloccando i corridori pronti */
void arbitro_via(struct corsa_t *corsa){
    pthread_mutex_lock(&corsa->mutex);

    printf("A : VIA!\n");
    pthread_cond_broadcast(&corsa->attesa_del_via);   // <-- nella bella potrei aver erroneamente passato il mutex come parametro

    pthread_mutex_unlock(&corsa->mutex);
}

/* Permette al Corridore di tagliare il traguardo */
void corridore_arrivo(struct corsa_t *corsa, int numerocorridore){
    pthread_mutex_lock(&corsa->mutex);

    corsa->corridori_arrivati++;
    printf("C%d: Arrivato!\n", numerocorridore);

    // Caso Primo Classificato
    if(corsa->corridori_arrivati == 1){
        corsa->primo_classificato = numerocorridore;
    }
    // Caso Ultimo Classificato
    if (corsa->corridori_arrivati == N_CORRIDORI){
        corsa->ultimo_classificato = numerocorridore;
        printf("C%d: Avviso l'Arbitro!\n", numerocorridore);
        pthread_cond_signal(&corsa->attesa_arbitro);
    }

    pthread_mutex_unlock(&corsa->mutex);
}

/* Comunicazione del Risultato da parte dell'Arbitro */
void arbitro_risultato(struct corsa_t *corsa){
    pthread_mutex_lock(&corsa->mutex);

    // Attendi la fine del Gioco
    while(corsa->corridori_arrivati != N_CORRIDORI){
        printf("A : Attendo il risultato!");
        pthread_cond_wait(&corsa->attesa_arbitro, &corsa->mutex);  // <-- nella bella potrei aver dimenticato di aggiungere questa riga di codice, necessaria affinchè
                                                                   //              il thread arbitro rimanga in attesa della fine della competizione.
                                                                   //     Anche togliendo questa riga, l'esecuzione non da problemi perchè i corridori presentano una usleep() che consente loro
                                                                   // di accedere per primi al mutex, mentre il thread arbitro una sleep(). Togliendo quest'ultima sleep il problema risulterebbe evidente.
    }
    printf("A : Primo Classificato - Corridore %d! \n", corsa->primo_classificato);
    printf("A : Ultimo Classificato - Corridore %d! \n", corsa->ultimo_classificato);

    pthread_mutex_unlock(&corsa->mutex);
}

/* Azioni del Corridore */
void *corridore(void *arg){
    int numerogiocatore = (int) arg;
    sleep(2);       // vado sulla pista
    corridore_attendivia(&corsa, numerogiocatore);
    usleep(100);    // corro più veloce possibile
    corridore_arrivo(&corsa, numerogiocatore);
    sleep(2);
    return 0;       // torno a casa
}

/* Azioni dell'Arbitro */
void *arbitro(){
    sleep(1);       // vado sulla pista
    arbitro_attendicorridori(&corsa);
    sleep(2);       // pronti, attenti
    arbitro_via(&corsa);
    sleep(1);       // attendo che arrivino al termine
    arbitro_risultato(&corsa);
    return 0;
}

int main()
{
    pthread_t arbitro_t, *corridori_t;

    pthread_attr_t p_attr;
    pthread_attr_init(&p_attr);

    init_corsa(&corsa);

    /* Allocazione Array Thread Corridori */
    corridori_t=(pthread_t *) malloc(N_CORRIDORI * sizeof(pthread_t));

    // Creazione dei Thread Corridori
    int i;
    for(i=1; i <= N_CORRIDORI; i++){
        pthread_create(&corridori_t[i], &p_attr, corridore, (void *) i);
    }
    // Creazione dell'Arbitro
    pthread_create(&arbitro_t, &p_attr, arbitro, NULL);

    /* Join di tutti i Thread */
    for (i = 1; i <= N_CORRIDORI; i++){
        pthread_join(corridori_t[i], NULL);
    }
    pthread_join(arbitro_t, NULL);

    pthread_attr_destroy(&p_attr);

    return 0;
}
