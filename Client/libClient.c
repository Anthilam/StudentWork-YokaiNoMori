#include "libClient.h"

void checkRecvrError(int err,int sock){
    if(err <= 0){
        perror("(client) erreur sur la reception");
        shutdown(sock, SHUT_RDWR); close(sock);
        exit(-1);
    }
}

void checkSendError(int err,int sock ){
    if( err <= 0){
        perror("(client) erreur sur l'envoie");
        shutdown(sock, SHUT_RDWR); close(sock);
        exit(-2);
    }
}

void sendPartieGetRep(int sock, TPartieReq req, TPartieRep* res){
    int err;
    err = send(sock, &req, sizeof(TPartieReq), 0);
    checkSendError(err,sock);

    err = recv(sock, res, sizeof(TPartieRep), 0);
    checkRecvrError(err,sock);
    printf("Code : %d , valeur retourner : %s\n",res->err,res->nomAdvers);
}

void sendCoupGetRep(int sock,TCoupReq reqCoup,TCoupRep *repCoup){
  int err;
  err = send(sock, &reqCoup, sizeof(TPartieReq), 0);
  checkSendError(err,sock);

  err = recv(sock, repCoup, sizeof(TPartieRep), 0);
  checkRecvrError(err,sock);
  printf("Code : %d , valeur retourner : %d\n",repCoup->err,repCoup->validCoup);
}

void readEnnemyAction(int sock,TCoupRep *coupAdv){
  // Lecture de la validation du coup de l'adversaire
  int err = recv(sock, coupAdv, sizeof(TPartieRep), 0);
  checkRecvrError(err,sock);
  printf("Coup de l'ennemie Code : %d , valeur retourner : %d\n",coupAdv->err,coupAdv->validCoup);
  // Read the opponent action

}
