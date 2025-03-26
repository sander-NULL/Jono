/*
 * movgen.h
 *
 *  Created on: Mar 30, 2020
 *      Author: sander
 */

#ifndef MOVGEN_H_
#define MOVGEN_H_

#include "bitboards.h"

#define KING	5
#define QUEENS	4
#define ROOKS	3
#define BISHOPS	2
#define KNIGHTS	1
#define PAWNS	0

typedef struct chessposition {
	bitboard pieces[6][2];		// 6 kinds of pieces with 2 colors
	struct {
		int toMove;				// 0 for white to move, 1 for black to move
		int kingCastle[2];		// 1 for castling possible, 0 else
		int queenCastle[2];
		int enPassant;			// En passant possible on file given: 0=a, ..., 7=h, -1 else
		int plies;				// For the 50 move rule
	} states;
	bitboard attackSet[64];		// Encodes attacked squares by piece on specified square
} chessposition;

extern char startfen[]; /* Start position in FEN string */

void initattsets(chessposition *position);
void clearpos(chessposition *position);
int importpos(char* fen, chessposition* position);
int init(void);
void destroymagictbls(void);
bitboard getattsquares(int player, chessposition position);
bitboard getpieces(int player, chessposition *position);
int loudMovGen(chessposition *position, chessposition *moves);
int generatemoves(chessposition position, chessposition *moves);
int getmovenotation(chessposition *before, chessposition *now, char *notation);

#endif /* MOVGEN_H_ */
