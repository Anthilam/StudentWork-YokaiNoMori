#ifndef LIB_SERVEUR_H
#define LIB_SERVEUR_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>

#include "../TCP/fonctionsTCP.h"
#include "../TCP/protocolYokai.h"

#define BOARD_SIZE 5

/* Fonction permettant de vérifié les érreurs RÉSEAU sur la fonction RECV
  params:
    int err : valeur de retour de la fonction recv
    int sock : socket connecté au serveur
 */
void checkRecvrError(int err,int sock);

/* Fonction permettant de vérifié les érreurs RÉSEAU sur la fonction SEND
  params:
    int err : valeur de retour de la fonction recv
    int sock : socket connecté au serveur
 */
void checkSendError(int err,int sock );

int sendAnswers(int sockP1,int sockP2,TPartieReq player1,TPartieReq player2);

#endif
