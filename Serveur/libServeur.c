#include "libServeur.h"

void checkRecvrError(int err,int checkingSock,int sock,TPartie game){
  if (err <= 0) {
    perror("* Error while receiving");

    // Envoie de reponse coup timeout

    endGame(checkingSock,sock,game);
  }
}

void checkSendError(int err,int checkingSock,int sock,TPartie game){
  if (err <= 0) {
    perror("* Error while sending");

    // Envoie de reponse coup timeout
    
    endGame(checkingSock,sock,game);
  }
}

int sendAnswers(int sockP1,int sockP2,TPartieReq player1,TPartieReq player2,TPartie *game){
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

    strcpy(game->player1Name, player1.nomJoueur);
    strcpy(game->player2Name, player2.nomJoueur);
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
  checkSendError(err,sockP1,sockP2,*game);
  err = send(sockP2,&answerP2,sizeof(TPartieRep),0);
  checkSendError(err,sockP2,sockP1,*game);

  return orientation;
}

void endGame(int sock1,int sock2,TPartie game){
  printf("Le joueur X gagne");

  shutdown(sock1, SHUT_RDWR); close(sock1);
  shutdown(sock2, SHUT_RDWR); close(sock2);
  exit(-2);
}
