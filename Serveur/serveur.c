/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * serveur.c - the main server program
 *
 */

#include "libServeur.h"
#include "validation.h"

#define TIME_MAX 6

int main(int argc, char** argv) {
  int sockConx, port, sizeAddr;   // Variable used to setup the server
  int connectionP1, connectionP2; // Sockets for clients
  int err;                        // Variable used to check errors

  TPartieReq player1;
  TPartieReq player2;

  // Initialize the global game data
  TPartie game;
  game.gameNumber = 1;    // First game
  game.scorePlayer1 = 0;  // Player 1 got 0 point
  game.scorePlayer2 = 0;  // Player 2 got 0 point
  game.nbStrike = 0;      // Move counter

  bool bothConnected;

  // true : player1 is oriented south  ; false : player2 is orientied north
  bool player1Sud = false;

  struct sockaddr_in addClient;	/* Client socket address */

  // Check arguments
  if (argc != 2) {
    printf ("usage : %s port\n", argv[0]);
    return -1;
  }

  // Prepare sockets
  port  = atoi(argv[1]);
  sockConx = socketServeur(port);
  sizeAddr = sizeof(struct sockaddr_in);

  printf("- Attente de connexion des clients \n");
  connectionP1 = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);

  printf("- Client 1 connecté \n");
  printf("- Attente de connexion du second client\n");

  connectionP2 = accept(sockConx, (struct sockaddr *)&addClient, (socklen_t *)&sizeAddr);
  bothConnected = true;

  // Initialization from Player1
  err = recv(connectionP1, &player1, sizeof(TPartieReq), 0);
  checkRecvrError(err, connectionP1, connectionP2, game, 1);

  // Initialization from Player2
  err = recv(connectionP2, &player2, sizeof(TPartieReq), 0);
  checkRecvrError(err, connectionP2, connectionP1, game, 2);

  // Send answers to both client
  int res = sendAnswers(connectionP1, connectionP2, player1, player2, &game);

  // Process and save the player orientation
  if (res == -1) {
    printf("Erreur sur les demandes de partie");
    return -1;
  }
  else if (res == 0) {
    player1Sud = false;
  }
  else {
    player1Sud = true;
  }

  printf("player1Sud :%d\n", player1Sud);

  strcpy(game.player1Name, player1.nomJoueur);
  strcpy(game.player2Name, player2.nomJoueur);

  printf("Joueur 1 : %s\n", game.player1Name) ;
  printf("Joueur 2 : %s\n", game.player2Name) ;

  // Setup the timeout
  struct timeval tv;
  tv.tv_sec = TIME_MAX;
  tv.tv_usec = 0;

  setsockopt(connectionP1, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
  setsockopt(connectionP2, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

  TCoupReq moveReq; // Structure used to get a move from clients
  TCoupRep moveRep; // Structure used to send a move to clients

  bool validation; // Boolean used to get the validationCoup() result

  // Board initialization
  initialiserPartie();

  // MAIN LOOP
  while(bothConnected && game.gameNumber < 3) {

    TPropCoup propCoup; // Variable used to store move property

    // Player1 play first in the first game (oriented north)
    if ((player1Sud == true && game.gameNumber == 1) ||
    (player1Sud == false && game.gameNumber == 2 )) {

      // Player 1 is playing
      err = recv(connectionP1, &moveReq, sizeof(TCoupReq), 0);
      checkRecvrError(err, connectionP1, connectionP2, game, 1);

      // Check move validation
      validation = validationCoup(1, moveReq, &propCoup);

      // Prepare move answer
      prepareStrikeAnswer(connectionP1, connectionP2, validation, moveReq, propCoup, &moveRep);

      // If there is more than 60 move played, it's a draw
      if (game.nbStrike >= 60) {
        propCoup = NUL;
      }

      // Send validation and move
      sendStrikeAnswer(connectionP1,connectionP2,moveReq,moveRep,game,1);

      // If the game ends
      if (moveRep.propCoup != CONT) {
        // Initialize a new game
        initialiserPartie();

        // Store the score
        switch (moveRep.propCoup) {
          case GAGNE : game.scorePlayer1++; break;
          case PERDU : game.scorePlayer2++; break;
          // If it's a draw
          default : break;
        }

        game.gameNumber++; // Increase game number
        game.nbStrike = 0; // Reset move counter

        // If two game have been played, end the game
        if (game.gameNumber == 3 ) {
          endGame(connectionP1, connectionP2, game);
        }

        printf("*** Début de la deuxième partie ***\n");
      }
      else {
        // Player 2 is playing
        err = recv(connectionP2, &moveReq, sizeof(TCoupReq), 0);
        checkRecvrError(err, connectionP2, connectionP1, game, 2);

        // Check move validation
        validation = validationCoup(2, moveReq, &propCoup);

        // Prepare move answer
        prepareStrikeAnswer(connectionP2, connectionP1, validation, moveReq, propCoup, &moveRep);

        // If there is more than 60 move played, it's a draw
        if (game.nbStrike >= 60) {
          propCoup = NUL;
        }

        // Send validation and move
        sendStrikeAnswer(connectionP2, connectionP1, moveReq, moveRep, game, 2);

        // If the game ends
        if (moveRep.propCoup != CONT) {
          // Initialize a new game
          initialiserPartie();

          // Store the score
          switch (moveRep.propCoup) {
            case GAGNE : game.scorePlayer2++; break;
            case PERDU : game.scorePlayer1++; break;
            // If it's a draw
            default : break;
          }

          game.gameNumber++; // Increase game number
          game.nbStrike = 0; // Reset move counter

          // If two game have been played, end the game
          if (game.gameNumber == 3) {
            endGame(connectionP1, connectionP2, game);
          }

          printf("*** Début de la deuxième partie ***\n");
        }
      }

      game.nbStrike++; // Increase move counter
    }
    // Player2 is second in first game (oriented south)
    else{
      // Player 2 is playing
      err = recv(connectionP2, &moveReq, sizeof(TCoupReq), 0);
      checkRecvrError(err, connectionP2, connectionP1, game, 2);

      // Check move validation
      validation = validationCoup(1, moveReq, &propCoup);

      // Preapare move answer
      prepareStrikeAnswer(connectionP2, connectionP1, validation, moveReq, propCoup, &moveRep);

      // If there is more than 60 move played, it's a draw
      if (game.nbStrike >= 60) {
        propCoup = NUL;
      }

      // Send validation and move
      sendStrikeAnswer(connectionP2, connectionP1, moveReq, moveRep, game, 2);

      // If the game ends
      if (moveRep.propCoup != CONT) {
        // Initialize a new game
        initialiserPartie();

        // Store the score
        switch(moveRep.propCoup){
          case GAGNE : game.scorePlayer2++; break;
          case PERDU : game.scorePlayer1++; break;
          // If it's a draw
          default : break;
        }

        game.gameNumber++; // Increase game number
        game.nbStrike = 0; // Reset move counter

        // If two game have been played, end the game
        if (game.gameNumber == 3) {
          endGame(connectionP2, connectionP1, game);
        }

        printf("*** Début de la deuxième partie ***\n");
      }
      else {
        // Player 1 is playing
        err = recv(connectionP1, &moveReq, sizeof(TCoupReq), 0);
        checkRecvrError(err, connectionP1, connectionP2, game, 1);

        // Check move validation
        validation = validationCoup(2, moveReq, &propCoup);

        // Prepare move answer
        prepareStrikeAnswer(connectionP1, connectionP2, validation, moveReq, propCoup, &moveRep);

        // If there is more than 60 move played, it's a draw
        if (game.nbStrike >= 60) {
          propCoup = NUL;
        }

        // Send validation and move
        sendStrikeAnswer(connectionP1, connectionP2, moveReq, moveRep, game, 1);

        // If the game ends
        if(moveRep.propCoup != CONT){
          // Initialize a new game
          initialiserPartie();

          // Store the score
          switch(moveRep.propCoup){
            case GAGNE : game.scorePlayer1++; break;
            case PERDU : game.scorePlayer2++; break;
            // If it's a draw
            default : break;
          }

          game.gameNumber++; // Increase game number
          game.nbStrike = 0; // Reset move counter

          // If two game have been played, end the game
          if (game.gameNumber == 3) {
            endGame(connectionP2, connectionP1, game);
          }

          printf("*** Début de la deuxième partie ***\n");
        }
      }

      game.nbStrike++; // Increase move counter
    }
  }

  // Close all sockets
  close(connectionP1);
  close(connectionP2);
  close(sockConx);

  return 0;
}
