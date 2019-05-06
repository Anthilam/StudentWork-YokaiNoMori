#include "libServeur.h"

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

int sendAnswers(int sockP1,int sockP2,TPartieReq player1,TPartieReq player2){
  int err=0;
  TPartieRep answerP1;
  TPartieRep answerP2;
  // true : player1 is oriented sud  ; false : player2 is orientied nord
  // null : error ⁼> endgame
  int orientation = -1;

  // requests verification
  if(player1.idReq != PARTIE){
    answerP1.err = ERR_PARTIE;
  }
  else if(player2.idReq != PARTIE){
    answerP2.err = ERR_PARTIE;
  }
  else{
    answerP1.validSensTete = true;
    answerP1.err = ERR_OK;
    answerP2.err = ERR_OK;
    strcpy(answerP1.nomAdvers, player2.nomJoueur);
    strcpy(answerP2.nomAdvers, player1.nomJoueur);

    if(player1.piece == NORD){
      orientation = 1;

      if(player2.piece == NORD){
        answerP2.validSensTete = false;
      }
      else{
        answerP2.validSensTete = true;
      }
    }
    else {
      orientation = 0;
      if(player2.piece == NORD){
        answerP2.validSensTete = true;
      }
      else{
        answerP2.validSensTete = false;
      }
    }
  }

  err = send(sockP1,&answerP1,sizeof(TPartieRep),0);
  checkSendError(err,sockP1);
  err = send(sockP2,&answerP2,sizeof(TPartieRep),0);
  checkSendError(err,sockP2);

  return orientation;
}
