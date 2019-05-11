/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * Client.c - the main client program that communicates with the game server
 *
 */

#include "libClient.h"

int main(int argc, char **argv) {
  // Check arguments
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

  // Send player data to the server
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

  // FIRST GAME
  printf("** STARTING GAME 1 **\n");
  while (connected && nbPartie == 1) {

    TCoupRep repCoup; // Structure used to get the answer from the server
    TCoupReq reqCoup; // Structure used to send a move to server
    TCoupIa coupIa;   // Structure used to send a move to AI
    TCoupIa recvIa;   // Structure used to get the answer from the AI

    // If the client is oriented south
    if (orientation.sens == true) {

      // Get a move from the AI
      printf("* Getting move from AI\n");
      getCoupFromAI(sockIa, &coupIa);

      // Send the move to the server
      printf("* Sending move to server\n");
      // Convert the move from AI shape to server shape
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // If the game ends, notify the AI
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);

        // Store first game result
        if (repCoup.propCoup == GAGNE) {
          winFirstGame = true;
        }

        printf("* AI has been notified\n");
      }
      // If the game isn't ended
      else {
        // Read opponent's move
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &recvIa, &winFirstGame);

        // If opponent's move is ending the game, notify the AI
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Otherwise, send opponent's move to the AI
        else {
          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // If the client is oriented north
    else {
      // Read opponent's move
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa,&winFirstGame);

      // If opponent's move is ending the game, notify the AI
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // If the game isn't ended
      else {
        // Send opponent's move to the AI
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Get a move from the AI
        printf("* Getting move from AI\n");
        getCoupFromAI(sockIa, &coupIa);


        // Send the move to the server
        printf("* Sending move to the server\n");
        // Convert the move from AI shape to server shape
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // If the game ends, notify the AI
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          coupIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);

          // Store first game result
          if (repCoup.propCoup == GAGNE) {
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
    TCoupReq reqCoup; // Structure used to send a move to server
    TCoupIa coupIa;   // Structure used to send a move to AI
    TCoupIa recvIa;   // Structure used to get the answer from the AI

    // // If the client is oriented south
    if (orientation.sens == false) {

      // Get a move from the AI
      printf("* Getting move from AI\n");
      getCoupFromAI(sockIa, &coupIa);

      // Send the move to the server
      printf("* Sending move to server\n");
      // Convert the move from AI shape to server shape
      convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
      sendCoupGetRep(sock, reqCoup, &repCoup);

      // If the game ends, notify the AI
      if (repCoup.propCoup != CONT) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        coupIa.finPartie = true;
        sendCoupToAI(sockIa, coupIa);

        // Store the result of the second game
        if (repCoup.propCoup == GAGNE) {
          winSecondGame = true;
        }

        printf("* AI has been notified\n");
      }
      // If the game isn't ended
      else {
        // Read opponent's move
        printf("* Getting ennemy action\n");
        readEnnemyAction(sock, &recvIa,&winSecondGame);

        // If opponent's move is ending the game, notify the AI
        if (recvIa.finPartie) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          sendCoupToAI(sockIa, recvIa);

          printf("* AI has been notified\n");
        }
        // Otherwise, send opponent's move to the AI
        else {
          printf("* Sending ennemy action to AI\n");
          sendCoupToAI(sockIa, recvIa);
        }
      }
    }
    // If the client is oriented north
    else {
      // Read opponent's move
      printf("* Getting ennemy action\n");
      readEnnemyAction(sock, &recvIa,&winSecondGame);

      // If opponent's move is ending the game, notify the AI
      if (recvIa.finPartie) {
        printf("* Game ended, notifying AI\n");
        nbPartie++;

        sendCoupToAI(sockIa, recvIa);

        printf("* AI has been notified\n");
      }
      // Otherwise
      else {
        // Send opponent's move to the AI
        printf("* Sending ennemy action to AI\n");
        sendCoupToAI(sockIa, recvIa);

        // Get a move from the AI
        printf("* Getting move from AI\n");
        getCoupFromAI(sockIa, &coupIa);

        // Send the move to the server
        printf("* Sending move to the server\n");
        // Convert the move from AI shape to server shape
        convertAItoServer(&coupIa, &reqCoup, orientation.sens, nbPartie);
        sendCoupGetRep(sock, reqCoup, &repCoup);

        // If the game ends, notify the AI
        if (repCoup.propCoup != CONT) {
          printf("* Game ended, notifying AI\n");
          nbPartie++;

          coupIa.finPartie = true;
          sendCoupToAI(sockIa, coupIa);

          // Store the result of the second game
          if (repCoup.propCoup == GAGNE) {
            winSecondGame = true;
          }

          printf("* AI has been notified\n");
        }
      }
    }
  }

  printf("\n** FIN DES PARTIES **\n");

  // Print results
  if (winFirstGame) {
    printf("* Résultat 1ère partie : gagné !\n");
  }
  else {
    printf("* Résultat 1ère partie : perdu ou nul !\n");
  }

  if (winSecondGame){
    printf("* Résultat 2ème partie : gagné !\n");
  }
  else {
    printf("* Résultat 2ème partie : perdu ou nul !\n");
  }

  // Shutdown and close AI socket
  shutdown(sockIa,SHUT_RDWR);
  close(sockIa);

  // Shutdown and close server socket
  shutdown(sock, SHUT_RDWR);
  close(sock);

  return 0;
}
