#include "libServeur.h"

/* taille du buffer de reception */
#define TAIL_BUF 20
#define MAX_CONNECTED 1023

int main(int argc, char** argv) {
  int sockConx, port, sizeAddr;
  int connexionP1, connexionP2;

  struct sockaddr_in addClient;	/* adresse de la socket client connectee */
  fd_set readSet;              /* variable pour le select */


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

  connexionP1 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  connexionP2 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);

  FD_ZERO(&readSet);
  FD_SET(connexionP1, &readSet);
  FD_SET(connexionP2, &readSet);

  while(1){

  }
  close(sockConx);

  return 0;
}
