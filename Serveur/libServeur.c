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
  // 1 : player1 is oriented sud  ; 0 : player2 is orientied nord
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
    answerP1.validSensTete = OK;
    answerP1.err = ERR_OK;
    answerP2.err = ERR_OK;

    strcpy(answerP1.nomAdvers, player2.nomJoueur);
    strcpy(answerP2.nomAdvers, player1.nomJoueur);

    if(player1.piece == SUD){
      printf("player 1 demande sud \n");
      orientation = 1;
      if(player2.piece == SUD){
        answerP2.validSensTete = KO;
      }
      else{
        answerP2.validSensTete = OK;
      }
    }
    else {
      printf("player 1 demande nord \n");
      orientation = 0;
      if(player2.piece == SUD){
        answerP2.validSensTete = OK;
      }
      else{
        answerP2.validSensTete = KO;
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

void prepareStrikeAnswer(int sock1,int sock2,bool validation, TCoupReq strike,TPropCoup strikeProp,TCoupRep *strikeAns){
  if(validation == true){

    strikeAns -> err = ERR_OK;
    strikeAns -> validCoup = VALID;
  }
  else{

    strikeAns -> err = ERR_COUP;
    strikeAns -> validCoup = TRICHE;
  }

  strikeAns->propCoup = strikeProp;
}

void sendStrikeAnswer(int sock1,int sock2, TCoupReq strike,TCoupRep strikeAns,TPartie game){
  int err;
  // validation to the player
  err = send(sock1,&strikeAns,sizeof(TCoupRep),0);
  checkSendError(err,sock1,sock2,game);

  //validation to the other player
  err = send(sock2,&strikeAns,sizeof(TCoupRep),0);
  checkSendError(err,sock2,sock1,game);

  // sending to the strike to the other player if the game should continue
  if(strikeAns.propCoup == CONT){
    err = send(sock2,&strike,sizeof(TCoupReq),0);
    checkSendError(err,sock2,sock1,game);
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
