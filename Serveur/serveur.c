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

  TTypePiece board[BOARD_SIZE][BOARD_SIZE];
  /* {{ONI,KIRIN,KOROPOKKURU,KIRIN,ONI},
      {NULL,NULL,NULL,NULL,NULL},
      {NULL,KODAMA,KODAMA,KODAMA,NULL},
      {NULL,NULL,NULL,NULL,NULL},
      {ONI,KIRIN,KOROPOKKURU,KIRIN,ONI}};
  */

  // Initialisation du tableau; -1 répresente l'absence de pièce
  for(int i=0; i < BOARD_SIZE ; i++){
    for(int j=0; j< BOARD_SIZE ; j++){
      board[i][j]=-1;
    }
  }
  
  // placement des pièces de base
  board[4][4] = board[4][0] = board[0][0] = board[0][4] = ONI;
  board[4][3] = board[4][1] = board[0][1] = board[0][3] = KIRIN;
  board[4][2] = board[0][2] = KOROPOKKURU;
  board[2][1] = board[2][2] = board[2][3] = KODAMA;

  // affichage de l'état de plateau
  for(int i=0; i < BOARD_SIZE ; i++){
    for(int j=0; j< BOARD_SIZE ; j++){
      printf("%d ",board[i][j]);
    }
    printf("\n");
  }

  while(1) {

  }

  close(sockConx);

  return 0;
}
