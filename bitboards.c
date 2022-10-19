/*
 * bitboards.c
 *
 *  Created on: 17.02.2017
 *      Author: sander
 */

#include "bitboards.h"

occupancies rookocc[64]; /* Possible occupancies for a rook */
occupancies bishopocc[64]; /* Possible occupancies for a bishop */

bitboard *bishopattset[64];
bitboard *rookattset[64];

bitboard kingmoves[64] = {
A2 | B1 | B2, /* King on A1 */
A1 | A2 | B2 | C1 | C2, /* King on B1 */
B1 | B2 | C2 | D1 | D2, /* King on C1 */
C1 | C2 | D2 | E1 | E2, /* King on D1 */
D1 | D2 | E2 | F1 | F2, /* King on E1 */
E1 | E2 | F2 | G1 | G2, /* King on F1 */
F1 | F2 | G2 | H1 | H2, /* King on G1 */
G1 | G2 | H2, /* King on H1 */

A1 | A3 | B1 | B2 | B3, /* King on A2 */
A1 | A2 | A3 | B1 | B3 | C1 | C2 | C3, /* King on B2 */
B1 | B2 | B3 | C1 | C3 | D1 | D2 | D3, /* King on C2 */
C1 | C2 | C3 | D1 | D3 | E1 | E2 | E3, /* King on D2 */
D1 | D2 | D3 | E1 | E3 | F1 | F2 | F3, /* King on E2 */
E1 | E2 | E3 | F1 | F3 | G1 | G2 | G3, /* King on F2 */
F1 | F2 | F3 | G1 | G3 | H1 | H2 | H3, /* King on G2 */
G1 | G2 | G3 | H1 | H3, /* King on H2 */

A2 | A4 | B2 | B3 | B4, /* King on A3 */
A2 | A3 | A4 | B2 | B4 | C2 | C3 | C4, /* King on B3 */
B2 | B3 | B4 | C2 | C4 | D2 | D3 | D4, /* King on C3 */
C2 | C3 | C4 | D2 | D4 | E2 | E3 | E4, /* King on D3 */
D2 | D3 | D4 | E2 | E4 | F2 | F3 | F4, /* King on E3 */
E2 | E3 | E4 | F2 | F4 | G2 | G3 | G4, /* King on F3 */
F2 | F3 | F4 | G2 | G4 | H2 | H3 | H4, /* King on G3 */
G2 | G3 | G4 | H2 | H4, /* King on H3 */

A3 | A5 | B3 | B4 | B5, /* King on A4 */
A3 | A4 | A5 | B3 | B5 | C3 | C4 | C5, /* King on B4 */
B3 | B4 | B5 | C3 | C5 | D3 | D4 | D5, /* King on C4 */
C3 | C4 | C5 | D3 | D5 | E3 | E4 | E5, /* King on D4 */
D3 | D4 | D5 | E3 | E5 | F3 | F4 | F5, /* King on E4 */
E3 | E4 | E5 | F3 | F5 | G3 | G4 | G5, /* King on F4 */
F3 | F4 | F5 | G3 | G5 | H3 | H4 | H5, /* King on G4 */
G3 | G4 | G5 | H3 | H5, /* King on H4 */

A4 | A6 | B4 | B5 | B6, /* King on A5 */
A4 | A5 | A6 | B4 | B6 | C4 | C5 | C6, /* King on B5 */
B4 | B5 | B6 | C4 | C6 | D4 | D5 | D6, /* King on C5 */
C4 | C5 | C6 | D4 | D6 | E4 | E5 | E6, /* King on D5 */
D4 | D5 | D6 | E4 | E6 | F4 | F5 | F6, /* King on E5 */
E4 | E5 | E6 | F4 | F6 | G4 | G5 | G6, /* King on F5 */
F4 | F5 | F6 | G4 | G6 | H4 | H5 | H6, /* King on G5 */
G4 | G5 | G6 | H4 | H6, /* King on H5 */

A5 | A7 | B5 | B6 | B7, /* King on A6 */
A5 | A6 | A7 | B5 | B7 | C5 | C6 | C7, /* King on B6 */
B5 | B6 | B7 | C5 | C7 | D5 | D6 | D7, /* King on C6 */
C5 | C6 | C7 | D5 | D7 | E5 | E6 | E7, /* King on D6 */
D5 | D6 | D7 | E5 | E7 | F5 | F6 | F7, /* King on E6 */
E5 | E6 | E7 | F5 | F7 | G5 | G6 | G7, /* King on F6 */
F5 | F6 | F7 | G5 | G7 | H5 | H6 | H7, /* King on G6 */
G5 | G6 | G7 | H5 | H7, /* King on H6 */

A6 | A8 | B6 | B7 | B8, /* King on A7 */
A6 | A7 | A8 | B6 | B8 | C6 | C7 | C8, /* King on B7 */
B6 | B7 | B8 | C6 | C8 | D6 | D7 | D8, /* King on C7 */
C6 | C7 | C8 | D6 | D8 | E6 | E7 | E8, /* King on D7 */
D6 | D7 | D8 | E6 | E8 | F6 | F7 | F8, /* King on E7 */
E6 | E7 | E8 | F6 | F8 | G6 | G7 | G8, /* King on F7 */
F6 | F7 | F8 | G6 | G8 | H6 | H7 | H8, /* King on G7 */
G6 | G7 | G8 | H6 | H8, /* King on H7 */

A7 | B7 | B8, /* King on A8 */
A7 | A8 | B7 | C7 | C8, /* King on B8 */
B7 | B8 | C7 | D7 | D8, /* King on C8 */
C7 | C8 | D7 | E7 | E8, /* King on D8 */
D7 | D8 | E7 | F7 | F8, /* King on E8 */
E7 | E8 | F7 | G7 | G8, /* King on F8 */
F7 | F8 | G7 | H7 | H8, /* King on G8 */
G7 | G8 | H7 /* King on H8 */
};

bitboard knightmoves[64] = {
B3 | C2, /* Knight on A1 */
A3 | C3 | D2, /* Knight on B1 */
A2 | B3 | D3 | E2, /* Knight on C1 */
B2 | C3 | E3 | F2, /* Knight on D1 */
C2 | D3 | F3 | G2, /* Knight on E1 */
D2 | E3 | G3 | H2, /* Knight on F1 */
E2 | F3 | H3, /* Knight on G1 */
F2 | G3, /* Knight on H1 */

B4 | C1 | C3, /* Knight on A2 */
A4 | C4 | D1 | D3, /* Knight on B2 */
A1 | A3 | B4 | D4 | E1 | E3, /* Knight on C2 */
B1 | B3 | C4 | E4 | F1 | F3, /* Knight on D2 */
C1 | C3 | D4 | F4 | G1 | G3, /* Knight on E2 */
D1 | D3 | E4 | G4 | H1 | H3, /* Knight on F2 */
E1 | E3 | F4 | H4, /* Knight on G2 */
F1 | F3 | G4, /* Knight on H2 */

B1 | B5 | C2 | C4, /* Knight on A3 */
A1 | A5 | C1 | C5 | D2 | D4, /* Knight on B3 */
A2 | A4 | B1 | B5 | D1 | D5 | E2 | E4, /* Knight on C3 */
B2 | B4 | C1 | C5 | E1 | E5 | F2 | F4, /* Knight on D3 */
C2 | C4 | D1 | D5 | F1 | F5 | G2 | G4, /* Knight on E3 */
D2 | D4 | E1 | E5 | G1 | G5 | H2 | H4, /* Knight on F3 */
E2 | E4 | F1 | F5 | H1 | H5, /* Knight on G3 */
F2 | F4 | G1 | G5, /* Knight on H3 */

B2 | B6 | C3 | C5, /* Knight on A4 */
A2 | A6 | C2 | C6 | D3 | D5, /* Knight on B4 */
A3 | A5 | B2 | B6 | D2 | D6 | E3 | E5, /* Knight on C4 */
B3 | B5 | C2 | C6 | E2 | E6 | F3 | F5, /* Knight on D4 */
C3 | C5 | D2 | D6 | F2 | F6 | G3 | G5, /* Knight on E4 */
D3 | D5 | E2 | E6 | G2 | G6 | H3 | H5, /* Knight on F4 */
E3 | E5 | F2 | F6 | H2 | H6, /* Knight on G4 */
F3 | F5 | G2 | G6, /* Knight on H4 */

B3 | B7 | C4 | C6, /* Knight on A5 */
A3 | A7 | C3 | C7 | D4 | D6, /* Knight on B5 */
A4 | A6 | B3 | B7 | D3 | D7 | E4 | E6, /* Knight on C5 */
B4 | B6 | C3 | C7 | E3 | E7 | F4 | F6, /* Knight on D5 */
C4 | C6 | D3 | D7 | F3 | F7 | G4 | G6, /* Knight on E5 */
D4 | D6 | E3 | E7 | G3 | G7 | H4 | H6, /* Knight on F5 */
E4 | E6 | F3 | F7 | H3 | H7, /* Knight on G5 */
F4 | F6 | G3 | G7, /* Knight on H5 */

B4 | B8 | C5 | C7, /* Knight on A6 */
A4 | A8 | C4 | C8 | D5 | D7, /* Knight on B6 */
A5 | A7 | B4 | B8 | D4 | D8 | E5 | E7, /* Knight on C6 */
B5 | B7 | C4 | C8 | E4 | E8 | F5 | F7, /* Knight on D6 */
C5 | C7 | D4 | D8 | F4 | F8 | G5 | G7, /* Knight on E6 */
D5 | D7 | E4 | E8 | G4 | G8 | H5 | H7, /* Knight on F6 */
E5 | E7 | F4 | F8 | H4 | H8, /* Knight on G6 */
F5 | F7 | G4 | G8, /* Knight on H6 */

B5 | C6 | C8, /* Knight on A7 */
A5 | C5 | D6 | D8, /* Knight on B7 */
A6 | A8 | B5 | D5 | E6 | E8, /* Knight on C7 */
B6 | B8 | C5 | E5 | F6 | F8, /* Knight on D7 */
C6 | C8 | D5 | F5 | G6 | G8, /* Knight on E7 */
D6 | D8 | E5 | G5 | H6 | H8, /* Knight on F7 */
E6 | E8 | F5 | H5, /* Knight on G7 */
F6 | F8 | G5, /* Knight on H7 */

B6 | C7, /* Knight on A8 */
A6 | C6 | D7, /* Knight on B8 */
A7 | B6 | D6 | E7, /* Knight on C8 */
B7 | C6 | E6 | F7, /* Knight on D8 */
C7 | D6 | F6 | G7, /* Knight on E8 */
D7 | E6 | G6 | H7, /* Knight on F8 */
E7 | F6 | H6, /* Knight on G8 */
F7 | G6 /* Knight on H8 */
};

bitboard pawnmoves[2][48];

bitboard pawnjumps[2][8]; /* 2 square pawn jumps [player][file] */

bitboard pawnattacks[2][48];

bitboard epstartsq[2][8]; /* Squares FROM where [player] can en passant capture TO given [file] */

bitboard rookmask[64];

bitboard bishopmask[64];

void initpawnmoves(void) { /* Initializes pawn moves without the special 2-squares pawn jump in the beginning */
	int i;
	for (i = 0; i < 48; i++) {
		pawnmoves[WHITE][i] = UINT64_C(1) << (16 + i);
		pawnmoves[BLACK][i] = UINT64_C(1) << i;
	}
}

void initpawnjumps(void) { /* Initializes the special 2-squares pawn jumps in the beginning */
	int i;
	for (i = 0; i < 8; i++)
		pawnjumps[WHITE][i] |= UINT64_C(1) << (24 + i);
	for (i = 0; i < 8; i++)
		pawnjumps[BLACK][i] |= UINT64_C(1) << (i + 32);
}

void initpawnattacks(void) {
	int i, j;
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 7; j++) {
			pawnattacks[WHITE][i * 8 + j] = UINT64_C(1)
					<< ((i + 2) * 8 + j + 1);
			pawnattacks[BLACK][i * 8 + j] = UINT64_C(1) << (i * 8 + j + 1);
		}
		pawnattacks[WHITE][i * 8 + 7] = UINT64_C(1) << ((i + 2) * 8 + 6);
		pawnattacks[BLACK][i * 8 + 7] = UINT64_C(1) << ((i * 8) + 6);
	}

	for (i = 0; i < 6; i++) {
		for (j = 1; j < 7; j++) {
			pawnattacks[WHITE][i * 8 + j] |= UINT64_C(1)
					<< ((i + 2) * 8 + j - 1);
			pawnattacks[BLACK][i * 8 + j] |= UINT64_C(1) << (i * 8 + j - 1);
		}
	}
}

void initepstartsq(void) {
	int file;
	for (file = 0; file <= 7; file++) {
		epstartsq[WHITE][file] = pawnattacks[BLACK][32 + file];
		epstartsq[BLACK][file] = pawnattacks[WHITE][8 + file];
	}
}

void initbishopmasks(void) {
	int i, j, k, bound;

	for (i = 0; i < 64; i++)
		bishopmask[i] = 0; /* Clear the masks first */

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			bound = MIN(7 - i, 7 - j);
			for (k = 1; k < bound; k++)
				SB(bishopmask[8 * i + j], 8 * (i + k) + j + k + 1); /* Set mask along the diagonal north-east */
			bound = MIN(i, j);
			for (k = 1; k < bound; k++)
				SB(bishopmask[8 * i + j], 8 * (i - k) + j - k + 1); /* Set mask along the diagonal south-west */
			bound = MIN(7 - i, j);
			for (k = 1; k < bound; k++)
				SB(bishopmask[8 * i + j], 8 * (i + k) + j - k + 1); /* Set mask along the diagonal north-west */
			bound = MIN(i, 7 - j);
			for (k = 1; k < bound; k++)
				SB(bishopmask[8 * i + j], 8 * (i - k) + j + k + 1); /* Set mask along the diagonal south-east */
		}
	}
}

void initrookmasks(void) {
	int i, j, k;

	for (i = 0; i < 64; i++)
		rookmask[i] = 0; /* Clear the masks first */

	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			for (k = 1; k < 7 - j; k++)
				SB(rookmask[8 * i + j], 8 * i + j + k + 1); /* Set mask along the upper rank */
			for (k = 1; k < j; k++)
				SB(rookmask[8 * i + j], 8 * i + j - k + 1); /* Set mask along the lower rank */

			for (k = 1; k < 7 - i; k++)
				SB(rookmask[8 * i + j], 8 * (i + k) + j + 1); /* Set mask along the upper file */
			for (k = 1; k < i; k++)
				SB(rookmask[8 * i + j], 8 * (i - k) + j + 1); /* Set mask along the lower file */
		}
	}
}

void initbishopocc(void) {
	int i, j, sqi, popcount, size, setbit[12] = {0};
	bitboard tmp;

	initbishopmasks(); /* Initialize the bishopmasks */

	for (sqi = 0; sqi < 64; sqi++) {
		popcount = POPCNT(bishopmask[sqi]); /* How many bits are set? */

		size = 1;
		for (i = 1; i <= popcount; i++)
			size *= 2; /* size = 2^popcount */
		bishopocc[sqi].size = size; /* This many occupancies for the square */

		tmp = bishopmask[sqi]; /* Fill setbit[] */
		for (i = 0; i < popcount; i++) {
			setbit[i] = FSB(tmp); /* setbit[i] contains index of (i+1)-th set bit in mask */
			CB(tmp, setbit[i]);
		}

		for (i = 0; i < size; i++) {
			tmp = 0;
			for (j = 0; j < popcount; j++)
				if ((i & (1 << j)) != 0)
					SB(tmp, setbit[j]); /* Map the integer i to the mask */
			bishopocc[sqi].occ[i] = tmp;
		}
	}
}

void initrookocc(void) { /* Initializes the occupancies for the rook */
	int i, j, sqi, popcount, size, setbit[12] = {0};
	bitboard tmp;

	initrookmasks(); /* Initialize the rookmasks */

	for (sqi = 0; sqi < 64; sqi++) {
		popcount = POPCNT(rookmask[sqi]); /* How many bits are set? */

		size = 1;
		for (i = 1; i <= popcount; i++)
			size *= 2; /* size = 2^popcount */
		rookocc[sqi].size = size; /* This many occupancies for the square */

		tmp = rookmask[sqi]; /* Fill setbit[] */
		for (i = 0; i < popcount; i++) {
			setbit[i] = FSB(tmp); /* setbit[i] contains index of (i+1)-th set bit in mask */
			CB(tmp, setbit[i]);
		}

		for (i = 0; i < size; i++) {
			tmp = 0;
			for (j = 0; j < popcount; j++)
				if ((i & (1 << j)) != 0)
					SB(tmp, setbit[j]); /* Map the integer i to the mask */
			rookocc[sqi].occ[i] = tmp;
		}
	}
}

bitboard getbishopattset(bitboard occ, int sqi) {
	int i = sqi;
	int n = sqi / 8 - sqi % 8; /* Diagonal (NE and SW) satisfies y-x=n */
	bitboard attset = 0;

	do {
		attset |= UINT64_C(1) << i;
		i += 9;
	} while ((occ & (UINT64_C(1) << (i - 9))) == 0 && (i / 8 - i % 8) == n
			&& i < 64); /* Check the north-eastern ray */

	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i -= 9;
	} while ((occ & (UINT64_C(1) << (i + 9))) == 0 && (i / 8 - i % 8) == n
			&& i >= 0); /* Check the south-western ray */

	n = sqi / 8 + sqi % 8; /* Diagonal (NW and SE) satisfies y+x=n */
	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i += 7;
	} while ((occ & (UINT64_C(1) << (i - 7))) == 0 && (i / 8 + i % 8) == n
			&& i < 64); /* Check the north-western ray */

	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i -= 7;
	} while ((occ & (UINT64_C(1) << (i + 7))) == 0 && (i / 8 + i % 8) == n
			&& i >= 0); /* Check the south-eastern ray */

	CB(attset, sqi + 1);
	return attset;
}

bitboard getrookattset(bitboard occ, int sqi) { /* Here check if the CB in the end is really necessary */
	int i = sqi;
	const int r = sqi / 8;
	bitboard attset = 0;

	do {
		attset |= UINT64_C(1) << i;
		i += 8;
	} while ((occ & (UINT64_C(1) << (i - 8))) == 0 && i < 64); /* Check the northern ray */

	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i -= 8;
	} while ((occ & (UINT64_C(1) << (i + 8))) == 0 && i >= 0); /* Check the southern ray */

	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i++;
	} while ((occ & (UINT64_C(1) << (i - 1))) == 0 && i / 8 == r); /* Check the eastern ray */

	i = sqi;
	do {
		attset |= UINT64_C(1) << i;
		i--;
	} while ((occ & (UINT64_C(1) << (i + 1))) == 0 && i / 8 == r && i >= 0); /* Check the western ray */

	CB(attset, sqi + 1);
	return attset;
}
