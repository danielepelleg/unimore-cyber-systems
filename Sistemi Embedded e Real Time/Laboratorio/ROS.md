- [Introduzione](#introduzione)

# Architettura
Vogliamo scrivere applicazioni lato utente utilizzando ROS. API di programmazione in C++, utilizzeremo i costrutti di sincronizzazione senza entrare nel dettaglio. I nodi in ROS sono i punti in cui il codice (processi) viene eseguito. Non c'è un concetto di memoria condivisa, pertanto non bisogna gestire gli accessi alla memoria. Il ROS master coordina il tutto: è un entità presente soltanto a runtime, è necessario che sia eseguito in background per far partire qualsiasi applicazione.

## Configurazione Ideale
- Driver che presndono dati dai sensori e li mettono in digitale, passandoli al programma che si occupa della configurazione

Il codice da scrivere in ROS si trova all'interno dei nodi. Si tratta solitamente di leggere i dati dal sensore, elaborare i dati e produrre un output.