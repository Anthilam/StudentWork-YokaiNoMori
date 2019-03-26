#include "libClient.h"

#define TAIL_BUF 20

int main(int argc, char **argv) {

    int sock,port;
    char* ipMachServ;

    /* verification des arguments */
    if (argc != 3) {
    printf("usage : %s nom/IPServ port\n", argv[0]);
    return -1;
    }

    ipMachServ = argv[1];
    port = atoi(argv[2]);

    /*
    * creation d'une socket, domaine AF_INET, protocole TCP
    */
    sock = socketClient(ipMachServ,port);




    /*
    * fermeture de la connexion et de la socket
    */
    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
