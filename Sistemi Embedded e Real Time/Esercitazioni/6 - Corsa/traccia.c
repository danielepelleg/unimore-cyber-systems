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
    // code
} corsa;

/* Inizializzazione Corsa */
void init_corsa(struct corsa_t *s){
    // code
}

/* Blocca i corridori mettendoli in attesa del "via" dell'Arbitro */
void corridore_attendivia(struct corsa_t *corsa, int numerocorridore){
    // code
}

/* Permette al Corridore di tagliare il traguardo */
void corridore_arrivo(struct corsa_t *corsa, int numerocorridore){
    // code
}

/* Attende che tutti i Corridori siano pronti per partire */
void arbitro_attendicorridori(struct corsa_t *corsa){
    // code
}

/* Da il via al gioco sbloccando i corridori pronti */
void arbitro_via(struct corsa_t *corsa){
    // code
}

/* Comunicazione del Risultato da parte dell'Arbitro */
void arbitro_risultato(struct corsa_t *corsa){
    // code
}

/* Azioni del Corridore */
void *corridore(void *arg){
    int numerogiocatore = (int) arg;
    // vado sulla pista
    corridore_attendivia(&corsa, numerogiocatore);
    // corro più veloce possibile
    corridore_arrivo(&corsa, numerogiocatore);
    // torno a casa
}

/* Azioni dell'Arbitro */
void *arbitro(){
    // vado sulla pista
    arbitro_attendicorridori(&corsa);
    // pronti, attenti
    arbitro_via(&corsa);
    // attendo che arrivino al termine
    arbitro_risultato(&corsa);
    return 0;
}

