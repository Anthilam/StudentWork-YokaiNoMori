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

/* Fonction de création d'une socket de communication type Client
 *  ipMachServ  char*   représente l'ip de la machine serveur avec laquelle on veut communiquer
 *  nPort       int     représente le port avec lequel on va communiquer avec le serveur
 *  return      int     renvoie l'int associer a la socket
 */
int socketClient(char* ipMachServ, unsigned short nPort);

/* Fonction de création d'une socket de connexion type Serveur
 * nPort       int     représente le port avec lequel le serveur va être disponible
 * return      int     renvoie l'int associer a la socket
 */
int socketServeur(unsigned short nPort);

#endif
