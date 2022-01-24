# Richieste

- Utilizzare una policy di negazione implicita per tutti i pacchetti in transito su entrambi i firewall
- Utilizzare una policy di negazione implicita per tutti i pacchetti in ingresso in entrambi i firewall
- Utilizzare una policy di negazione implicita per tutti i pacchetti in uscita da entrambi i firewall
- Consentire a local1 di aprire connessioni ssh verso int-firewall ed ext-firewall
- Consentire connessioni TCP sulla porta 80 dalla LAN verso il server web
- Consentire le risposte del server web a connessioni originate dalla LAN
- Consentire connessioni TCP sulla porta 21 dalla LAN verso il server FTP ftp.fake.com
- Consentire le risposte di ftp.fake.com a connessioni originate dalla LAN
- Consentire connessioni TCP sulla porta 80 da Internet verso il server web
- Consentire le risposte del server web a connessioni originate da Internet
- Consentire connessioni TCP sulla porta 21 da Internet verso il server FTP ftp.fake.com
- Consentire le risposte di ftp.fake.com a connessioni originate da Internet
- Consentire agli host in Internet di accedere al sito web installato nella DMZ utilizzando l'indirizzo www.fake.com
- Consentire agli host in Internet di accedere al server FTP installato nella DMZ utilizzando l'indirizzo ftp.fake.com
- Consentire agli host della LAN di accedere a server web in Internet solo utilizzando il server web installato nella DMZ di FAKE come proxy (non trasparente)