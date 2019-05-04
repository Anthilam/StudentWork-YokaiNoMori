#ifndef PROTOCOL_IA
#define PROTOCOL_IA

// Structure used to
typedef struct {
  bool sens; // false = nord ; true = sud
} TInitIa;

// Light version of the TCoup defined in protocolYokai.h
typedef struct {
  int finPartie;               /* Indique si la partie est terminée 0 = non 1 = oui */
  TCoup typeCoup;               /* Type du coup */
  TTypePiece piece;             /* Info de la piece jouee */
  union {
    TDeplPiece deplPiece;       /* Deplacement de piece */
    TDeposerPiece deposerPiece; /* Placement d'une piece capturee */
  } params;
} TCoupIa;

#endif
