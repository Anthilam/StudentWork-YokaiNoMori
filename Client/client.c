#include "libClient.h"

/* Taille des chaines de caracteres pour les noms */

int main(int argc, char **argv) {

    int sock,port;
    char* ipMachServ;
    int nbPartie=1; // Variable mémorisant le numéro de partie

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


    // tant que le client est connecté au serveur
    // et qu'on a pas jouer deux parties
    while(true && nbPartie < 3){
      // Envoie des données et
      // Consultation d'IA
      // IA envoie :
      TCoupRep repCoup; // Variable pour la réponse du serveur
      TCoupReq reqCoup; // Variable pour envoyer le coup jouer

      // Construction du coup
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

      // Construction de la requete d'un coup
      reqCoup.idRequest = COUP;
      reqCoup.numPartie = nbPartie;
      reqCoup.typeCoup = DEPLACER;
      reqCoup.piece = tP;
      reqCoup.params.deplPiece = tDepl;

      // Notre joueur demande toujours le le coté nord
      if(repServeur.validSensTete == OK && nbPartie == 1){
        // On commence la partie
        printf("coté nord");

        // Envoie du coup jouer
        sendCoupGetRep(sock,reqCoup,&repCoup);


        // Coup de l'adversaire
        readEnnemyAction(sock,&repCoup);

        if(repCoup.propCoup != CONT){
          nbPartie++;
        }

      }else{
        // L'adversaire commence
        printf("coté sud");

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
