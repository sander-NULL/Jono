/*
 * debug.c
 *
 *  Created on: Apr 19, 2020
 *      Author: sander
 */

#include <stdio.h>
#include "bitboards.h"
#include "movgen.h"

int cmppos(chessposition pos1, chessposition pos2) {	// Returns 0 if positions are equal,
	int i, piece, player;								// negative number if pieces or states differ,
														// positive number i if attackSet[i] differs
	for (player = WHITE; player <= BLACK; player++) {
	{
		for (piece = QUEENS; piece <= KING; piece++)
			if (pos1.pieces[piece][player] != pos2.pieces[piece][player])
				return -10 * piece - player;
		}
		if (pos1.states.kingCastle[player] != pos2.states.kingCastle[player])
			return -60 - player;
		if (pos1.states.queenCastle[player] != pos2.states.queenCastle[player])
			return -62 - player;
	}
	if (pos1.states.enPassant != pos2.states.enPassant)
		return -64;
	if (pos1.states.plies != pos2.states.plies)
		return -65;
	if (pos1.states.toMove != pos2.states.toMove)
		return -66;

	for (i = 0; i <= 63; i++)
		if (pos1.attackSet[i] != pos2.attackSet[i])
			return i;
	return 0;
}

void visualize(bitboard board) { /* Only to visualize bitboards, needed for debugging reasons */
	int i, j;
	char visboard[8][8];

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (board & (UINT64_C(1) << (i * 8 + j)))
				visboard[i][j] = 'x';
			else
				visboard[i][j] = '-';
		}
	}

	for (i = 7; i >= 0; i--) {
		printf("%d  ", i + 1);
		for (j = 0; j < 8; j++)
			printf("%c ", visboard[i][j]);
		printf("\n");
	}
	printf("   a b c d e f g h\n");
}

int vispos(chessposition pos) { /* Visualize position, needed for debugging reasons */

	bitboard population;
	int i, j;
	char board[8][8];

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++)
			board[i][j] = '-';
	}

	population = pos.pieces[KING][WHITE];
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (pos.pieces[KING][WHITE] & (UINT64_C(1) << (i * 8 + j)))
				board[i][j] = 'K';
		}
	}

	if ((population & pos.pieces[QUEENS][WHITE]) == 0) {
		population |= pos.pieces[QUEENS][WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[QUEENS][WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'Q';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[ROOKS][WHITE]) == 0) {
		population |= pos.pieces[ROOKS][WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[ROOKS][WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'R';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[BISHOPS][WHITE]) == 0) {
		population |= pos.pieces[BISHOPS][WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[BISHOPS][WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'B';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[KNIGHTS][WHITE]) == 0) {
		population |= pos.pieces[KNIGHTS][WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[KNIGHTS][WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'N';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[PAWNS][WHITE]) == 0) {
		population |= pos.pieces[PAWNS][WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[PAWNS][WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'P';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[KING][BLACK]) == 0) {
		population |= pos.pieces[KING][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[KING][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'k';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[QUEENS][BLACK]) == 0) {
		population |= pos.pieces[QUEENS][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[QUEENS][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'q';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[ROOKS][BLACK]) == 0) {
		population |= pos.pieces[ROOKS][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[ROOKS][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'r';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[BISHOPS][BLACK]) == 0) {
		population |= pos.pieces[BISHOPS][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[BISHOPS][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'b';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[KNIGHTS][BLACK]) == 0) {
		population |= pos.pieces[KNIGHTS][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[KNIGHTS][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'n';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pieces[PAWNS][BLACK]) == 0) {
		population |= pos.pieces[PAWNS][BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pieces[PAWNS][BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'p';
			}
		}
	} else
		return EXIT_FAILURE;

	for (i = 7; i >= 0; i--) {
		printf("%d  ", i + 1);
		for (j = 0; j < 8; j++)
			printf("%c ", board[i][j]);
		printf("\n");
	}
	printf("   a b c d e f g h\n");
	printf("to move: ");
	if (pos.states.toMove == 0)
		printf("WHITE\t");
	else
		printf("BLACK\t");
	printf("castling: ");
	if (pos.states.kingCastle[WHITE] == 1)
		printf("K");
	if (pos.states.queenCastle[WHITE] == 1)
		printf("Q");
	if (pos.states.kingCastle[BLACK] == 1)
		printf("k");
	if (pos.states.queenCastle[BLACK] == 1)
		printf("q");
	printf("\ten passant: %d\tplies: %d\n", pos.states.enPassant,
			pos.states.plies);
	fflush(stdout);
	return EXIT_SUCCESS;
}
