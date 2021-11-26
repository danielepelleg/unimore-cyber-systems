for i in 'seq 1 1000';
    do
        # -R disabilita una delle tecniche di buffer overflow utilizzato dai SO moderni:
        # disattiva la randomizzazione dello spazio di indirizzi
        # Il software mandato in esecuzione in questo modo non viene più mappato ogni volta in indirizzi diversi
        setarch i386 -R .exploit $i;
done