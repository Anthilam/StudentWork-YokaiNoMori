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
    TPartieRep repServeur;

    initGame.idReq = PARTIE;
    char *name = argv[3];
    stpcpy(initGame.nomJoueur,name);
    initGame.piece = NORD;

    printf("name : %s\n",initGame.nomJoueur );

    printf("envoie au serveur ");
    sendPartieGetRep(sock,initGame,&repServeur);

    // Consultation au serveur d'IA

    TPiece tP;
    tP.sensTetePiece = NORD;
    tP.typePiece = KODAMA;

    TCase tCaseDep;
    tCaseDep.c = A;
    tCaseDep.l = UN;

    TCase tCaseArr;
    tCaseArr.c = A;
    tCaseArr.l = DEUX;

    TDeplPiece tDepl;
    tDepl.caseDep = tCaseDep;
    tDepl.caseArr = tCaseArr;

    TCoupReq reqCoup;
    TCoupRep repCoup;

    reqCoup.idRequest = COUP;
    reqCoup.numPartie = 1;
    reqCoup.typeCoup = DEPLACER;
    reqCoup.piece = tP;
    reqCoup.params.deplPiece = tDepl;
    sendCoupGetRep(sock,reqCoup,&repCoup);

    // Fermeture de la socket
    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
