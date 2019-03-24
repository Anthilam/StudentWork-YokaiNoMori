#include "fonctionsTCP.h"

int socketClient(char* ipMachServ, unsigned short nPort){

    socklen_t sizeAdd;              /* taille d'une structure pour l'adresse de socket */
    struct sockaddr_in addSockServ  ;/* adresse de la socket connexion du serveur */

    int sock, err;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
      perror("(client) erreur sur la creation de socket");
      exit(-1);
    }

    /*
     * initialisation de l'adresse de la socket - version inet_aton
     */

    addSockServ.sin_family = AF_INET;
    err = inet_aton(ipMachServ, &addSockServ.sin_addr);
    if (err == 0) {
      perror("(client) erreur obtention IP serveur");
      close(sock);
      exit(-2);
    }

    addSockServ.sin_port = htons(nPort);
    bzero(addSockServ.sin_zero, 8);

    sizeAdd = sizeof(struct sockaddr_in);

    /*
     * connexion au serveur
     */
    err = connect(sock, (struct sockaddr *)&addSockServ, sizeAdd);

    if (err < 0) {
      perror("(client) erreur a la connection de socket");
      close(sock);
      exit(-3);
    }

    return sock;
}

int socketServeur(unsigned short nPort){
    int sizeAddr,err;
    int option = 1;
    struct sockaddr_in addServ;	/* adresse socket connex serveur */

    int sockValue = socket(AF_INET, SOCK_STREAM, 0);
    if (sockValue < 0) {
      perror("(serveurTCP) erreur de socket");
      exit(-1);
    }

    /*
     * initialisation de l'adresse de la socket
     */
    addServ.sin_family = AF_INET;
    addServ.sin_port = htons(nPort); // conversion en format réseau (big endian)
    addServ.sin_addr.s_addr = INADDR_ANY;
    // INADDR_ANY : 0.0.0.0 (IPv4) donc htonl inutile ici, car pas d'effet
    bzero(addServ.sin_zero, 8);

    sizeAddr = sizeof(struct sockaddr_in);

    if(setsockopt(sockValue,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(int)) < 0)
    {
        printf("setsockopt failed\n");
        close(sockValue);
        exit(-2);
    }

    /*
     * attribution de l'adresse a la socket
     */
    err = bind(sockValue, (struct sockaddr *)&addServ, sizeAddr);
    if (err < 0) {
      perror("(serveurTCP) erreur sur le bind");
      close(sockValue);
      exit(-3);
    }



    /*
     * utilisation en socket de controle, puis attente de demandes de
     * connexion.
     */
    err = listen(sockValue, 1);
    if (err < 0) {
      perror("(serveurTCP) erreur dans listen");
      close(sockValue);
      exit(-4);
    }
    return sockValue;
}
