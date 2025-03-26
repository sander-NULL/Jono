/*
 * debug.c
 *
 *  Created on: Apr 19, 2020
 *      Author: sander
 */

#include <stdio.h>
#include "bitboards.h"
#include "movgen.h"

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

	population = pos.king[WHITE];
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (pos.king[WHITE] & (UINT64_C(1) << (i * 8 + j)))
				board[i][j] = 'K';
		}
	}

	if ((population & pos.queens[WHITE]) == 0) {
		population |= pos.queens[WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.queens[WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'Q';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.rooks[WHITE]) == 0) {
		population |= pos.rooks[WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.rooks[WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'R';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.bishops[WHITE]) == 0) {
		population |= pos.bishops[WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.bishops[WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'B';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.knights[WHITE]) == 0) {
		population |= pos.knights[WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.knights[WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'N';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pawns[WHITE]) == 0) {
		population |= pos.pawns[WHITE];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pawns[WHITE] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'P';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.king[BLACK]) == 0) {
		population |= pos.king[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.king[BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'k';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.queens[BLACK]) == 0) {
		population |= pos.queens[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.queens[BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'q';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.rooks[BLACK]) == 0) {
		population |= pos.rooks[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.rooks[BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'r';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.bishops[BLACK]) == 0) {
		population |= pos.bishops[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.bishops[BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'b';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.knights[BLACK]) == 0) {
		population |= pos.knights[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.knights[BLACK] & (UINT64_C(1) << (i * 8 + j)))
					board[i][j] = 'n';
			}
		}
	} else
		return EXIT_FAILURE;

	if ((population & pos.pawns[BLACK]) == 0) {
		population |= pos.pawns[BLACK];
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				if (pos.pawns[BLACK] & (UINT64_C(1) << (i * 8 + j)))
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
	if (pos.states.tomove == 0)
		printf("WHITE\t");
	else
		printf("BLACK\t");
	printf("castling: ");
	if (pos.states.kingcastle[WHITE] == 1)
		printf("K");
	if (pos.states.queencastle[WHITE] == 1)
		printf("Q");
	if (pos.states.kingcastle[BLACK] == 1)
		printf("k");
	if (pos.states.queencastle[BLACK] == 1)
		printf("q");
	printf("\ten passant: %d\tplies: %d\n", pos.states.enpassant,
			pos.states.plies);
	fflush(stdout);
	return EXIT_SUCCESS;
}
