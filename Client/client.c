#include "libClient.h"

/* Taille des chaines de caracteres pour les noms */

int main(int argc, char **argv) {
    /* verification des arguments */
    if (argc != 4) {
      printf("usage : %s nom/IPServ port nom_joueur\n", argv[0]);
      return -1;
    }
    int sock,port;
    char* ipMachServ;
    // Lecture des arguments
    ipMachServ = argv[1];
    port = atoi(argv[2]);


    int nbPartie=1; // Variable mémorisant le numéro de partie
    bool connected;
    TTypePiece board[BOARD_SIZE][BOARD_SIZE];
    /* {{ONI,KIRIN,KOROPOKKURU,KIRIN,ONI},
        {NULL,NULL,NULL,NULL,NULL},
        {NULL,KODAMA,KODAMA,KODAMA,NULL},
        {NULL,NULL,NULL,NULL,NULL},
        {ONI,KIRIN,KOROPOKKURU,KIRIN,ONI}};
    */

    // Initialisation du tableau; -1 répresente l'absence de pièce
    for(int i=0; i < BOARD_SIZE ; i++){
      for(int j=0; j< BOARD_SIZE ; j++){
        board[i][j]=-1;
      }
    }
    // placement des pièces de base
    board[4][4] = board[4][0] = board[0][0] = board[0][4] = ONI;
    board[4][3] = board[4][1] = board[0][1] = board[0][3] = KIRIN;
    board[4][2] = board[0][2] = KOROPOKKURU;
    board[2][1] = board[2][2] = board[2][3] = KODAMA;

    // affichage de l'état de plateau
    for(int i=0; i < BOARD_SIZE ; i++){
      for(int j=0; j< BOARD_SIZE ; j++){
        printf("%d ",board[i][j]);
      }
      printf("\n");
    }

    // Connexion au serveur
    sock = socketClient(ipMachServ,port);
    printf("connected\n");
    connected = true;
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
