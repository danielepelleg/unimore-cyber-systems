- [Introduzione](#introduzione)
  - [Architettura Linux Semplificata](#architettura-linux-semplificata)
    - [Kernel Linux](#kernel-linux)
  - [Board Support Package - BSP](#board-support-package---bsp)
  - [Ambiente di Sviluppo](#ambiente-di-sviluppo)
  - [Host and Target](#host-and-target)
  - [Distribuzioni Linux](#distribuzioni-linux)
  - [Patch](#patch)

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
L'host è il portatile, la workstation, dove vengono installati il compilatore, i debuggers e gli SDK tools. Il target è generalmente la boward. Solitamente i sistemi sono collegati mediante cavi chiamati *seriali*.

## Distribuzioni Linux
Ubuntu è da sempre la più usata. Non lavorare mai utilizzando l'ultima versione. All'interno di linux non esiste codice C++. Linux è stato implementato in C come tutti gli altri sistemi UNIX. Tutto il codice è compilato con il compilatore `gcc`. All'interno del codice del kernel linux non esistono numeri con la virgola, perchè linux è stato pensato per supportare diverse architetture, che non sempre supportano operazioni a virgola mobile. Alcuni componenti hardware non rispettano determinati standard, pertanto linux va loro incontro in questo modo.

## Patch
Con il termine si fa rifermento ad un *metodo* che i sistemi utilizzano per aggiungere o rimuovere del codice da un determinato sorgente.