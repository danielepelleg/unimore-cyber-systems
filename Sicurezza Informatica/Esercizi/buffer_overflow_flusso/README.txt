Prima esercitazione su buffer overflow: modificare il flusso di esecuzione di un programma utilizzando stack overflow.

Il programma secret.c è vulnerabile a stack overflow (la funzione gets è insicura, e scrive il contenuto di standard input all'interno del buffer small senza verificarne la lunghezza).

1- compilare il sorgente secret.c

gcc -m32 -o secret secret.c

2- verificare il funzionamento dell'eseguibile lame

setarch i386 -R ./secret

3- causare un segfault introducendo un input troppo lungo

4- ottenere un core file 

in bash: 
ulimit -c unlimited
per altre shell, fate riferimento al manuale della shell che state utilizzando

ripetere il segmentation fault

5- caricare il core file in gdb e esaminare lo stato dei registri

gdb lame core

all'interno della console di gdb: info register

6- disassemblare la funzione main del programma lame per ottenere l'indirizzo di una delle funzioni

gdb secret
all'interno della console di gdb: disass main

7- utilizzare un programma esterno (ad esempio ret.c) per creare un buffer di dimensioni suffucienti a creare un segmentation fault. Riempire il buffer con l'indirizzo della funzione secret.

prendere spunto dal sorgente ret.c, scritto per invocare la funzione all'indirizzo della funzione desiderata.

8- utilizzate l'output prodotto da ret come input di secret, e verificatene il comportamento. (eventualmente, seguite passo passo l'esecuzione di lame utilizzando gdb)

./ret | setarch i386 -R ./secret
