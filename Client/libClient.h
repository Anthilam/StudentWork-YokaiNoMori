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

/*  Function to send a game request to the server

    params:
      int sock : socket connected to the server
      TPartieReq req  : stucture send to the server
      TPartieRep *rep : structure used to read the server's answer
  */
void sendPartieGetRep(int sock, TPartieReq req, TPartieRep *res);

/* Function to check the recv's errors

  params:
    int err : recv's return value
    int sock : socket connected to the server
 */
void checkRecvrError(int err,int sock);

/* Function to check the send's errors

  params:
    int err : send's return value
    int sock : socket connected to the server
 */
void checkSendError(int err,int sock );

/*  Function to send a strike to the server, and process the answer

    params:
      int sock : socket connected to the server
      TCoupReq reqCoup  : request to send
      TCoupRep *repCoup : server's answer
 */
void sendCoupGetRep(int sock,TCoupReq reqCoup,TCoupRep *repCoup);

/*  Function to send a strike to the server, and process the answer

    params:
      int sock : socket connected to the server
      TCoupRep repCoup : opponent's strike
 */
void readEnnemyAction(int sock,TCoupIa *coupAdv);

/*  Function to read a int from a java server

    params:
      int sock : socket connected to the java server

    return : the int value
  */
int receiveIntFromJava(int sock);

/*  Funtion to read a boolean from a java server

    params:
      int sock : socket connected to the server

    return : the boolean value
  */
int receiveBoolFromJava(int sock);

/*  Function to read a strike from the AI ( java server )

    params:
      int sock : socket connected to the AI ( java server )
      TCoupIa *res : the strike
  */
void getCoupFromAI(int sock,TCoupIa *res);

/*  Function to send a strike to the AI ( java server )

    params:
      int sock : socket connected to the AI
      TCoupIa : the strike to send
  */
void sendCoupToAI(int sock, TCoupIa coupIa);

/*  Function to convert a AI shape strike to a server shape strike

    params:
      TCoupIa *ai : the AI shape strike
      TCoupReq *req ; the convertion result into a server shape strike
      bool sens : the orientation in the game
      int nbPartie : the game number
  */
void convertAItoServer(TCoupIa *ai, TCoupReq *req, bool sens, int nbPartie);

/*  Function to convert a serveur shape strike to a AI shape strike

    params:
      TCoupIa *ai : the convertion result into a AI shape strike
      TCoupReq *req ; the server shape strike
      bool end : boolean representing the game's end
  */
void convertServerToAI(TCoupIa *ai, TCoupReq *req, bool end);

/*  Function to print a AI shape strike

    params:
      TCoupIa coup : the strike to print
  */
void printStrikeIa(TCoupIa coup);

/*  Function to print a serveur shape strike

    params:
      TCoupReq coup : the strike to print
  */
void printStrikeServer(TCoupReq coup);

#endif
