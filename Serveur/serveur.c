#include "libServeur.h"
#include "validation.h"

#define TIMEOUT_SEC 6

int main(int argc, char** argv) {
  int sockConx, port, sizeAddr;
  int connectionP1, connectionP2;
  int numeroPartie = 1;
  int err;

  TPartieReq player1;
  TPartieReq player2;

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

  connectionP1 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  connectionP2 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  bothConnected = true;

  // Détermination de l'orientation
  err = recv(connectionP1, &player1,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP1);
  err = recv(connectionP2, &player2,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP2);


  int res = sendAnswers(connectionP1,connectionP2,player1,player2);
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

  while(bothConnected) {
    printf("Partie %d\n",numeroPartie);
    // player1 is first first game(oriented nord)
    if(orientation == true){

    }
    // player1 is second first game (oriented south)
    else{

    }
  }

  close(sockConx);

  return 0;
}
