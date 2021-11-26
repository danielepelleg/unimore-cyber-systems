```bash
$> iptables -t FILTER -p tcp --dport 22 -i eth0 -s 192.168.2.1 -A INPUT -j ACCEPT -m state --state NEW,ESTABLISHED 
```

## Collegamento da Internet al Server Web
L'indirizzo IP del server web non è pubblico. E' necessario pertanto fare il natting. E' necessario "nattare" il server web www (192.168.1.1) dietro il firewall esterno, sulla sua porta 80. Ci spostiamo sul firewall esterno per eseguire le politiche di natting:
```bash
ext-firewall:~$> iptables -t nat -A PREROUTING -i eth1 -p tcp --dport http -j DNAT --to-destination 192.168.1.1  
```
Non viene specificato l'indirizzo sorgente perchè vogliamo che la regola sia valida per ogni utente di internet. Prepariamo ora la risposta dal server web verso internet.
```bash
ext-firewall:~$> iptables -t filter -A FORWARD -i eth0 -p tcp -d 192.168.1.1 --dport http -j ACCEPT -m state --state NEW,ESTABLISHED
```

## Connessione al Server FTP da Internet
Situazione analoga alla precedente
```bash
ext-firewall:~$> iptables -t nat -A PREROUTING -i eth1 -p tcp --dport ftp -j DNAT 192.168.1.1 --to-destination 192.168.1.2
```
Ora si vuole gestire la richiesta di connessione dall'esterno. Notare che nella richiesta il parametro NEW deve essere accettato in quanto si cerca di stabilire una nuova connessione.
```bash
ext-firewall:~$> iptables -t filter -A FORWARD -i eth1 -p tcp -d 192.168.1.2 --dport ftp -j ACCEPT -m state --state NEW,ESTABLISHED
```
```bash
ext-firewall:~$> iptables -t filter -A FORWARD -i eth0 -o eth1 -p tcp -s 192.168.1.2 --sport ftp-data -j ACCEPT 192.168.1.1 -m state --state NEW,ESTABLISHED
```
## Connessione dalla rete Locale al Server Web
```bash
int-firewall:~$> iptables -t filter -A FORWARD -i eth0 -o eth1 -p tcp -d 192.168.1.1 -s 192.168.2.0/24 --sport http -j ACCEPT -m state --state NEW,ESTABLISHED
```
```bash
int-firewall:~$> iptables -t filter -A FORWARD -i eth1 -o eth0 -p tcp -s 192.168.1.1 -d 192.168.2.0/24 --sport http -j ACCEPT -m state --state ESTABLISHED
```