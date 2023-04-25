In un processore little endian vediamo 4 byte compilando con indirizzi a 32bit e l'indirizzo 015704d0 in memoria sono 4 byte, da quello meno significativo a quello
piú significativo: d0 04 57 01.

Quando si vanno a scrivere int o double in file binari, bisogna considerare il tipo di endian con cui si ha a che fare.