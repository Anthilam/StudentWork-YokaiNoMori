/* BOISSON Romain - GUY Timothée
 *
 * Yokai No-Mori project - UFR-ST 2019
 *
 * protocoleIa.h - AI protocol header
 *
 */

#ifndef PROTOCOL_IA
#define PROTOCOL_IA

// Structure used to initialize the AI
typedef struct {
  bool sens; // false = north; true = south
} TInitIa;

// Light version of the TCoup structure defined in protocolYokai.h
// Used to communicate with the AI
typedef struct {
  int finPartie;                /* Game number */
  TCoup typeCoup;               /* Move type */
  TTypePiece piece;             /* Piece's data */

  union {
    TDeplPiece deplPiece;       /* The move movement */
    TDeposerPiece deposerPiece; /* The move placement */
  } params;
} TCoupIa;

#endif
