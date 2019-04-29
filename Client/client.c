#include "libClient.h"

/* Taille des chaines de caracteres pour les noms */

int main(int argc, char **argv) {
    /* Checking args */
    if (argc != 6) {
      printf("usage : %s nom/IPServ port nom_joueur port_IA ip_IA\n", argv[0]);
      return -1;
    }

    int sock, sockIa;
    int port = atoi(argv[2]);     // Game server port
    int portIa = atoi(argv[4]);   // AI port
    int nbPartie = 1;             // Game's Id
    int err;

    char* ipMachServ = argv[1];   // Game server ip
    char *name = argv[3];         // Name of the player
    char* ipIa = argv[5];

    bool connected;               // Connection's state

    printf("* Starting client..\n");

    printf("* Trying to connect to the AI\n");

    // Connection to the AI
    sockIa = socketClient(ipIa, portIa);

    // Orientation of the player
    TInitIa orientation;
    orientation.sens = true;

    printf("* Connected to the AI\n");

    printf("* Sending the orientation\n");
    err = send(sockIa, &orientation, sizeof(bool), 0);

    printf("send a strike");
    TCoupRep repCoup; // Variable pour la réponse du serveur
    TCoupReq reqCoup; // Variable pour envoyer le coup jouer

    // Construction du coup
    TPiece tP;
    tP.sensTetePiece = SUD;
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
    tDepl.estCapt = false;

    // Construction de la requete d'un coup
    reqCoup.idRequest = COUP;
    reqCoup.numPartie = nbPartie;
    reqCoup.typeCoup = DEPLACER;
    reqCoup.piece = tP;
    reqCoup.params.deplPiece = tDepl;

    TCase tCaseDep1;
    tCaseDep1.c = htonl(A);
    tCaseDep1.l = htonl(UN);

    TCase tCaseArr1;
    tCaseArr1.c = htonl(A);
    tCaseArr1.l = htonl(DEUX);

    TDeplPiece tDepl1;
    tDepl1.caseDep = tCaseDep1;
    tDepl1.caseArr = tCaseArr1;
    tDepl1.estCapt = false;

    TCoupIa coupIa;
    coupIa.typeCoup = htonl(DEPLACER);
    coupIa.piece = htonl(KODAMA);
    coupIa.params.deplPiece = tDepl1;

    err = send(sockIa,  &coupIa ,sizeof(TCoupIa),0);
    printf("waiting for recv\n");

    err = recv(sockIa, &err, sizeof(TPartieRep), 0);

    printf("* Trying to connect to the game-server\n");

    // Connection to the game server
    sock = socketClient(ipMachServ,port);
    connected = true;

    printf("* Connected to the game-server\n");

    TPartieReq initGame;
    TPartieRep repServeur;

    initGame.idReq = PARTIE;

    stpcpy(initGame.nomJoueur, name);
    initGame.piece = NORD;

    printf("* Player's name : %s\n", initGame.nomJoueur);

    printf("* Sending player's name to the game-server\n");
    sendPartieGetRep(sock,initGame,&repServeur);

    printf("* Player's name sent\n");

    // tant que le client est connecté au serveur
    // et qu'on a pas jouer deux parties
    while (connected && nbPartie < 3) {
      // Envoie des données et
      // Consultation d'IA


      // Notre joueur demande toujours le coté sud
      if (repServeur.validSensTete == OK && nbPartie == 1) {
        // On commence la partie
        printf("coté sud");

        // Envoie du coup jouer
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // Coup de l'adversaire
        readEnnemyAction(sock, &repCoup);

        if (repCoup.propCoup != CONT) {
          nbPartie++;
        }
      }
      else {
        // L'adversaire commence
        printf("coté nord");

        // Coup de l'adversaire
        readEnnemyAction(sock, &repCoup);

        // Envoie du coup jouer
        sendCoupGetRep(sock,reqCoup, &repCoup);
      }
    }

    // Fermeture de la socket
    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
