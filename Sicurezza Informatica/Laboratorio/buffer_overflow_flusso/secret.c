#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void stampa_errore()
{
	puts("La scelta immessa non è un numero oppure non è un numero valido\n");
}

void stampa_autore()
{
	puts("L'autore del \"La Guida Galattica per Autostoppisti\" è Douglas Adams\n");
}

void accesso_autorizzato()
{
	puts("La risposta alla domanda fondamentale sulla vita, l'universo e tutto quanto è: 42\n");
}

void accesso_non_autorizzato()
{
	puts("Non sei degno di conoscere la risposta alla domanda fondamentale sulla vita, l'universo e tutto quanto\n");
}

void accesso_non_previsto()
{
	puts("Questa funzione non è mai invocata da nessuno e quindi non sarà mai eseguita\n");
}

void saluta_educatamente()
{
	puts("Grazie per aver utilizzato questo meraviglioso software!\n");
}

int leggi_scelta()
{
	char buffer[15];
	gets(buffer);
	return atoi(buffer);
}

bool autorizza(const char* secret){
	char buffer[20];
	puts("Immetti la parola d'ordine! ");
	gets(buffer);
	return !strcmp(secret,buffer);
}

int main(int* argc, char** argv)
{

	char *secret = "douglasadams";
	char buffer[8];
	int scelta=0;
	while (scelta !=3){
		puts("Cosa vuoi sapere delle Guida Galattica per Autostoppisti?:\n");
		puts("\t1- chi è l'autore?\n");
		puts("\t2- qual'è la risposta alla domanda fondamentale sulla vita, l'universo e tutto quanto?\n");
		puts("\t3- nulla, voglio uscire!\n");
		puts("Immetti il numero corrispondente alla tua scelta e premi invio: ");
		scelta = leggi_scelta();
		switch (scelta)
		{
			case 1:
				stampa_autore();
				break;
			case 2:
				if (autorizza(secret))
				{
					accesso_autorizzato();
				}else{
					accesso_non_autorizzato();
				};
				break;
			case 3:
				saluta_educatamente();
				break;
			default : 
				stampa_errore();
				break;
		}
	}

	return 0;
}
