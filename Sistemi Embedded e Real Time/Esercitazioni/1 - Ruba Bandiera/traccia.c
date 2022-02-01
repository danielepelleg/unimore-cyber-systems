/*
    Simulazione d'Esame - 12.01.2022

    Il sistema modella un turno del gioco delle bandierine. Ci sono due giocatori che al "via"
    devono tentare di prendere la bandierina tenuta dal giudice. Il primo che riesce a prendere
    la bandierina deve ritornare alla base, mentre l'altro deve cercare di raggiungerlo.
    
    Il Sistema consiste in 3 thread: 1 per il Giudice, ed 1 per Ciascun Giocatore.

*/

struct bandierine_t{
    //...
} bandierine;

void init_bandierine(struct bandierine_t *t){
    /* data */
}

/*
    Da il via al gioco sbloccando i giocatori in attesa
*/
void via(struct bandierine_t *b){

}

/*
    Blocca i giocatori che attendono il via
*/
void attendi_il_via(struct bandierine_t *b, int n){

}

/*
    Prende la bandierina (se disponibile).
        1 -> Se il Giocatore prende la bandierina
        0 -> Se il Giocatore NON prende la bandierina
*/
int bandierina_presa(struct bandierine_t *b, int n){

}

/*
    1 -> Se il Giocatore NON è stato preso
    0 -> Se il Giocatore è stato preso
*/
int sono_salvo(struct bandierine_t *b, int n){

}

/*
    1 -> Se il Giocatore NON si è ancora salvato
    0 -> Se il Giocatore si è già salvato
*/
int ti_ho_preso(struct bandierine_t *b, int n){

}

/*
    Ritorna il Numero del Vincitore
*/
int risultato_gioco(struct bandierine_t *b){

}

/* 
    Attende che entrambi i giocatori siano pronti in attesa prima di dare il via
*/
void attendi_giocatori(struct bandierine_t *b){

}



void *giocatore(void *arg){
    int numero_giocatore = (int) arg;
    attendi_il_via(&bandierine, numero_giocatore);
    // corri e tenta di prendere la bandierina
    if (bandierina_presa(&bandierine, numero_giocatore)){
        // corri alla base
        if (sono_salvo(&bandierine, numero_giocatore)) printf("P%d: Sono Salvo!");
        else {
            // cerca di acchiappare l'altro giocatore
            if (ti_ho_preso(&bandierine, numero_giocatore)) printf("P%d: Ti ho preso!");
        }
        return 0;
    }
}

void *giudice(void *arg){
    attendi_giocatori(&bandierine);
    // pronti, attenti
    via(&bandierine);
    printf("G: Il Vincitore è %d:", risultato_gioco(&bandierine));
    return 0;
}