/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * libServeur.c - function library for the server
 *
 */

#include "libServeur.h"

void checkRecvrError(int err, int checkingSock, int sock, TPartie game, int playerId) {
  if (err <= 0) {
    perror("* Error while receiving");

    // Send a move with a timeout
    TCoupRep timeOut;
    timeOut.err = ERR_COUP;
    timeOut.validCoup = TIMEOUT;

    // Check which player has failed to connect
    if (playerId == 1) {
      game.scorePlayer2 = 2;
    }
    else {
      game.scorePlayer1 = 2;
    }

    endGame(checkingSock, sock, game);
  }
}

void checkSendError(int err, int checkingSock, int sock, TPartie game, int playerId) {
  if (err <= 0) {
    perror("* Error while sending");

    // Send a move with a timeout
    TCoupRep timeOut;
    timeOut.err = ERR_COUP;
    timeOut.validCoup = TIMEOUT;

    // Check which player has failed to connect
    if (playerId == 1) {
      game.scorePlayer2 = 2;
    }
    else {
      game.scorePlayer1 = 2;
    }

    endGame(checkingSock, sock, game);
  }
}

int sendAnswers(int sockP1, int sockP2, TPartieReq player1, TPartieReq player2, TPartie *game){
  int err = 0;

  TPartieRep answerP1; // Used to send answers to the first client
  TPartieRep answerP2; // Used to send answers to the second client

  // 1 : player1 is oriented south ; 0 : player2 is orientied north
  // null : error -> endgame
  int orientation = -1;

  // Check requests
  if (player1.idReq != PARTIE) {
    answerP1.err = ERR_PARTIE;
  }
  else if (player2.idReq != PARTIE) {
    answerP2.err = ERR_PARTIE;
  }
  else {
    // Prepare both validation
    answerP1.validSensTete = OK;
    answerP1.err = ERR_OK;
    answerP2.err = ERR_OK;

    strcpy(answerP1.nomAdvers, player2.nomJoueur);
    strcpy(answerP2.nomAdvers, player1.nomJoueur);

    // Check orientation
    if (player1.piece == SUD) {
      printf("* Le joueur 1 demande le SUD\n");

      orientation = 1;
      if (player2.piece == SUD) {
        answerP2.validSensTete = KO;
      }
      else {
        answerP2.validSensTete = OK;
      }
    }
    else {
      printf("* Le joueur 1 demande le NORD\n");

      orientation = 0;
      if (player2.piece == SUD) {
        answerP2.validSensTete = OK;
      }
      else {
        answerP2.validSensTete = KO;
      }
    }
  }

  // Send game validation
  err = send(sockP1, &answerP1, sizeof(TPartieRep), 0);
  checkSendError(err, sockP1, sockP2, *game, 1);

  // Send game validation
  err = send(sockP2, &answerP2, sizeof(TPartieRep), 0);
  checkSendError(err, sockP2, sockP1, *game, 2);

  return orientation;
}

void endGame(int sock1, int sock2, TPartie game) {
  // Print score and winner
  printf("Score du joueur %s  : %d\n", game.player1Name, game.scorePlayer1);
  printf("Score du joueur %s  : %d\n", game.player2Name, game.scorePlayer2);

  if (game.scorePlayer1 > game.scorePlayer2) {
    printf("*** Le joueur ' %s ' gagne ***\n", game.player1Name);
  }
  else if (game.scorePlayer2 > game.scorePlayer1) {
    printf("*** Le joueur ' %s ' gagne ***\n", game.player2Name);
  }
  else {
    printf("*** Le match se termine sur une égalité ! ***\n");
  }

  // Close sockets
  shutdown(sock1, SHUT_RDWR); close(sock1);
  shutdown(sock2, SHUT_RDWR); close(sock2);

  exit(-2);
}

void prepareStrikeAnswer(int sock1, int sock2, bool validation, TCoupReq move, TPropCoup moveProp, TCoupRep *moveAns) {
  // Check validation
  if (validation == true) {
    moveAns -> err = ERR_OK;
    moveAns -> validCoup = VALID;
  }
  else {
    moveAns -> err = ERR_COUP;
    moveAns -> validCoup = TRICHE;
  }

  moveAns->propCoup = moveProp;
}

void sendStrikeAnswer(int sock1, int sock2, TCoupReq move, TCoupRep moveAns, TPartie game, int playerId) {
  int err;
  int secondPlayerId;

  if (playerId == 1) {
    secondPlayerId = 2;
  }
  else {
    secondPlayerId = 1;
  }

  // Validation of the player
  err = send(sock1, &moveAns, sizeof(TCoupRep), 0);
  checkSendError(err, sock1, sock2, game, playerId);

  // Validation of the other player
  err = send(sock2, &moveAns, sizeof(TCoupRep), 0);
  checkSendError(err, sock2, sock1, game, secondPlayerId);

  // Send the move to the other player if the game should continue
  if (moveAns.propCoup == CONT) {
    err = send(sock2, &move, sizeof(TCoupReq), 0);
    checkSendError(err, sock2, sock1, game, secondPlayerId);
  }
}

void printStrikeServer(TCoupReq coup) {
  printf("* Strike Server\n\tNum partie : %d\n", coup.numPartie);

  printf("\tType : %d\n\tPiece : %d\n", coup.typeCoup, coup.piece.typePiece);

  if (coup.typeCoup == DEPLACER) {
    printf("\tCaseDep > Col: %d Lig: %d\n\tCaseArr > Col: %d Lig: %d\n\tCapture %d\n\n",
      coup.params.deplPiece.caseDep.c, coup.params.deplPiece.caseDep.l,
      coup.params.deplPiece.caseArr.c, coup.params.deplPiece.caseArr.l,
      coup.params.deplPiece.estCapt
    );
  }
  else {
    printf("\tCase > Col: %d Lig: %d\n\n",
      coup.params.deposerPiece.c, coup.params.deposerPiece.l
    );
  }
}
