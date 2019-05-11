/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * fonctionsTCP.h - TCP functions header used for network communications
 *
 */

#ifndef TCP_H
#define TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

/* Function that creates a client socket
 *  ipMachServ  char*   server ip address
 *  nPort       int     server port
 *  return      int     the socket fd
 */
int socketClient(char* ipMachServ, unsigned short nPort);

/* Function that creates a server socket
 * nPort       int     server port
 * return      int     the socket fd
 */
int socketServeur(unsigned short nPort);

#endif
