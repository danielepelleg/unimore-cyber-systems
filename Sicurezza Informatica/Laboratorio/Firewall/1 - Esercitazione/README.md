# Richieste

- Utilizzare una policy di negazione implicita per tutti i pacchetti in transito
- Bloccare tutti i pacchetti diretti verso il firewall e provenienti da Internet
- Bloccare tutti i pacchetti diretti verso il firewall e provenienti dalla DMZ
- Bloccare tutti i pacchetti diretti verso il firewall e provenienti dalla LAN, tranne le connessioni ssh generate dalla macchina local2
- Consentire connessioni TCP sulla porta 80 da Internet verso il server web www.fake.com
- Consentire le risposte di www.fake.com a connessioni originate da Internet
- Consentire connessioni TCP sulla porta 80 dalla LAN verso il server web www.fake.com
- Consentire le risposte di www.fake.com a connessioni originate dalla LAN
- Consentire connessioni TCP sulla porta 21 da Internet verso il server FTP ftp.fake.com
- Consentire le risposte di ftp.fake.com a connessioni originate da Internet
- Consentire connessioni TCP sulla porta 21 dalla LAN verso il server FTP ftp.fake.com
- Consentire le risposte di ftp.fake.com a connessioni originate dalla LAN
- Consentire a tutte le macchine nella LAN di inviare pacchetti verso macchine in Internet e di ricevere i pacchetti in risposta e correlati ai pacchetti inviati
- Consentire alle macchine local1 e local2 di accedere a macchine in Internet condividendo l'IP pubblico associato all'interfaccia eth2 di firewall
- Consentire alla macchina remote1 di aprire una connessione TCP sulla porta 22 della macchina local1 utilizzando l'IP pubblico associato all'interfaccia eth2 di firewall come indirizzo di destinazione