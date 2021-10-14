/*
    Esempio di Shellcode Injection
*/
int main(int argc, char *argv[]){
    // Alloca 500 byte
    char buffer[500];
    // Prende il parametro con cui viene invocato da riga di comando
    // e lo copia all'interno del buffer
    if (argc >= 2) strcopy(buffer, argv[1]);
    return 0;
}