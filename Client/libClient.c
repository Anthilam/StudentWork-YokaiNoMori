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
  err = send(sock, &reqCoup, sizeof(TPartieReq), 0);
  checkSendError(err, sock);

  err = recv(sock, repCoup, sizeof(TPartieRep), 0);
  checkRecvrError(err, sock);

  printf("* sendCoupGetRep\n\tCode : %d\n\tValue : %d\n", repCoup->err, repCoup->validCoup);
}

void readEnnemyAction(int sock, TCoupRep *coupAdv){
  int err = recv(sock, coupAdv, sizeof(TPartieRep), 0);
  checkRecvrError(err, sock);

  printf("* readEnnemyAction\n\tCode : %d\n\tValue : %d\n", coupAdv->err, coupAdv->validCoup);
  // Read the opponent action
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

void getCoupFromNetwork(int sock,TCoupIa *res){
  res->typeCoup = receiveIntFromJava(sock);
  res->piece = receiveIntFromJava(sock);

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

void printStrikeDepl(TCoupIa coup){
  printf("Type : %d Piece : %d \n",coup.typeCoup, coup.piece);
  if( coup.typeCoup == DEPLACER){
    printf("Case : Col :%d Lig : %d \n Case : Col :%d Lig : %d \n Capture %d \n",coup.params.deplPiece.caseDep.c,coup.params.deplPiece.caseDep.l,coup.params.deplPiece.caseArr.c,coup.params.deplPiece.caseArr.l,coup.params.deplPiece.estCapt);
  }else{
    printf("Case : Col :%d Lig : %d \n ",coup.params.deposerPiece.c, coup.params.deposerPiece.l);
  }
}
