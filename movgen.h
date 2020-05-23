/*
 * movgen.h
 *
 *  Created on: Mar 30, 2020
 *      Author: sander
 */

#ifndef MOVGEN_H_
#define MOVGEN_H_

#include "bitboards.h"

typedef struct chessposition {
	bitboard king[2];
	bitboard queens[2];
	bitboard rooks[2];
	bitboard bishops[2];
	bitboard knights[2];
	bitboard pawns[2];
	struct {
		int tomove;				/* 0 for white to move, 1 for black to move */
		int kingcastle[2];		/* 1 for castling possible, 0 else */
		int queencastle[2];
		int enpassant;			/* En passant possible on file given: 0=a, ..., 7=h, -1 else */
		int plies;				/* For the 50 move rule */
	} states;
	bitboard attackset[64];		/* Encodes attacked squares by piece on specified square */
} chessposition;

extern char startfen[]; /* Start position in FEN string */

void initattsets(chessposition *position);
void clearpos(chessposition *position);
int importpos(char* fen, chessposition* position);
int init(void);
void destroymagictbls(void);
bitboard getattsquares(int player, chessposition position);
bitboard getpieces(int player, chessposition *position);
int generatemoves(chessposition position, chessposition *moves);

#endif /* MOVGEN_H_ */
