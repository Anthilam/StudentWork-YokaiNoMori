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

// Structure use to regroup game's information
typedef struct {
  int gameNumber;
  int scorePlayer1;
  int scorePlayer2;
  char player1Name[T_NOM];
  char player2Name[T_NOM];
  int nbStrike;
}TPartie;

/* Function to check the recv's errors

    params:
      int err : recv's return value
      int checkingSock : socket to the client associate to the function used
      int sock : socket connected to the second client
      TPartie game : the global game informations
      int idPlayer : the id's player associate to the function used
 */
void checkRecvrError(int err,int checkingSock,int sock,TPartie game, int idPlayer);

/* Function to check the send's errors

    params:
      int err : recv's return value
      int checkingSock : socket to the client associate to the function used
      int sock : socket connected to the second client
      TPartie game : the global game informations
      int idPlayer : the id's player associate to the function used
 */
void checkSendError(int err,int checkingSock,int sock,TPartie game, int playerId);

/*  Function to process the game request send the game's initializator answers,
    also update game informations

    params:
      int sockP1 : socket connected to first client
      int sockP2 : socket connected to the second client
      TPartieReq player1 : the player1's game request
      TPartieReq player2 : the player2's game request
      TPartie *game : the game global game informations
  */
int sendAnswers(int sockP1,int sockP2,TPartieReq player1,TPartieReq player2,TPartie *game);

/*  Function to proper print the game's informations and end the game

    params:
      int sockP1 : socket connected to the first client
      int sockP2 : socket connected to the second client
      TPartie game : the global game informations
  */
void endGame(int sock1,int sock2,TPartie game);

/*  Function to process a strike request and prepare the strike's answer

    params:
      int sockP1 : socket connected to the first client
      int sockP2 : socket connected to the second clients
      bool validation : validationCoup()'s return value
      TCoupReq strike : strike to process
      TPropCoup strikeProp : strike's propperty
      TCoupRep *strikeAns : the process's result
  */
void prepareStrikeAnswer(int sock1,int sock2,bool validation, TCoupReq strike,TPropCoup strikeProp,TCoupRep *strikeAns);

/*  Function to send the validation to both client and the strike to the other client

    params:
      int sock1 : socket connected to the first client
      int sock2 : socket connected to the second client
      TCoupReq strike : the last strike received by the server
      TCoupRep strikeAns : the strike's validation
      TPartie game : the global game informations
      int playerId : the id number of the player that played the last strike
  */
void sendStrikeAnswer(int sock1,int sock2, TCoupReq strike,TCoupRep strikeAns,TPartie game,int playerId);

/*  Function to print a strike

    params:
      TCoupReq coup : the strike to print
  */
void printStrikeServer(TCoupReq coup);

#endif
