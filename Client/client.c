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
    printf("New Strike incoming\n");
    TCoupRep repCoup; // Variable pour la réponse du serveur
    TCoupReq reqCoup; // Variable pour envoyer le coup jouer
    TCoupIa coupIa; // structure d'envoie à l'Ia

    // Si côté sud et partie 1 ou côté nord et partie 2
    if ((orientation.sens == true && nbPartie == 1)
    || (orientation.sens == false && nbPartie == 2)) {

      // Génération d'un coup par l'IA
      TCoupIa recvIa;
      printf("Getting strike from IA\n");
      getCoupFromAI(sockIa, &recvIa);

      convertAItoServer(&recvIa, &reqCoup, orientation.sens, nbPartie);
      printf("Sending to the server\n");
      // Envoie du coup jouer
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // Si la partie se termine, notifier l'IA
      if (repCoup.propCoup != CONT) {
        nbPartie++;
      }
      // Sinon lire le coup adverse
      else {
        // Lecture coup adverse
        printf("Reading ennemy action\n");
        readEnnemyAction(sock, &coupIa);

        // Si le coup adverse termine la partie, notifier l'IA
        if (coupIa.finPartie) {
          nbPartie++;
        }
      }
      printf("Sending ennemy action to ia\n");
      send(sockIa,&coupIa,sizeof(coupIa),0);
    }
    // TODO : Même chose qu'au dessus à l'inverse
    else {
      printf("Reading ennemy action\n");
      // Lecture coup adverse
      readEnnemyAction(sock, &coupIa);
      // Si le coup adverse termine la partie
      if (coupIa.finPartie) {
        nbPartie++;
      }
      printf("Sending ennemy action to ia\n");
      send(sockIa,&coupIa,sizeof(coupIa),0);
      // Génération d'un coup par l'IA
      TCoupIa recvIa;
      printf("Getting strike from IA\n");
      getCoupFromAI(sockIa, &recvIa);

      convertAItoServer(&recvIa, &reqCoup, orientation.sens, nbPartie);
      printf("Sending to the server\n");
      // Envoie du coup jouer
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // Si la partie se termine, notifier l'IA
      if (repCoup.propCoup != CONT) {
        nbPartie++;
      }
    }
  }


  // Fermeture de la socket
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
