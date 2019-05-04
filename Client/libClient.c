#include "libClient.h"

void checkRecvrError(int err,int sock){
  if (err <= 0) {
    perror("* Error while receiving");
    shutdown(sock, SHUT_RDWR); close(sock);
    exit(-1);
  }
}

void checkSendError(int err, int sock ){
  if( err <= 0){
    perror("* Error while sending");
    shutdown(sock, SHUT_RDWR); close(sock);
    exit(-2);
  }
}

void sendPartieGetRep(int sock, TPartieReq req, TPartieRep* res){
  int err;
  err = send(sock, &req, sizeof(TPartieReq), 0);
  checkSendError(err, sock);

  err = recv(sock, res, sizeof(TPartieRep), 0);
  checkRecvrError(err, sock);

  printf("* sendPartieGetRep\n\tCode : %d\n\tValue : %s\n", res->err, res->nomAdvers);
}

void sendCoupGetRep(int sock, TCoupReq reqCoup, TCoupRep *repCoup){
  int err;
  err = send(sock, &reqCoup, sizeof(reqCoup), 0);
  checkSendError(err, sock);

  err = recv(sock, repCoup, sizeof(repCoup), 0);
  checkRecvrError(err, sock);

  printf("* sendCoupGetRep\n\tCode : %d\n\tValue : %d\n", repCoup->err, repCoup->validCoup);
}

void readEnnemyAction(int sock,TCoupIa *coupAdv){
  TCoupRep res;
  TCoupReq coup;
  int err = recv(sock, &res, sizeof(res), 0);
  checkRecvrError(err, sock);

  printf("* readEnnemyAction\n\tCode : %d\n\tValue : %d\n", res.err, res.validCoup);

  // TODO : lire le coup adverse
  if (res.propCoup != CONT) {
    convertServerToAI(coupAdv, &coup, true);
  }
  else {
    int err = recv(sock, &coup, sizeof(coup), 0);
    checkRecvrError(err, sock);
    convertServerToAI(coupAdv, &coup, false);
  }
}

int receiveIntFromJava(int sock){
  char buff[4];

  int readed = 0;
  int r=0;
  while (readed < 4){
    r = read(sock, buff + readed, sizeof(buff) - readed);
    if (r < 1) {
      break;
    }
    readed += r;
  }
  return ntohl(buff[3] << 24 | buff[2] << 16 | buff[1] << 8 | buff[0]);
}

int receiveBoolFromJava(int sock){
  int r=0;
  bool res;
  r = read(sock, &res, sizeof(bool));
  if (r < 1) {
    exit(-1);
  }
  return res;
}

void getCoupFromAI(int sock, TCoupIa *res){
  res->typeCoup = receiveIntFromJava(sock);
  res->piece = receiveIntFromJava(sock);
  res->finPartie = receiveBoolFromJava(sock);
  if(res->typeCoup == DEPLACER){
    res->params.deplPiece.caseDep.c = receiveIntFromJava(sock);
    res->params.deplPiece.caseDep.l = receiveIntFromJava(sock);
    res->params.deplPiece.caseArr.c = receiveIntFromJava(sock);
    res->params.deplPiece.caseArr.l = receiveIntFromJava(sock);
    res->params.deplPiece.estCapt = receiveBoolFromJava(sock);

  }else{
    res->params.deposerPiece.c = receiveIntFromJava(sock);
    res->params.deposerPiece.l = receiveIntFromJava(sock);
  }
}

void sendCoupToAI(int sock, TCoupIa coupIa){
  printStrikeIa(coupIa);
  if(coupIa.finPartie == true){
    if(coupIa.typeCoup == DEPLACER){
      coupIa.params.deplPiece.caseDep.c = htonl(0);
      coupIa.params.deplPiece.caseDep.l = htonl(0);
      coupIa.params.deplPiece.caseArr.c = htonl(0);
      coupIa.params.deplPiece.caseArr.l = htonl(0);
    }else if(coupIa.typeCoup == DEPOSER){
      coupIa.params.deposerPiece.c = htonl(0);
      coupIa.params.deposerPiece.l = htonl(0);
    }
    coupIa.typeCoup = htonl(0);
    coupIa.piece = htonl(0);
  }else{
    if(coupIa.typeCoup == DEPLACER){
      coupIa.params.deplPiece.caseDep.c = htonl(coupIa.params.deplPiece.caseDep.c);
      coupIa.params.deplPiece.caseDep.l = htonl(coupIa.params.deplPiece.caseDep.l);
      coupIa.params.deplPiece.caseArr.c = htonl(coupIa.params.deplPiece.caseArr.c);
      coupIa.params.deplPiece.caseArr.l = htonl(coupIa.params.deplPiece.caseArr.l);
    }else if(coupIa.typeCoup == DEPOSER){
      coupIa.params.deposerPiece.c = htonl(coupIa.params.deposerPiece.c);
      coupIa.params.deposerPiece.l = htonl(coupIa.params.deposerPiece.l);
    }
    coupIa.typeCoup = htonl(coupIa.typeCoup);
    coupIa.piece = htonl(coupIa.piece);
  }
  printf("\n apres conversion \n");
  printStrikeIa(coupIa);

  send(sock, &coupIa, sizeof(coupIa), 0);
}

void convertAItoServer(TCoupIa *ai, TCoupReq *req, bool sens, int nbPartie) {
	// Construction du coup depuis le coup de l'IA
	TPiece tP;
	if (sens == true) {
		tP.sensTetePiece = SUD;
	}
	else {
		tP.sensTetePiece = NORD;
	}

	tP.typePiece = ai->piece;

	// Construction de la requete d'un coup
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

void printStrikeIa(TCoupIa coup){
  printf("Partie finie ? %d ", coup.finPartie);
  if(coup.finPartie == true){
    printf(" oui \n");
  }else{
    printf(" non \n");
  }
  printf("Type : %d Piece : %d \n",coup.typeCoup, coup.piece);
  if( coup.typeCoup == DEPLACER){
    printf("Case : Col :%d Lig : %d \n Case : Col :%d Lig : %d \n Capture %d \n",coup.params.deplPiece.caseDep.c,coup.params.deplPiece.caseDep.l,coup.params.deplPiece.caseArr.c,coup.params.deplPiece.caseArr.l,coup.params.deplPiece.estCapt);
  }else{
    printf("Case : Col :%d Lig : %d \n ",coup.params.deposerPiece.c, coup.params.deposerPiece.l);
  }
  printf("\n");
}
