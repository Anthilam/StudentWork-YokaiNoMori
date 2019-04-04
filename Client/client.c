#include "libClient.h"

/* Taille des chaines de caracteres pour les noms */

int main(int argc, char **argv) {

    int sock,port;
    char* ipMachServ;

    /* verification des arguments */
    if (argc != 4) {
      printf("usage : %s nom/IPServ port nom_joueur\n", argv[0]);
    }
    // Lecture des arguments
    ipMachServ = argv[1];
    port = atoi(argv[2]);

    // Connexion au serveur
    sock = socketClient(ipMachServ,port);
    printf("connected\n");

    TPartieReq initGame;
    TPartieRep repServeur ;
    /*
    typedef struct {
      TIdReq idReq;
      char nomJoueur[T_NOM];
      TSensTetePiece piece;
    } TPartieReq;
    */
    initGame.idReq = PARTIE;
    char *name = argv[3];
    stpcpy(initGame.nomJoueur,name);
    initGame.piece = NORD;

    printf("name : %s\n",initGame.nomJoueur );

    printf("envoie au serveur ");
    sendPartieGetRep(sock,initGame,&repServeur);
    // Fermeture de la socket
    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
