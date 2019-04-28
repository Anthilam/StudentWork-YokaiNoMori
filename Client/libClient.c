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
