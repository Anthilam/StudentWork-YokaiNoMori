#include "libClient.h"

int main(int argc, char **argv) {
  /* Checking args */
  for(int i= 0; i < argc;i++){
    printf("%s\n",argv[i]);
  }
  if (argc != 6) {
    printf("usage : %s IPServ port nom_joueur port_IA ip_IA\n", argv[0]);
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

  // PARTIE 1
  printf("** STARTING GAME 1 **\n");

  while (connected && nbPartie == 1) {
    TCoupRep repCoup; // Structure réponse serveur
    TCoupReq reqCoup; // Structure envoi serveur
    TCoupIa coupIa;   // Structure envoi IA
    TCoupIa recvIa;   // Structure réponse IA

    // Si orienté sud
    if (orientation.sens == true) {
      // Génération du coup par l'IA
      printf("* Getting strike from AI\n");
      getCoupFromAI(sockIa, &coupIa);

      // Envoi du coup au serveur
      printf("* Sending strike to server\n");
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // Si la partie se termine, notifier l'IA
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);

        printf("* AI has been notified\n");
      }
      // Sinon lire le coup adverse
      else {
        // Lecture coup adverse
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &recvIa);

        // Si le coup adverse termine la partie, notifier l'IA
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Sinon envoyer le coup adverse à l'IA
        else {
          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // Si orienté nord
    else {
      // Lecture du coup adverse
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa);

      // Si le coup adverse termine la partie, notifier l'IA
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // Sinon
      else {
        // Envoi du coup adverse à l'IA
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Génération du coup par l'IA
        printf("* Getting strike from AI\n");
        getCoupFromAI(sockIa, &coupIa);

        // Envoi du coup au serveur
        printf("* Sending strike to the server\n");
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // Si la partie se termine, notifier l'IA
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          recvIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);

          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("** STARTING GAME 2 **\n");

  while (connected && nbPartie == 2) {
    TCoupRep repCoup; // Structure réponse serveur
    TCoupReq reqCoup; // Structure envoi serveur
    TCoupIa coupIa;   // Structure envoi IA
    TCoupIa recvIa;   // Structure réponse IA

    // Si orienté sud
    if (orientation.sens == false) {
      // Génération du coup par l'IA
      printf("* Getting strike from AI\n");
      getCoupFromAI(sockIa, &coupIa);

      // Envoi du coup au serveur
      printf("* Sending strike to server\n");
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // Si la partie se termine, notifier l'IA
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);

        printf("* AI has been notified\n");
      }
      // Sinon lire le coup adverse
      else {
        // Lecture coup adverse
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &recvIa);

        // Si le coup adverse termine la partie, notifier l'IA
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Sinon envoyer le coup adverse à l'IA
        else {
          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // Si orienté nord
    else {
      // Lecture du coup adverse
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa);

      // Si le coup adverse termine la partie, notifier l'IA
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // Sinon
      else {
        // Envoi du coup adverse à l'IA
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Génération du coup par l'IA
        printf("* Getting strike from AI\n");
        getCoupFromAI(sockIa, &coupIa);

        // Envoi du coup au serveur
        printf("* Sending strike to the server\n");
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // Si la partie se termine, notifier l'IA
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          recvIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);

          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("\n** FIN DES PARTIES **\n");

  shutdown(sockIa,SHUT_RDWR);
  close(sockIa);
  // Fermeture de la socket
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
