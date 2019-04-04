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
/*
  */
void sendPartieGetRep(int sock, TPartieReq req, TPartieRep* res);

/*
 */
void checkRecvrError(int err,int sock);

/*
 */
void checkSendError(int err,int sock );

/*
 */
void sendCoupGetRep(int sock,TCoupReq reqCoup,TCoupRep *repCoup);

#endif
