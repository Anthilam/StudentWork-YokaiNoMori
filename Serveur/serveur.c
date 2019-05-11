#include "libServeur.h"
#include "validation.h"

#define TIMEOUT_SEC 6

int main(int argc, char** argv) {
  int sockConx, port, sizeAddr; // variable used for setting up the server
  int connectionP1, connectionP2; // sock to the first client and the second client
  int err; // variable used to check errors

  TPartieReq player1;
  TPartieReq player2;

  // initializating the global game informations
  TPartie game;
  game.gameNumber = 1; // First game
  game.scorePlayer1 = 0; // first player 1 got 0 point
  game.scorePlayer2 = 0; // first player 2 got 0 point
  game.nbStrike = 0; // no strike have been played

  bool bothConnected;
  // true : player1 is oriented south  ; false : player2 is orientied north
  bool player1Sud = false; // does player1 is oriented nord
  struct sockaddr_in addClient;	/* Client socket address */
  /*
    Args checking
   */
  if (argc != 2) {
    printf ("usage : %s port\n", argv[0]);
    return -1;
  }

  /* Socket preparing */
  port  = atoi(argv[1]);
  sockConx = socketServeur(port);
  sizeAddr = sizeof(struct sockaddr_in);

  printf("- Attente de connexion des clients \n");
  connectionP1 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);

  printf("- Client 1 connecté \n");
  printf("- Attente de connexion du second client\n");

  connectionP2 = accept(sockConx,(struct sockaddr *)&addClient,(socklen_t *)&sizeAddr);
  bothConnected = true;

  // initialization from Player1
  err = recv(connectionP1, &player1 ,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP1,connectionP2, game,1);

  // initialization from Player2
  err = recv(connectionP2, &player2 ,sizeof(TPartieReq),0);
  checkRecvrError(err,connectionP2,connectionP1,game,2);

  // send both client and answers to both client
  int res = sendAnswers(connectionP1,connectionP2,player1,player2,&game);

  // Process and save the player orientation
  if(res == -1){

    printf("Erreur sur les demandes de partie");
    return -1;
  }
  else if (res == 0){
    player1Sud = false;
  }
  else{
    player1Sud = true;
  }

  printf("player1Sud :%d\n",player1Sud);

  strcpy(game.player1Name,player1.nomJoueur);
  strcpy(game.player2Name,player2.nomJoueur);

  printf("Joueur 1 : %s\n",game.player1Name) ;
  printf("Joueur 2 : %s\n",game.player2Name) ;

  // setting up the timeout
  struct timeval tv;
  tv.tv_sec = TIMEOUT_SEC;
  tv.tv_usec = 0;

  setsockopt(connectionP1, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));
  setsockopt(connectionP2, SOL_SOCKET,SO_RCVTIMEO, (const char*)&tv, sizeof(struct timeval));

  TCoupReq strikeReq; // Structure used to get a strike from clients
  TCoupRep strikeRep; // Structure used to send a strike to clients

  bool validation; // boolean used to get the validationCoup() result

  // board initialization
  initialiserPartie();

  /* while both client are conneted and the server and
     only one game have been played
    */
  while(bothConnected && game.gameNumber < 3) {

    TPropCoup propCoup; // variable used to stock strike property

    // player1 is first first game(oriented nord)
    if((player1Sud == true && game.gameNumber == 1) ||
    ( player1Sud == false && game.gameNumber == 2 )){

      // Player 1 is playing
      err = recv(connectionP1,&strikeReq,sizeof(TCoupReq),0);
      checkRecvrError(err,connectionP1,connectionP2,game,1);

      // checking strike validation
      validation = validationCoup(1,strikeReq,&propCoup);

      // prepare strike answer
      prepareStrikeAnswer(connectionP1,connectionP2,validation,strikeReq,propCoup,&strikeRep);

      // If there is more than 60 strike played, the match is "NUL"
      if(game.nbStrike >= 60){
        propCoup = NUL;
      }

      // sending validation and strike
      sendStrikeAnswer(connectionP1,connectionP2,strikeReq,strikeRep,game,1);

      // Checking if the game should continue or not
      if(strikeRep.propCoup != CONT){
        // the game is ended
        // initializating the new game
        initialiserPartie();
        // associating the score
        switch(strikeRep.propCoup){

          case GAGNE : game.scorePlayer1++;
            break;
          case PERDU : game.scorePlayer2++;
            break;
          default : // case NUL no one win a point
            break;
        }

        game.gameNumber++;
        game.nbStrike = 0;

        // if two game have been played, end of the game
        if(game.gameNumber == 3 ){
          endGame(connectionP1,connectionP2,game);
        }

        printf("*** Début de la deuxième partie ***\n");
      }
      else{
        // Player 2 is playing
        err = recv(connectionP2,&strikeReq,sizeof(TCoupReq),0);
        checkRecvrError(err,connectionP2,connectionP1,game,2);

        // checking strike validation
        validation = validationCoup(2,strikeReq,&propCoup);

        // prepare strike answer
        prepareStrikeAnswer(connectionP2,connectionP1,validation,strikeReq,propCoup,&strikeRep);

        // If there is more than 60 strike played, the match is "NUL"
        if(game.nbStrike >= 60){
          propCoup = NUL;
        }

        // sending validation and strike
        sendStrikeAnswer(connectionP2,connectionP1,strikeReq,strikeRep,game,2);

        // Checking if the game should continue or not
        if(strikeRep.propCoup != CONT){
          // the game is ended
          // initializating the new game
          initialiserPartie();
          // associating the score
          switch(strikeRep.propCoup){

            case GAGNE : game.scorePlayer2++;
              break;
            case PERDU : game.scorePlayer1++;
              break;
            default : // case NUL no one win a point
              break;
          }

          game.gameNumber++;
          game.nbStrike = 0;

          // if two game have been played, end of the game
          if(game.gameNumber == 3 ){
            endGame(connectionP1,connectionP2,game);
          }

          printf("*** Début de la deuxième partie ***\n");
        }
      }
      game.nbStrike++;
    }
    // player1 is second first game (oriented south)
    else{

      // Player 2 is playing
      err = recv(connectionP2,&strikeReq,sizeof(TCoupReq),0);
      checkRecvrError(err,connectionP2,connectionP1,game,2);

      // checking strike validation
      validation = validationCoup(1,strikeReq,&propCoup);

      // prepare strike answer
      prepareStrikeAnswer(connectionP2,connectionP1,validation,strikeReq,propCoup,&strikeRep);

      // If there is more than 60 strike played, the match is "NUL"
      if(game.nbStrike >= 60){
        propCoup = NUL;
      }

      // sending validation and strike
      sendStrikeAnswer(connectionP2,connectionP1,strikeReq,strikeRep,game,2);

      // Checking if the game should continue or not
      if(strikeRep.propCoup != CONT){
        // the game is ended
        // initializating the new game
        initialiserPartie();
        // associating the score
        switch(strikeRep.propCoup){
          case GAGNE : game.scorePlayer2++;
            break;
          case PERDU : game.scorePlayer1++;
            break;
          default : // case NUL no one win a point
            break;
        }

        game.gameNumber++;
        game.nbStrike = 0;

        // if two game have been played, end of the game
        if(game.gameNumber == 3 ){
          endGame(connectionP2,connectionP1,game);
        }

        printf("*** Début de la deuxième partie ***\n");
      }
      else{

        // Player 1 is playing
        err = recv(connectionP1,&strikeReq,sizeof(TCoupReq),0);
        checkRecvrError(err,connectionP1,connectionP2,game,1);

        // checking strike validation
        validation = validationCoup(2,strikeReq,&propCoup);

        // prepare strike answer
        prepareStrikeAnswer(connectionP1,connectionP2,validation,strikeReq,propCoup,&strikeRep);

        // If there is more than 60 strike played, the match is "NUL"
        if(game.nbStrike >= 60){
          propCoup = NUL;
        }

        // sending validation and strike
        sendStrikeAnswer(connectionP1,connectionP2,strikeReq,strikeRep,game,1);

        // Checking if the game should continue or not
        if(strikeRep.propCoup != CONT){
          // the game is ended
          // initializating the new game
          initialiserPartie();
          // associating the score
          switch(strikeRep.propCoup){
            case GAGNE : game.scorePlayer1++;
              break;
            case PERDU : game.scorePlayer2++;
              break;
            default : // case NUL no one win a point
              break;
          }

          game.gameNumber++;
          game.nbStrike = 0;

          // if two game have been played, end of the game
          if(game.gameNumber == 3 ){
            endGame(connectionP2,connectionP1,game);
          }
          printf("*** Début de la deuxième partie ***\n");
        }
      }
      game.nbStrike++;
    }
  }

  // closing all socket
  close(connectionP1);
  close(connectionP2);
  close(sockConx);

  return 0;
}
