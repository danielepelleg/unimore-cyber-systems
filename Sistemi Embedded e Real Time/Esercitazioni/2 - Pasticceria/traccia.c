/*
 * Simulazione d'Esame - La Pasticceria 08.06.2006
 *
 * Una pasticceria produce e vende al dettaglio delle torte. La pasticceria è gestita
 * da un cuoco, che cucina le torte con arte, e da un commesso, che prende le torte prodotte,
 * le incarta e le vende al dettaglio. Il pasticcere evita di iniziare a produrre torte se in
 * pasticceria ci sono più di N torte invendute. I clienti acquistano una torta alla volta.
 * La vendita di una torta da parte del commesso coincide con l'acquisto da parte di un cliente.
 *
 * Il sistema è modellator tramite un thread per il cuoco, uno per il commesso, e uno per ogni cliente.
 * Il nuimero di clienti non è specificato, e non è importante il loro ordine di accodamento.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define N 5 // Numero di Torte Invendute oltre il quale il Cuoco non inizia a produrne altre

struct pasticceria_t{
    // code
} pasticceria;

void init_pasticceria(struct pasticceria_t *p){

};

void cuoco_inizio_torta(struct pasticceria_t *p){

};

void cuoco_fine_torta(struct pasticceria_t *p){

};

void commesso_prendo_torta(struct pasticceria_t *p){

};

void commesso_vendo_torta(struct pasticceria_t *p){

};

void cliente_acquisto(struct pasticceria_t *p){

};

void *cuoco(void *arg){
    while(1){
        cuoco_inizio_torta(&pasticceria);           // 1 torta, può essere bloccante
        // preparo torta
        cuoco_fine_torta(&pasticceria);             // 1 torta, non bloccante
    }
}

void *commesso(void *arg){
    while(1){
        commesso_prendo_torta(&pasticceria);        // 1 torta; bloccante
        // incarto la torta
        commesso_vendo_torta(&pasticceria);         // 1 torta; bloccante
    }
}

void *un_cliente(void *arg){
    while(1){
        // vado in pasticceria
        cliente_acquisto(&pasticceria);             // 1 torta; bloccante
        // torno a casa e la mangio
    }
}

int main(){
    
}
