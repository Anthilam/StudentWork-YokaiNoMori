#ifndef LIB_CLIENT_H
#define LIB_CLIENT_H

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

#include "../TCP/fonctionsTCP.h"
#include "../TCP/protocolYokai.h"
#include "protocoleIa.h"

/*  Envoie une requete de type PARTIE au serveur
    params:
      int sock : socket connecté au serveur
      TPartieReq req : stucture envoyer au serveur
      TPartieRep rep : structure stockant la réponse du serveur à la réquete
  */
void sendPartieGetRep(int sock, TPartieReq req, TPartieRep* res);

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

/*  Fonction permettant d'envoyer un coup, celle-ci traite aussi les érreurs de
    réseau et de réponse du serveur ( coup valide ou non ...). La fonction appel aussi
    la fonction de mise à jours du plateau de jeu dans le cas où le coup est valide.
    params:
      int sock : socket connecté au serveur
      TCoupReq reqCoup requête à envoyer
      TCoupRep repCoup réponse du serveur
 */
void sendCoupGetRep(int sock,TCoupReq reqCoup,TCoupRep *repCoup);

/*  Fonction permettant de récupérer le coup de l'adversaire, celle-ci traite
    aussi les érreurs de réseau et de réponse du serveur ( coup valide ou non ...).
    params:
      int sock : socket connecté au serveur
      TCoupRep repCoup : réponse du serveur
 */
void readEnnemyAction(int sock,TCoupIa *coupAdv);

/*
  params:
    int sock :
  */
int receiveIntFromJava(int sock);

/*
  params:
    int sock :
  */
int receiveBoolFromJava(int sock);

/*
  params:
    int sock :
  */
void getCoupFromAI(int sock,TCoupIa *res);


void sendCoupToAI(int sock, TCoupIa coupIa);

/*
  params:

  */
void convertAItoServer(TCoupIa *ai, TCoupReq *req, bool sens, int nbPartie);

void convertServerToAI(TCoupIa *ai, TCoupReq *req, bool end);

void printStrikeIa(TCoupIa coup);

#endif
