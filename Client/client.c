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
  char *name = argv[3];         // Player's name
  char* ipIa = argv[5];

  bool connected;               // Connection's to the server state

  printf("* Starting client..\n");
  printf("* Trying to connect to the AI\n");

  // Connection to the AI
  sockIa = socketClient(ipIa, portIa);
  printf("* Connected to the AI\n");

  printf("* Trying to connect to the game-server\n");

  // Connection to the game server
  sock = socketClient(ipMachServ, port);
  connected = true;

  printf("* Connected to the game-server\n");

  TPartieReq initGame;
  TPartieRep repServeur;

  initGame.idReq = PARTIE;

  stpcpy(initGame.nomJoueur, name);
  initGame.piece = SUD;

  printf("* Sending player's data to the game-server :\n");
  printf("\tName : %s\n", initGame.nomJoueur);
  printf("\tOrientation : SUD\n");
  sendPartieGetRep(sock, initGame, &repServeur);

  printf("* Player's data sent\n");

  // Update side with server response
  printf("* Orientation sent by server : ");

  TInitIa orientation;
  if (repServeur.validSensTete == OK) {
    orientation.sens = true;
    printf("SUD\n");
  }
  else {
    orientation.sens = false;
    printf("NORTH\n");
  }

  printf("* Sending the orientation to the AI\n");
  err = send(sockIa, &orientation, sizeof(bool), 0);
  if (err <= 0) {
    perror("Sending orientation has failed");
    return -1;
  }

  /* Tant que le client est connecté au serveur
  et qu'on a pas joué deux parties */
  while (connected && nbPartie < 3) {
    TCoupRep repCoup; // Structure réponse serveur
    TCoupReq reqCoup; // Structure envoi serveur
    TCoupIa coupIa;   // Structure envoi IA
    TCoupIa recvIa;   // Structure réponse IA
    // Si orienté sud et partie 1 ou orienté nord et partie 2
    printf("Partie numéro %d : \n\n",nbPartie);
    if ((orientation.sens == true && nbPartie == 1)
    || (orientation.sens == false && nbPartie == 2)) {

      printf("* Getting strike from AI\n");
      getCoupFromAI(sockIa, &recvIa);

      convertAItoServer(&recvIa, &reqCoup, orientation.sens, nbPartie);

      printf("* Sending strike to server\n");
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // Si la partie se termine, notifier l'IA
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        recvIa.finPartie = true;
        sendCoupToAI(sockIa,recvIa);

        printf("* AI has been notified\n");
      }
      // Sinon lire le coup adverse
      else {
        recvIa.finPartie = false;

        // Lecture coup adverse
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &coupIa);

        // Si le coup adverse termine la partie, notifier l'IA
        if (coupIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;
        }
        if(!coupIa.finPartie){
          printf("* Sending strike to the server");
        }
        sendCoupToAI(sockIa,coupIa);
        if(coupIa.finPartie){
            printf("* AI has been notified\n");
        }
      }
    }
    else {
      // reset variable
      recvIa.finPartie = false;

      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &coupIa);

      // Si le coup adverse termine la partie, notifier l'IA
      if (coupIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;
      }

      printf("* Sending ennemy action to AI\n");
      sendCoupToAI(sockIa,coupIa);
      if(!coupIa.finPartie){
        // Génération d'un coup par l'IA
        printf("* Getting strike from AI\n");
        getCoupFromAI(sockIa, &recvIa);

        convertAItoServer(&recvIa, &reqCoup, orientation.sens, nbPartie);

        printf("* Sending strike to the server\n");
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // Si la partie se termine, notifier l'IA
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          recvIa.finPartie = true;
          sendCoupToAI(sockIa,recvIa);

          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("\n\nFIN DES PARTIES\n\n nbGame : %d",nbPartie);
  // Fermeture de la socket
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
