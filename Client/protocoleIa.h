#ifndef PROTOCOL_IA
#define PROTOCOL_IA

// Permet d'informer du sens à l'IA
typedef struct {
  bool sens; // false = nord ; true = sud
} TInitIa;

// Permet la confirmation d'un coup
typedef struct {
  bool estValide;
} TValidIa;

// Light version of the TCoup defined in protocolYokai.h
typedef struct {
  TCoup typeCoup;               /* Type du coup */
  TTypePiece piece;             /* Info de la piece jouee */
  bool finPartie;               /* Indique si la partie est terminée */
  union {
    TDeplPiece deplPiece;       /* Deplacement de piece */
    TDeposerPiece deposerPiece; /* Placement d'une piece capturee */
  } params;
} TCoupIa;

#endif
