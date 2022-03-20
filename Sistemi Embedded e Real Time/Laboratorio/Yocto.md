



[TOC]

# Yocto

Yocto è un *set di strumenti* che generano il sistema finale (*distribuzione* Linux) a partire da un file di configurazione. Nel caso di studio si è generato un sistema per un emulatore con un'architettura a 64 bit: il sistema è stato testato con i comandi che fornisce Yocto stesso, `run qemu` è uno di questi.

## File di Configurazione

La distribuzione finale si genera a partire da un ***file di configurazione***. Il file di configurazione si trova in `/poky/build/conf/local.conf` e consiste in un *insieme di variabili*. 

### Bitbake

Lo stesso comando `bitbake core-image-minimal`, che genera tutto il sistema, non è altro che una ricetta (*macro* per generare una serie di pacchetti): si chiede al sistema *se esiste una ricetta per generare il pacchetto* specificato. Viene eseguito il comando `bitbake` per ogni pacchetto definito in `core-image-minimal` (insieme di pacchetti). Il `[TARGET]` specificato in seguito al comando `bitbake` può essere una libreria, un'applicazione o un emulatore. Il comando crea un *sistema minimale*, con la sola interfaccia a linea di comando. Solitamente viene utilizzato come ***configurazione di pacchetti di base*** per aggiungere nuovi pacchetti alla distribuzione.

#### Aggiungere un Pacchetto

Per aggiungere un pacchetto al sistema finale che si vuole creare è necessario utilizzare la variabile `IMAGE_INSTALL:append` all'interno del file di configurazione. I pacchetti che si vogliono inserire nel sistema devono essere aggiunti a questa variabile, che non è altro che una ***lista di pacchetti***: una *stringa* contenente i *pacchetti separati da uno spazio*.

```bash
IMAGE_INSTALL:append = " pkg1 pkg2"
```

Notare lo *spazio presente appena aperte le virgolette*. Una volta definito il pacchetto da installare *non è necessario preoccuparsi delle dipendenze*: è *bitbake* che si preoccupa di installare le dipendenze (installare manualmente le dipendenze non comporta comunque dei problemi). A questo punto sarà sufficiente lanciare nuovamente il comando **`bitbake core-image-minimal`**. 

I termini ***pacchetto o ricetta*** sono equivalenti: è possibile pensare a un pacchetto come a qualcosa da *scaricare*, *compilare* ed eventualmente *installare sul sistema* (es. *OpenSSH*, *Kernel Linux*, *Emulatore Qemu* che deve girare non sulla board ma sulla macchina).

Yocto è uno strumento che non genera solo un binario o un intero sistema che esegue su una board, ma genera anche degli applicativi che girano sulla macchina. Durante la build ci sono alcuni pacchetti che impiegano molto tempo per compilare: il kernel Linux (*linux yocto*) *Qemu*.

##### Applicativi sul Sistema Host

Tra i pacchetti da installare è possibile che ci siano pacchetti girano non sul target, ma sulla macchina. È opportuno riconoscere questi pacchetti. La differenza tra un pacchetto che gira sulla board e uno che gira sulla macchina è *ben documentata nella **ricetta** stessa*. Quando viene richiesto di "installare un pacchetto", questo non deve essere installato sulla board  (sistema finale): non deve essere inserito nella variabile `IMAGE_INSTALL`, deve essere installato *sulla macchina* attraverso l'apposito comando. Tutti gli applicativi sul sistema Host vengono sempre comunque ***installati all'interno della cartella `/build`***: non intaccheranno il *sistema "principale"*, viene tutto mantenuto all'interno dell'ambiente.

## Ricetta

La ricetta è un *file* con estensione `.bb`. È possibile trovarne una in `/poky/build/meta-example/recipes-example/example`. Si tratta di un **file di testo** che definisce come ***scaricare***, ***compilare*** e ***installare*** un software. Un gestore di pacchetti come `apt` si limita a scaricare *direttamente i binari* (non i sorgenti) e li installa, saltando lo step di compilazione. Le ricette sono inserite all'interno di layer (cartelle) nominate come `meta-[NAME]`, dove vengono raggruppate in altre cartelle in base al tema (utilizzo o tipologia): qui ciascuna ricetta deve poi essere inserita in una cartella avente lo *stesso nome della ricetta*.

> Es. <u>Nome e Posizione di una Ricetta</u>
>
> ```bash
> user@hpc1:~/poky/meta-example/recipes-example/example$ cat example_0.1.bb
> ```
>
> Se come nel caso precedente una ricetta presenta una *versione*, questa *non deve essere riportata nel nome della cartella*.

I pacchetti installabili sulla macchina finale si possono trovare in `/poky/meta/recipes-devtools/file`.

Nella maggior parte dei casi **per scaricare** i sorgenti all'interno di una ricetta si usa ***git***, specificando una repository su Github.

```bash
SRC_URI = "git://github.com/file/file.git;branch=master;protocol=https"
```

In seguito, nella ricetta vengono utilizzati gli *autotools* per le fasi successive della ricetta: si tratta di *strumenti* sviluppati da GNU Software che aiutano nella configurazione e nella compilazione di un generico applicativo scritto in C/C++. Infine nel file viene specificato il *come compilare il pacchetto* `do_compile:append:class-native(){}` e il come *installarlo* `do_install:append:class-native(){}`. L'importante è che in una ricetta siano presenti tutte e tre le fasi di scaricamento, compilazione e installazione: il modo in cui queste tre vengono fatte cambia da ricetta a ricetta. Nel caso si voglia inserire uno *script in linguaggio Python* è opportuno specificare Python come dipendenza: poiché uno script Python viene interpretato e non compilato, la parte di compilazione sarà definita ma risulterà nulla, ovvero non presenterà istruzioni al suo interno.

### Pacchetti

Per ottenere una **lista dei pacchetti installabili** (per ogni layer) è sufficiente, in seguito al `source ./oe-init-build-env` lanciare, nel percorso `/poky/build` il comando:

```bash
<$ bitbake -s | less
```

Quando ricercando un pacchetto il suo nome lo si trova preceduto da `nativesdk-[NAME]`, ***non si sta installando il pacchetto sul sistema finale, ma sul sistema Host*** (in un ambiente di compilazione). Anche gli altri pacchetti con il nome nel formato `[NAME]-native` si comportano come i precedenti, andando sempre a installare il pacchetto sull'Host (per il sistema in se'). Poiché i pacchetti messi a disposizione dai layer sono pochi, talvolta può essere necessario scaricare nuovi layer.

### Layer

Per ottenere **la lista di layer presenti sul sistema** si deve lanciare il comando nel percorso `/poky/build`:

```bash
<$ bitbake-layers show-layers
```

#### Installare un Layer

##### Scaricare un Layer Esistente

Utilizzato ***per installare un pacchetto già esistente***. Per ricercare un applicativo particolare non presente sui layer nel sistema si fa affidamento a Google. Per installare il Linux di Xilinx è sufficiente cercare `meta-xilinx`: è sempre preferibile *cercare il layer*, la maggior parte dei quali sono presenti su Github. Alcuni layer presentano al loro interno altri layer (*layer innestato*): questo non è un problema dal momento che le ricette si troveranno sempre all'interno della cartella `recipes-[THEME]` di un layer. Se non si conosce il nome del layer da cercare si procede cercando: `yocto recipes [PKG_NAME]`. Nel caso di *applicativi semplici* è possibile fare affidamento su sistemi online che si occupano di *indicizzare pacchetti*, nel caso di pacchetti più complessi è sconsigliato.

Una volta trovata la repository Github del layer di nostro interesse questo deve essere ***scaricato all'interno della cartella `/poky`***.

```bash
<$ git clone https://github.com/netmodule/meta-netmodule.git
```

Una volta scaricato il layer nella cartella, questo ***non è automaticamente aggiunto a bitbake***. Per dire al sistema di prendere anche le ricette dell'ultimo layer aggiunto si può utilizzare un comando oppure modificare un file di configurazione. 

##### Creare un Layer

Per creare un layer è possibile utilizzare il seguente comando nella directory `/poky`:

```bash
<$ bitbake-layers create-layer ${LAYER_NAME}
```

Il `${LAYER_NAME}` rappresenta un **percorso**: può essere il nome del layer (cartella), quindi un *percorso relativo*, oppure un *percorso assoluto*. 

#### Aggiungere il Layer

Le azioni precedenti *creano o scaricano* il layer, ma *non lo aggiungono* a `bitbake`. Per aggiungerlo è possibile ricorrere a due metodi.

##### File di Configurazione

Il file definisce la *lista di layer dove bitbake deve andare a ricercare i pacchetti*, e si trova in `/poky/build/conf/bblayers.conf`. Per aggiungere un layer a bitbake è necessario aggiungere il ***percorso completo del layer*** in questo file. Una volta aggiunto è possibile accedere a tutte le ricette del layer appena aggiunto. Rappresenta la soluzione più semplice senza complicazioni.

##### Comando `bash`

Per aggiungerlo è necessario utilizzare il comando:

```bash
<$ bitbake-layers add-layer ~/../../${LAYER_NAME}
```

In questo caso deve essere specificato il *percorso assoluto*. Per aggiungere il layer è sempre preferibile il metodo manuale che prevede la modifica del file `/poky/build/conf/bblayer.conf`.

È opportuno controllare sempre che la **versione di Yocto** che si sta utilizzando sia utilizzata con **layer che supportano quella versione**.

### Build della Ricetta

Una volta che il layer è stato scaricato/creato e aggiunto a bitbake, per fare la build di una sua ricetta si utilizza il comando:

```bash
<$ bitbake {RECIPE_NAME}
```

## Creazione di una Ricetta Personalizzata

### Applicazione in C

Si scrive un'applicazione e se ne gestiscono le dipendenze. Tutte le ricette e i pacchetti devono essere inseriti all'interno di un layer. Si procede con la creazione di una ricetta semplice che prende il codice di un'applicazione scritta in C, lo compila e lo installa. Il codice dell'applicazione scritta in C è il seguente:

```c
#include <stdio.h>

int main(){
    pritnf("Hello, World!\n");
    return 0;
}
```

Deve essere creata una cartella che abbia lo stesso nome della ricetta all'interno di un layer, come ad esempio `/poky/meta-example/recipes-example/hello-world`.

### Ricetta

La ricetta più semplice che gestisce un file sorgente in linguaggio C è la seguente:

```bash
DESCRIPTION = "Simple helloworld application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://hello.c"

S = "${WORKDIR}"

do_compile() {
	${CC} hello.c ${LDFLAGS} -o hello
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 hello ${D}${bindir}
}
```

L'ordine delle variabili non importa, tuttavia all'inizio della ricetta è bene mettere i *metadati*: `DESCRIZIONE`, `LICENZA`, `CHECKSUM`. Il `SRC_URI` definisce la posizione del file nella rete: può ***prendere in ingresso più file separati da uno spazio***. Con una configurazione di default il sistema grazie alla sintassi `file://` va a cercare i file in delle posizioni specifiche: queste posizioni sono riconosciute dalla *directory **`/files`*** all'interno della cartella avente il nome della ricetta:

```bash
../meta-example/${NOME_RICETTA}/files/${NOME_FILE}.c
```

È sempre preferibile utilizzare ***`git://`***. La ricetta deve contenere le funzioni **`do_compile(){}`** e **`do_install(){}`**, se l'applicazione necessita di particolari configurazioni è possibile specificare quest'ultime all'interno del **`do_configure(){}`**, che non è obbligatorio. All'interno di tutte queste funzioni si utilizza *scripting bash* come linguaggio. 

#### Compilazione: `do_compile(){}`

In fase di compilazione non è possibile utilizzare comandi come `gcc`: non viene riconosciuto da Yocto. Bisogna fare in modo che, come quando si utilizza una *funzione in una shell*, venga utilizzato il giusto percorso a `gcc-cc` (non è propriamente `gcc`, ma una versione che permette la *cross-compilazione*), che sul sistema esiste in quanto lo scarica e lo compila: questo percorso è contenuto all'interno della variabile **`${CC}`**.

- `${CC}` &rarr; `gcc`
- `${CXX}` &rarr; `gpp`
- `${OEMAKE}` &rarr; `make`

La variabile **`S="${WORKDIR}"`** è una variabile che permette a Yocto di specificare come ***ambiente di lavoro*** la *cartella dove sono stati scaricati i sorgenti*. Con la variabile **`${LDFLAGS}`** si fa riferimento alle ***librerie da passare al compilatore***: viene messa perché può tornare utile, ma di default ha valori nulli, è *necessario configurarla*.

#### Installazione: `do_install(){}`

Il comando **`install`** è un comando  che permette di "installare" un file. Solitamente quando si vuole installare un file o si crea una cartella e si copia il file, o ci si limita a copiare il file. La maggior parte degli applicativi finali vengono installati in `/usr/bin`. La combinazione di `${D}${bindir}` danno il *percorso completo di questa cartella* (la `${D}` dà il percorso prima, come `../build/tmp/work/..`). Con l'opzione `-d` la cartella viene creata, con l'opzione `-m` il binario specificato viene copiato nella cartella e gli vengono dati i permessi specificati.

```text
/poky
	|--- meta-example
			|-  recipes-example
					|- hello-world
						|- files
							|- hello.c
```

#### Aggiungere Ricetta a bitbake

Per aggiungerla è necessario modificare il file `/poky/build/conf/local.conf`, modificano il parametro `IMAGE_INSTALL`:

```bash
IMAGE_INSTALL:append = " hello-world"
```

A questo punto bisogna rilanciare il comando:

```bash
<$ bitbake core-image-minimal
```

### Testare il Pacchetto

Per testare il pacchetto installato è necessario rilanciare l'emulatore.

```bash
<$ runqemu slirp nographic
```

Una volta fatto il login con `root` sarà sufficiente scrivere il nome dell'applicazione, che nel nostro caso è `hello`.

La sorgente di copia e la destinazione di copia utilizzano la stessa struttura: Yocto al termine della compilazione sincronizza la struttura sorgente con quella di destinazione. 

## Modifica al Kernel Linux

Una **patch** è una piccola modifica contenuta aggiunta in maniera dinamica a lato della compilazione del software. Quando il software è semplice e il sorgente è già presente sulla macchina è possibile modificarlo, nel caso di software più complessi è necessario utilizzare un *sistema di patching*. Si vuole creare un *set di ricette semplici* che permettono di modificare Linux data una patch. Nel momento in cui si crea una modifica è necessario capire su quali kernel deve andare. Per cominciare si apre il file `/poky/build/conf/local.conf`, dove la variabile:

```bash
INHERIT += "rm_work"
```

È una configurazione che fa sì che il sistema liberi la memoria da tutti i file generati durante la compilazione in seguito alla build del pacchetto eliminando anche il sorgente del software scaricato. Per escludere singoli pacchetti da questo tipo di politica di eliminazione è possibile utilizzare la variabile `RM_WORK_EXCLUDE`. Il sorgente di Linux si trova nel percorso `/poky/build/tmp/work-shared/qemuarm64/kernel-source`.

### Aggiunta di una Stringa

Si suppone di voler modificare Linux affinché stampi una stringa all'avvio. Il file da modificare per questo tipo di operazione si trova nel percorso `/poky/build/tmp/work-shared/qemuarm64/kernel-source/arch/arm64/kernel/setup.c` (codice del *kernel linux* utilizzato durante la fase di setup in un'architettura `arm64`). Si procede ricercando all'interno del codice la stringa "Booting Linux on Physical CPU" e prima di questa si aggiunge la stampa di interesse mediante la funzione `pr_info();`.

### Creazione Patch

Una volta completato è necessario **creare la patch**, per farlo si utilizza **git**.

1. `git add arch/arm64/kernel/setup.c`
2. `git commit -s -m "Message"`
3. `git format-patch -1`

#### Aggiunta della Patch

Dopo aver generato la patch, è necessario aggiungerla tra i file che la ricetta deve gestire. La patch è un file, pertanto deve essere gestita *come un sorgente* ed essere inserita all'interno di una cartella `/files`. Tutte le ricette che hanno a che vedere con Linux vengono messe all'interno della cartella **`/recipes-kernel`**. Il layer deve essere conforme allo standard di organizzazione di yocto, pertanto questa cartella deve essere inserita ***all'interno della cartella `/meta-${LAYER}`***. Il nome della cartella di Linux dipende dal layer principale, che in questo caso è `/poky`: andando nel layer in cui linux è definito si ricerca la ricetta in cui viene definito: nel caso di poky il nome della cartella linux è `/linux`, pertanto deve essere creata una cartella omonima all'interno della cartella precedentemente creata:

```bash
user@iot:~/poky/meta-example/recipes-kernel/linux
```

In questo percorso si deve creare la cartella `/files` e al suo interno si inserisce la patch creata `hello.patch`.

### Ricetta

Yocto mette a disposizione un sistema per l'***estensione delle ricette***. Si crea una ricetta che *estende una ricetta genitore*. Una volta creata questa estensione, tutto ciò che viene aggiunto al suo interno viene utilizzato dalla ricetta genitore. Per aggiungere una patch da fare applicare a Yocto, prima della fase di compilazione, è necessario aggiungere all'interno dei file sorgente il file con estensione `.patch`, è a questo che serve l'estensione della ricetta.

#### Creazione Estensione della Ricetta

All'interno dell'ultima cartella creata `/linux` è necessario creare il file **`linux-yocto_%.bbappend`**; nel caso di Linux il nome dell'estensione della ricetta non deve essere uguale a quello della cartella in cui è contenuto: Linux presenta molte versione e il sistema deve essere in grado di gestirle tutte. In questo modo si lascia che sia Yocto a preoccuparsi di questo aspetto.

> **NB**. Linux è un sistema complesso, pertanto all'interno di poky i nomi delle sue ricette differiscono da quello della cartella in cui risiedono: la cartella prende il nome di `/linux`, mentre le ricette `linux-yocto.bb`. 

L'estensione della ricetta creata è un file di configurazione:

```bash
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += " file://hello.patch"
```

La variabile `FILESEXTRAPATHS` aggiunge ai percorsi che Yocto interroga il percorso specificato come valore. Linux è un pacchetto che di default viene già aggiunto da Yocto, pertanto non è necessario specificare ulteriori dettagli in `/poky/build/conf/local.conf`.

A questo punto è necessario lanciare nuovamente il comando:

```bash
<$ bitbake core-image-minimal
```

E guardare il risultato della patch avviando l'emulatore:

```bash
<$ runqemu slirp no-graphic
```

