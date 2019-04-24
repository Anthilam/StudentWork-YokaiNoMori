#ifndef PROTOCOL_IA
#define PROTOCOL_IA

typedef struct {
  int sens;
}TInitIa;

// Light version of the TCoup defined in protocolYokai.h

typedef struct {
  TCoup      typeCoup;      /* Type du coup : deplacement, placement ou aucune action */
  TPiece     piece;         /* Info de la piece jouee */
  union {
    TDeplPiece deplPiece;        /* Deplacement de piece */
    TDeposerPiece deposerPiece;  /* Placement d'une piece capturee */
  } params;
} TCoupIa;

#endif
