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

typedef struct {
  int gameNumber;
  int scorePlayer1;
  int scorePlayer2;
  char player1Name[T_NOM];
  char player2Name[T_NOM];
  int nbStrike;
}TPartie;

/* Fonction permettant de vérifié les érreurs RÉSEAU sur la fonction RECV
  params:
    int err : valeur de retour de la fonction recv
    int sock : socket connecté au serveur
 */
void checkRecvrError(int err,int checkingSock,int sock,TPartie game, int idPlayer);

/* Fonction permettant de vérifié les érreurs RÉSEAU sur la fonction SEND
  params:
    int err : valeur de retour de la fonction recv
    int sock : socket connecté au serveur
 */
void checkSendError(int err,int checkingSock,int sock,TPartie game, int playerId);


int sendAnswers(int sockP1,int sockP2,TPartieReq player1,TPartieReq player2,TPartie *game);

void endGame(int sock1,int sock2,TPartie game);

void prepareStrikeAnswer(int sock1,int sock2,bool validation, TCoupReq strike,TPropCoup strikeProp,TCoupRep *strikeAns);

void sendStrikeAnswer(int sock1,int sock2, TCoupReq strike,TCoupRep strikeAns,TPartie game,int playerId);

void printStrikeServer(TCoupReq coup);

#endif
