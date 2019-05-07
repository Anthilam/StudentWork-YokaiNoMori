#include "libServeur.h"
#include "validation.h"

#define TIMEOUT_SEC 6

int main(int argc, char** argv) {
  int sockConx, port, sizeAddr;
  int connectionP1, connectionP2;
  int err;


  TPartieReq player1;
  TPartieReq player2;

  TPartie game;
  game.gameNumber = 1;
  game.scorePlayer1 = 0;
  game.scorePlayer2 = 0;

  bool bothConnected;
  // true : player1 is oriented south  ; false : player2 is orientied north
  bool orientation = false; // does player1 is oriented nord
  struct sockaddr_in addClient;	/* adresse de la socket client connectee */
  /*
  * verification des arguments
  */
  if (argc != 2) {
    printf ("usage : %s port\n", argv[0]);
    return -1;
  }

  /* Préparation de la socket de connexion */
  port  = atoi(argv[1]);
  sockConx = socketServeur(port);

  sizeAddr = sizeof(struct sockaddr_in);

  printf("- Attente de connexion des clients \n");
  connectionP1 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  printf("- Client 1 connecté \n");
  printf("- Attente de connexion du second client\n");
  connectionP2 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  bothConnected = true;

  // Détermination de l'orientation
  err = recv(connectionP1, &player1 ,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP1,connectionP2, game);
  err = recv(connectionP2, &player2 ,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP2,connectionP1,game);


  int res = sendAnswers(connectionP1,connectionP2,player1,player2,&game);

  if(res == -1){
    printf("Erreur sur les demandes de partie");
    return -1;
  }else if (res == 0){
    orientation = false;
  }else{
    orientation = true;
  }

  // setting up the timeout
  struct timeval tv;
  tv.tv_sec = TIMEOUT_SEC;
  tv.tv_usec = 0;
  setsockopt(connectionP1, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
  setsockopt(connectionP2, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

  TCoupReq reqPlayer1;
  TCoupRep repPlayer1;

  TCoupReq reqPlayer2;
  TCoupRep repPlayer2;

  bool validation;
  // game initialization
  initialiserPartie();

  while(bothConnected) {
    TPropCoup propCoup;
    printf("Partie %d\n",game.gameNumber);
    // player1 is first first game(oriented nord)
    if(orientation == true && game.gameNumber == 1){
      err = recv(connectionP1,&reqPlayer1,sizeof(TCoupReq),0);
      checkRecvrError(err,connectionP1,connectionP2,game);
      validation = validationCoup(1,reqPlayer1,&propCoup);
      
    }
    // player1 is second first game (oriented south)
    else{

    }
  }

  close(sockConx);

  return 0;
}
