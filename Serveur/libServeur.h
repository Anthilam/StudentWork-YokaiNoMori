/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * libServeur.h - function library header for the server
 *
 */

#ifndef LIB_SERVEUR_H
#define LIB_SERVEUR_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../TCP/fonctionsTCP.h"
#include "../TCP/protocolYokai.h"

// Structure use to regroup game's data
typedef struct {
  int gameNumber;
  int scorePlayer1;
  int scorePlayer2;
  char player1Name[T_NOM];
  char player2Name[T_NOM];
  int nbStrike;
} TPartie;

/* Function to check recv's errors

    params:
      int err : recv's return value
      int checkingSock : socket to the client associate to the function used
      int sock : socket connected to the second client
      TPartie game : the global game data
      int idPlayer : player's id associated to the function used
 */
void checkRecvrError(int err, int checkingSock, int sock, TPartie game, int idPlayer);

/* Function to check send's errors

    params:
      int err : recv's return value
      int checkingSock : socket to the client associate to the function used
      int sock : socket connected to the second client
      TPartie game : the global game data
      int idPlayer : player's id associated to the function used
 */
void checkSendError(int err, int checkingSock, int sock, TPartie game, int playerId);

/*  Function to process game request, send game's init answers,
    and also update game data

    params:
      int sockP1 : socket connected to first client
      int sockP2 : socket connected to the second client
      TPartieReq player1 : player1's game request
      TPartieReq player2 : player2's game request
      TPartie *game : the game global game data
  */
int sendAnswers(int sockP1, int sockP2, TPartieReq player1, TPartieReq player2, TPartie *game);

/*  Function to proper print game's data and end the game

    params:
      int sockP1 : socket connected to the first client
      int sockP2 : socket connected to the second client
      TPartie game : the global game data
  */
void endGame(int sock1, int sock2, TPartie game);

/*  Function to process a move request and prepare move's answer

    params:
      int sockP1 : socket connected to the first client
      int sockP2 : socket connected to the second clients
      bool validation : validationCoup()'s return value
      TCoupReq move : move to process
      TPropCoup moveProp : move's propperty
      TCoupRep *moveAns : the processing result
  */
void prepareStrikeAnswer(int sock1, int sock2, bool validation, TCoupReq move, TPropCoup moveProp, TCoupRep *moveAns);

/*  Function to send the validation to both client and the move to the other client

    params:
      int sock1 : socket connected to the first client
      int sock2 : socket connected to the second client
      TCoupReq move : the last move received by the server
      TCoupRep moveAns : move's validation
      TPartie game : the global game data
      int playerId : the id of the player that played the last move
  */
void sendStrikeAnswer(int sock1, int sock2, TCoupReq move, TCoupRep moveAns, TPartie game, int playerId);

/*  Function to print a move

    params:
      TCoupReq coup : the move to print
  */
void printStrikeServer(TCoupReq coup);

#endif
