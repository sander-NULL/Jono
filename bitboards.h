/*
 * bitboards.h
 *
 *  Created on: 30.01.2017
 *      Author: sander
 */

#ifndef BITBOARDS_H_
#define BITBOARDS_H_

#include <stdlib.h>
#include <stdint.h>			/* For 64bit integers */
#include <string.h>

#define WHITE 0
#define BLACK 1

#define A1 UINT64_C(1)				/* 2^0 */
#define B1 UINT64_C(0x2)
#define C1 UINT64_C(0x4)
#define D1 UINT64_C(0x8)
#define E1 UINT64_C(0x10)
#define F1 UINT64_C(0x20)
#define G1 UINT64_C(0x40)
#define H1 UINT64_C(0x80)

#define A2 UINT64_C(0x100)			/* 2^8 */
#define B2 UINT64_C(0x200)
#define C2 UINT64_C(0x400)
#define D2 UINT64_C(0x800)
#define E2 UINT64_C(0x1000)
#define F2 UINT64_C(0x2000)
#define G2 UINT64_C(0x4000)
#define H2 UINT64_C(0x8000)

#define A3 UINT64_C(0x10000)			/* 2^16 */
#define B3 UINT64_C(0x20000)
#define C3 UINT64_C(0x40000)
#define D3 UINT64_C(0x80000)
#define E3 UINT64_C(0x100000)
#define F3 UINT64_C(0x200000)
#define G3 UINT64_C(0x400000)
#define H3 UINT64_C(0x800000)

#define A4 UINT64_C(0x1000000)
#define B4 UINT64_C(0x2000000)
#define C4 UINT64_C(0x4000000)
#define D4 UINT64_C(0x8000000)
#define E4 UINT64_C(0x10000000)
#define F4 UINT64_C(0x20000000)
#define G4 UINT64_C(0x40000000)
#define H4 UINT64_C(0x80000000)

#define A5 UINT64_C(0x100000000)
#define B5 UINT64_C(0x200000000)
#define C5 UINT64_C(0x400000000)
#define D5 UINT64_C(0x800000000)
#define E5 UINT64_C(0x1000000000)
#define F5 UINT64_C(0x2000000000)
#define G5 UINT64_C(0x4000000000)
#define H5 UINT64_C(0x8000000000)

#define A6 UINT64_C(0x10000000000)
#define B6 UINT64_C(0x20000000000)
#define C6 UINT64_C(0x40000000000)
#define D6 UINT64_C(0x80000000000)
#define E6 UINT64_C(0x100000000000)
#define F6 UINT64_C(0x200000000000)
#define G6 UINT64_C(0x400000000000)
#define H6 UINT64_C(0x800000000000)

#define A7 UINT64_C(0x1000000000000)
#define B7 UINT64_C(0x2000000000000)
#define C7 UINT64_C(0x4000000000000)
#define D7 UINT64_C(0x8000000000000)
#define E7 UINT64_C(0x10000000000000)
#define F7 UINT64_C(0x20000000000000)
#define G7 UINT64_C(0x40000000000000)
#define H7 UINT64_C(0x80000000000000)

#define A8 UINT64_C(0x100000000000000)
#define B8 UINT64_C(0x200000000000000)
#define C8 UINT64_C(0x400000000000000)
#define D8 UINT64_C(0x800000000000000)
#define E8 UINT64_C(0x1000000000000000)
#define F8 UINT64_C(0x2000000000000000)
#define G8 UINT64_C(0x4000000000000000)
#define H8 UINT64_C(0x8000000000000000)

#define ACORNER(player) (A1 << (player * 56))	/* Home of players queen rook */
#define HCORNER(player) (H1 << (player * 56)) /* Home of players king rook */

#define CHESSRANK(player, i) (A1 | B1 | C1 | D1 | E1 | F1 | G1 | H1) << (((player + 1) % 2) * 8 * i + player * (56 - 8 * i))	/* i-th rank seen from player */
#define CHESSFILE(i) (A1 | A2 | A3 | A4 | A5 | A6 | A7 | A8) << i		/* (i+1)-st file */

#define FSB(bitboard) __builtin_ffsl(bitboard)								/* Get first set bit, 1 <= FSB <= 64 */
#define POPCNT(bitboard) __builtin_popcountll(bitboard)						/* Get set 1-bits, population count */
#define CB(bitboard, n) bitboard&=~(UINT64_C(1) << (n-1)) 					/* Clear n-th bit */
#define SB(bitboard, n) bitboard|=(UINT64_C(1) << (n-1))					/* Set n-th bit */
#define MIN(a, b) (a<b) ? a : b
#define MAX(a, b) (a>b) ? a : b
#define PAWNINDEX(sqi) sqi - 8										/* Convert square index to pawn array index */
#define GETFILE(sqi) sqi % 8										/* Get file from square */

typedef uint64_t bitboard;

typedef struct occupancies { /* Possible occupancies */
	int size; /* The number of all possible occupancies for a square */
	bitboard occ[4096]; /* The actual occupancy */
} occupancies;

extern occupancies rookocc[64]; /* Possible occupancies for a rook */
extern occupancies bishopocc[64]; /* Possible occupancies for a bishop */
extern bitboard *bishopattset[64];
extern bitboard *rookattset[64];
extern bitboard kingmoves[64];
extern bitboard knightmoves[64];
extern bitboard pawnmoves[2][48];
extern bitboard pawnjumps[2][8]; /* 2 square pawn jumps [player][file] */
extern bitboard pawnattacks[2][48];
extern bitboard epstartsq[2][8]; /* Squares FROM where [player] can en passant capture TO given [file] */
extern bitboard rookmask[64];
extern bitboard bishopmask[64];

void initpawnmoves(void);
void initpawnjumps(void);
void initpawnattacks(void);
void initepstartsq(void);
void initbishopmasks(void);
void initrookmasks(void);
void initbishopocc(void);
void initrookocc(void);
bitboard getbishopattset(bitboard occ, int sqi);
bitboard getrookattset(bitboard occ, int sqi);

#endif /* BITBOARDS_H_ */
