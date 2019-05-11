/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * libClient.c - client functions library
 *
 */

#include "libClient.h"

#include <sys/ioctl.h>

void checkRecvrError(int err,int sock){
  if (err <= 0) {
    perror("* Error while receiving");
    shutdown(sock, SHUT_RDWR); close(sock);
    exit(-1);
  }
}

void checkSendError(int err, int sock ){
  if (err <= 0) {
    perror("* Error while sending");
    shutdown(sock, SHUT_RDWR); close(sock);
    exit(-2);
  }
}

void sendPartieGetRep(int sock, TPartieReq req, TPartieRep *res){
  // Send a game init request
  int err = send(sock, &req, sizeof(TPartieReq), 0);
  checkSendError(err, sock);

  // Get the answer
  err = recv(sock, res, sizeof(TPartieRep), 0);
  checkRecvrError(err, sock);

  printf("* sendPartieGetRep\n\tCode : %d\n\tValue : %s\n", res->err, res->nomAdvers);
}

void sendCoupGetRep(int sock, TCoupReq reqCoup, TCoupRep *repCoup){
  printStrikeServer(reqCoup);

  // Send a move to the server
  int err = send(sock, &reqCoup, sizeof(TCoupReq), 0);
  checkSendError(err, sock);

  // Get the answer
  err = recv(sock, repCoup, sizeof(TCoupRep), 0);
  checkRecvrError(err, sock);

  printf("* sendCoupGetRep\n\tCode : %d\n\tValid : %d\n\tProp : %d\n", repCoup->err, repCoup->validCoup, repCoup->propCoup);
}

void readEnnemyAction(int sock, TCoupIa *coupAdv, bool *win){
  TCoupRep res;
  TCoupReq coup;

  // Get opponent move's validation
  int err = recv(sock, &res, sizeof(TCoupRep), 0);
  checkRecvrError(err, sock);

  printf("* readEnnemyAction\n\tCode : %d\n\tValid : %d\n\tProp : %d\n", res.err, res.validCoup, res.propCoup);

  // Check if the game continue, if so we can read the opponent's move
  if (res.propCoup == CONT) {
    err = recv(sock, &coup, sizeof(TCoupReq), 0);
    checkRecvrError(err, sock);
  }

  printStrikeServer(coup);

  // Convert the move from server shape to AI shape with game end notification
  if (res.propCoup != CONT) {
    convertServerToAI(coupAdv, &coup, true);

    // If the opponent lost the game
    if(res.propCoup == PERDU){
      *win = true;
    }
  }
  else {
    convertServerToAI(coupAdv, &coup, false);
  }
}

int receiveIntFromJava(int sock){
  char buff[4];

  int readed = 0;
  int r = 0;

  // Read int size
  while (readed < 4) {
    r = read(sock, buff + readed, sizeof(buff) - readed);
    if (r < 1) {
      break;
    }

    readed += r;
  }

  // Build the int
  return ntohl(buff[3] << 24 | buff[2] << 16 | buff[1] << 8 | buff[0]);
}

int receiveBoolFromJava(int sock){
  int r = 0;
  bool res;

  // Read one bit (Java boolean)
  r = read(sock, &res, sizeof(bool));
  if (r < 1) {
    exit(-1);
  }

  return res;
}

void getCoupFromAI(int sock, TCoupIa *res) {
  // Read a move from the AI
  res->finPartie = receiveIntFromJava(sock);
  res->typeCoup = receiveIntFromJava(sock);
  res->piece = receiveIntFromJava(sock);

  if (res->typeCoup == DEPLACER) {
    res->params.deplPiece.caseDep.c = receiveIntFromJava(sock);
    res->params.deplPiece.caseDep.l = receiveIntFromJava(sock);
    res->params.deplPiece.caseArr.c = receiveIntFromJava(sock);
    res->params.deplPiece.caseArr.l = receiveIntFromJava(sock);
    res->params.deplPiece.estCapt = receiveBoolFromJava(sock);
  }
  else {
    res->params.deposerPiece.c = receiveIntFromJava(sock);
    res->params.deposerPiece.l = receiveIntFromJava(sock);
  }

  printStrikeIa(*res);
}

void sendCoupToAI(int sock, TCoupIa coupIa) {
  // Set the move to the proper format
  if (coupIa.typeCoup == DEPLACER) {
    coupIa.params.deplPiece.caseDep.c = htonl(coupIa.params.deplPiece.caseDep.c);
    coupIa.params.deplPiece.caseDep.l = htonl(coupIa.params.deplPiece.caseDep.l);
    coupIa.params.deplPiece.caseArr.c = htonl(coupIa.params.deplPiece.caseArr.c);
    coupIa.params.deplPiece.caseArr.l = htonl(coupIa.params.deplPiece.caseArr.l);
  }
  else if (coupIa.typeCoup == DEPOSER) {
    coupIa.params.deposerPiece.c = htonl(coupIa.params.deposerPiece.c);
    coupIa.params.deposerPiece.l = htonl(coupIa.params.deposerPiece.l);
  }

  coupIa.typeCoup = htonl(coupIa.typeCoup);
  coupIa.piece = htonl(coupIa.piece);
  coupIa.finPartie = htonl(coupIa.finPartie);

  // Send the move to the AI
  send(sock, &coupIa, sizeof(TCoupIa), 0);
}

void convertAItoServer(TCoupIa *ai, TCoupReq *req, bool sens, int nbPartie) {
	TPiece tP;

  // Set direction
	if (sens == true) {
		tP.sensTetePiece = SUD;
	}
	else {
		tP.sensTetePiece = NORD;
	}

	tP.typePiece = ai->piece;

	// Build move request
	req->idRequest = COUP;
	req->numPartie = nbPartie;
	req->typeCoup = ai->typeCoup;
	req->piece = tP;

	if (ai->typeCoup == DEPLACER) {
		req->params.deplPiece = ai->params.deplPiece;
	}
	else if (ai->typeCoup == DEPOSER) {
		req->params.deposerPiece = ai->params.deposerPiece;
	}
}

void convertServerToAI(TCoupIa *ai, TCoupReq *req, bool end) {
  ai->typeCoup = req->typeCoup;
  ai->piece = req->piece.typePiece;
  ai->finPartie = end;

  if (req->typeCoup == DEPLACER) {
    ai->params.deplPiece = req->params.deplPiece;
  }
  else if (ai->typeCoup == DEPOSER) {
    ai->params.deposerPiece = req->params.deposerPiece;
  }
}

void printStrikeIa(TCoupIa coup) {
  printf("* Strike IA\n\tPartie finie ? ");

  if (coup.finPartie == true) {
    printf("oui\n");
  }
  else {
    printf("non\n");
  }

  printf("\tType : %d\n\tPiece : %d\n", coup.typeCoup, coup.piece);

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
