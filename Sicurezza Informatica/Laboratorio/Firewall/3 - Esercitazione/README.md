# Richieste

- Utilizzare una policy di negazione implicita per tutti i pacchetti in transito su entrambi i firewall
- Utilizzare una policy di negazione implicita per tutti i pacchetti in ingresso in entrambi i firewall
- Utilizzare una policy di negazione implicita per tutti i pacchetti in uscita da entrambi i firewall
- Consentire a local2 di aprire connessioni ssh verso il firewall
- Consentire connessioni TCP sulla porta 80 da Internet verso il server web www.local.it
- Consentire le risposte di www.local.it a connessioni originate da Internet
- Consentire connessioni TCP sulla porta 80 da local2 verso il server web www.local.it
- Consentire le risposte di www.local.it a connessioni originate dal local2
- Consentire connessioni TCP sulla porta 21 da Internet verso il server FTP ftp.local.it
- Consentire le risposte di ftp.local.it a connessioni originate da Internet
- Consentire connessioni TCP sulla porta 21 da local2 verso il server FTP ftp.local.it
- Consentire le risposte di ftp.local.it a connessioni originate dal local2
- Consentire a tutte le macchine nella LAN di inviare pacchetti verso macchine in Internet e di ricevere i pacchetti in risposta e correlati ai pacchetti inviati
- Consentire alle macchine local1 e local2 di accedere a macchine in Internet condividendo  l'IP pubblico associato all'interfaccia eth0 di firewall
- Consentire alla macchina remote1 di aprire una connessione ssh sulla porta 2222 della  macchina local2 utilizzando l'IP pubblico associato all'interfaccia eth2 di firewall come indirizzo di destinazione
