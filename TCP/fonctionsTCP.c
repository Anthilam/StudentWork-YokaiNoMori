/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * fonctionsTCP.c - TCP functions used for network communications
 *
 */

#include "fonctionsTCP.h"

int socketClient(char* ipMachServ, unsigned short nPort){
  socklen_t sizeAdd;

  struct sockaddr_in addSockServ ;

  int sock, err;
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("* Error while creating client socket");
    exit(-1);
  }

  // Initialize socket
  addSockServ.sin_family = AF_INET;
  err = inet_aton(ipMachServ, &addSockServ.sin_addr);
  if (err == 0) {
    perror("* Error while obtaining server ip");
    close(sock);
    exit(-2);
  }

  addSockServ.sin_port = htons(nPort);
  bzero(addSockServ.sin_zero, 8);

  sizeAdd = sizeof(struct sockaddr_in);

  // Server connection
  err = connect(sock, (struct sockaddr *)&addSockServ, sizeAdd);
  if (err < 0) {
    perror("* Error while connecting to the server");
    close(sock);
    exit(-3);
  }

  return sock;
}

int socketServeur(unsigned short nPort){
  int sizeAddr,err;
  int option = 1;

  struct sockaddr_in addServ;

  int sockValue = socket(AF_INET, SOCK_STREAM, 0);
  if (sockValue < 0) {
    perror("* Error while creating server socket");
    exit(-1);
  }

  // Initialize server socket
  addServ.sin_family = AF_INET;
  addServ.sin_port = htons(nPort);  // Convert to big endian
  addServ.sin_addr.s_addr = INADDR_ANY;

  bzero(addServ.sin_zero, 8);

  sizeAddr = sizeof(struct sockaddr_in);

  // Set the address to reusable
  if(setsockopt(sockValue, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(int)) < 0) {
    perror("* Error : setsockopt server failed");
    close(sockValue);
    exit(-2);
  }

  // Bind the address to the socket
  err = bind(sockValue, (struct sockaddr *)&addServ, sizeAddr);
  if (err < 0) {
    perror("* Error while binding server address");
    close(sockValue);
    exit(-3);
  }

  // Listen
  err = listen(sockValue, 1);
  if (err < 0) {
    perror("* Error while server is listening");
    close(sockValue);
    exit(-4);
  }

  return sockValue;
}
