#include "libClient.h"

/* Taille des chaines de caracteres pour les noms */

int main(int argc, char **argv) {
    /* Checking args */
    if (argc != 4) {
      printf("usage : %s nom/IPServ port nom_joueur port_IA\n", argv[0]);
      return -1;
    }
    int sock,port,sockIa;
    char* ipMachServ;

    ipMachServ = argv[1]; // Game serveur ip
    port = atoi(argv[2]); // Game server port

    char *name = argv[3]; // The name of the player
    int portIa = atoi(argv[4]);   // the Ia port

    int nbPartie=1; // Game's Id
    bool connected; // Connection's state

    // connection to the IA
    sockIa = socketClient("localhost",portIa);

    // Connection to the game server
    sock = socketClient(ipMachServ,port);
    printf("connected\n");
    connected = true;
    TPartieReq initGame;
    TPartieRep repServeur;

    initGame.idReq = PARTIE;

    stpcpy(initGame.nomJoueur,name);
    initGame.piece = NORD;

    printf("name : %s\n",initGame.nomJoueur );

    printf("envoie au serveur ");
    sendPartieGetRep(sock,initGame,&repServeur);

    // tant que le client est connecté au serveur
    // et qu'on a pas jouer deux parties
    while(connected && nbPartie < 3){
      // Envoie des données et
      // Consultation d'IA
      // IA envoie :
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

      // Construction de la requete d'un coup
      reqCoup.idRequest = COUP;
      reqCoup.numPartie = nbPartie;
      reqCoup.typeCoup = DEPLACER;
      reqCoup.piece = tP;
      reqCoup.params.deplPiece = tDepl;

      // Notre joueur demande toujours le coté sud
      if(repServeur.validSensTete == OK && nbPartie == 1){
        // On commence la partie
        printf("coté sud");

        // Envoie du coup jouer
        sendCoupGetRep(sock,reqCoup,&repCoup);

        // Coup de l'adversaire
        readEnnemyAction(sock,&repCoup);

        if(repCoup.propCoup != CONT){
          nbPartie++;
        }
      }else{
        // L'adversaire commence
        printf("coté nord");

        // Coup de l'adversaire
        readEnnemyAction(sock,&repCoup);

        // Envoie du coup jouer
        sendCoupGetRep(sock,reqCoup,&repCoup);
      }
    }
    // Fermeture de la socket
    shutdown(sock, SHUT_RDWR);
    close(sock);

    return 0;
}
