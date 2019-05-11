#include "libClient.h"

int main(int argc, char **argv) {
  /* Checking args */
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

  bool connected;               // Connection's state to the server
  bool winFirstGame = false;    // Store the first game's result
  bool winSecondGame = false;   // Store the second game's result

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

  /* while the client is connected to the server
  and we are playing the first game */

  // FIRST GAME
  printf("** STARTING GAME 1 **\n");
  while (connected && nbPartie == 1) {

    TCoupRep repCoup; // Structure used to get the answer from the server
    TCoupReq reqCoup; // Structure used to send a strike to server
    TCoupIa coupIa;   // Structure used to send a strike to AI
    TCoupIa recvIa;   // Structure used to get the answer from the AI

    // If the client is oriented south
    if (orientation.sens == true) {

      // Receptioning the strike from the AI
      printf("* Getting strike from AI\n");
      getCoupFromAI(sockIa, &coupIa);


      printf("* Sending strike to server\n");
      // Converting the strike from AI shape to server shape
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      // Sending the strike to the server
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // If the game end, notifiying the AI
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);
        // storing the first game result
        if(repCoup.propCoup == GAGNE){
          winFirstGame = true;
        }
        printf("* AI has been notified\n");
      }
      // If the game isn't ended read opponent strike
      else {

        printf("* Getting ennemy action\n");
        // Reading opponent's strike
        readEnnemyAction(sock, &recvIa, &winFirstGame);

        // If the opponent's strike is ending the game, notifying the AI
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Else, sending the opponent's strike to the AI
        else {

          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // If the client is oriented north
    else {

      // Reading the opponent's strike
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa,&winFirstGame);

      // If the opponent's is ending the game, notifiying the AI
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // if the game isn't ended
      else {

        // Sending the opponent's strike to the AI
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Getting strike from the AI
        printf("* Getting strike from AI\n");
        getCoupFromAI(sockIa, &coupIa);


        printf("* Sending strike to the server\n");

        // converting the AI shape to the server shape
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        // sending the strike to the server
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // If the game is ended, notifying the AI
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          coupIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);

          if(repCoup.propCoup == GAGNE){
            winFirstGame = true;
          }

          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("** STARTING GAME 2 **\n");

  while (connected && nbPartie == 2) {
    TCoupRep repCoup; // Structure used to get the answer from the server
    TCoupReq reqCoup; // Structure used to send a strike to server
    TCoupIa coupIa;   // Structure used to send a strike to AI
    TCoupIa recvIa;   // Structure used to get the answer from the AI

    // // If the client is oriented south
    if (orientation.sens == false) {

      // Receptioning the strike from the AI
      printf("* Getting strike from AI\n");
      getCoupFromAI(sockIa, &coupIa);

      printf("* Sending strike to server\n");
      // Converting the strike from AI shape to server shape
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      // Sending the strike to the server
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // If the game end, notifiying the AI
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);
        if(repCoup.propCoup == GAGNE){
          winSecondGame = true;
        }
        printf("* AI has been notified\n");
      }
      // If the game isn't ended read opponent strike
      else {
        // Reading opponent's strike
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &recvIa,&winSecondGame);

        // If the opponent's strike is ending the game, notifying the AI
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Else, sending the opponent's strike to the AI
        else {

          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // If the client is oriented north
    else {
      // Reading opponent's strike
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa,&winSecondGame);

      // If the opponent's strike is ending the game, notifying the AI
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // Else, sending the opponent's strike to the AI
      else {
        // Sending the opponent's strike to the AI
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Getting strike from the AI
        printf("* Getting strike from AI\n");
        getCoupFromAI(sockIa, &coupIa);


        printf("* Sending strike to the server\n");
        // converting the AI shape to the server shape
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        // sending the strike to the server
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // If the game is ended, notifying the AI
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          coupIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);
          if(repCoup.propCoup == GAGNE){
            winSecondGame = true;
          }
          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("\n** FIN DES PARTIES **\n");

  if(winFirstGame){
    printf("* Mon Ia a gagné la première partie !\n");
  }else{
    printf("* Mon adversaire a gagné la première partie ou on a fait match nul\n");
  }

  if(winSecondGame){
    printf("* Mon Ia a gagné la seconde partie !\n");
  }else{
    printf("* Mon adversaire a gagné la seconde partie ou on a fait match nul\n");

  }
  // shutdown and closing the socket to the AI
  shutdown(sockIa,SHUT_RDWR);
  close(sockIa);
  // shutdown and closing the socket to the server
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
