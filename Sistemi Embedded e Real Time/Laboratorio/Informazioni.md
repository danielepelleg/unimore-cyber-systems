- [Introduzione](#introduzione)
  - [Architettura Linux Semplificata](#architettura-linux-semplificata)
    - [Kernel Linux](#kernel-linux)
  - [Board Support Package - BSP](#board-support-package---bsp)
  - [Ambiente di Sviluppo](#ambiente-di-sviluppo)
  - [Host and Target](#host-and-target)
  - [Distribuzioni Linux](#distribuzioni-linux)
  - [Patch](#patch)
- [Toolchain](#toolchain)
  - [Macchine](#macchine)
  - [Componenti](#componenti)
  - [Emulatore vs Virtualizzazione](#emulatore-vs-virtualizzazione)
- [Device Tree](#device-tree)

# Introduzione
I sistemi embedded sono piattaforme di dimensioni contenuti inserite all'interno di un sistemo più complesso. A livello di *potenza di calcolo* una piattaforma vale l'altra: ciò che cambia è ciò che circonda il chip. Strumenti come *Chromecast*, *Alexa*, sono tutti sistemi che hanno al loro interno piattaforme di questo tipo. La necessità di un hardware così ridotto sta nei consumi altrettanto ridotti. L'idea che una piattaforma piccola sia meno potente non corrisponde sempre alla realtà: queste sono piattaforma studiate per essere utilizzati in questi contesti.

## Architettura Linux Semplificata
- `Firmware`: software che esegue a stretto contatto con l'hardware
- `Bootloader`: software che permette l'avvio di altri sistemi
- `Applicazioni`: applicazioni web, web server, applicazioni con interfaccia grafica

Su queste board girano aalgortimi in grado di utilizzarela gpu per velocizzare i calcoli. Sono utilizzati in molteplici ambiti e talvolta sono molto più potenti di quelli che si trovano sui normali computer.

### Kernel Linux
Linux non è un sistema operativo ma è un *kernel*. Un kernel è quel componente che gestisce le risorse hardware: CPU, memoria, I/O. Comprende un set di API per consentire alle applicazioni e alle librerie utente di interfacciarsi con l'hardware. Linux è molto comodo perchè molte delle cose di cui si ha bisogno sono già state fatte da altri. Linux supporta diverse architetture: la più utilizzata è quella di ARM.

Il driver hanno il compito di interfacciare il software direttamente con l'hardware.

## Board Support Package - BSP
Contiene tutto il necessario per eseguire un'istanza di Linux su una determinata piattaforma. Deve essere creato tutte le volte che si mette linux su una piattaforma. Tutto il lavoro che si fa su una piattforma si deve poter installare in maniera semplice. Elemento fondamentale in ambito embedded.

## Ambiente di Sviluppo
Si può scegliere di utilizzare un sistema proprietario oppure un sistema open source. Nel nostro caso si opterà per un sistema open source: ***Yocto***. Si possono generare BSP e applicazioni per tutte le piattaforme che supporta Yocto.

## Host and Target
L'host è il portatile, la workstation, dove vengono installati il compilatore, i debuggers e gli SDK tools. Il target è generalmente la board. Solitamente i sistemi sono collegati mediante cavi chiamati *seriali*.

## Distribuzioni Linux
Ubuntu è da sempre la più usata. Non lavorare mai utilizzando l'ultima versione. All'interno di linux non esiste codice C++. Linux è stato implementato in C come tutti gli altri sistemi UNIX. Tutto il codice è compilato con il compilatore `gcc`. All'interno del codice del kernel linux non esistono numeri con la virgola, perchè linux è stato pensato per supportare diverse architetture, che non sempre supportano operazioni a virgola mobile. Alcuni componenti hardware non rispettano determinati standard, pertanto linux va loro incontro in questo modo.

## Patch
Con il termine si fa rifermento ad un *metodo* che i sistemi utilizzano per aggiungere o rimuovere del codice da un determinato sorgente.

# Toolchain
La toolchain è un insieme di strumenti di sviluppo. Il crosscompilatore è un elemento della *toolchain*. Per le differenze tra la macchina di sviluppo e la macchina target (in cui il codice andrà in esecuzione), che possono avere architetture differenti (set di istruzioni disponibili sulla macchina), si utilizza una *cross-compiling toolchain*

## Macchine
La macchina di *build* è quella dove viene compilata la toolchain. La macchina di host è quella sulla quale la toolchain viene eseguita, mentre quella target è quella in cui vengono eseguiti i binari.

## Componenti
Il Kernel Headers contiene tutti i file *.h* di Linux. Il codice che si va a cross compilare può essere di qualsiasi tipo. Vado a prendere gli headers della toolchain perchè la versione del Kernel utilizzata va a braccetto con la toolchain.

## Emulatore vs Virtualizzazione
L'emulatore è un software scritto per *emulare* un componente hardware.

# Device Tree
Strumento utilizzato per informare Linux riguardo le *caratteristiche hardware* del sistema che girerà. Serve a Linux per inizializzare solo le risorse che servono al nostro sistema. Si tratta di un file (una lista) di componenti hardware, in una forma più o meno complessa (viene specificato il voltaggio di un componente o semplicemente il suo seriale). Viene utilizzato durante la *fase di Boot*, viene caricato da U-Boot e *utilizzato da Linux*. E' possibile scegliere cosa far vedere a Linux. Tra le varie cartelle di Linux ce n'è una per architettura che definisce diversi Device Tree: ciascun Device Tree è scritto *per board*. Il Device Tree nasce dall'esigenza di una *dinamicità nella configurazione di Linux*: le board cambiano spesso, pertanto è spesso necessario cambiare un Device Tree anche solo quando si aggiunge un dispositivo.

Il codice di un Device Tree si compila, perchè è un binario e a seconda del compilatore viene organizzato in maniera diversa. Non è possibile cross-compilarlo.