/*
 * movgen.c
 *
 *  Created on: 23.01.2017
 *      Author: sander
 *      Purpose: Generating all moves in a given chess position
 */

#include <stdio.h>
#include <stdlib.h>
#include "bitboards.h"
#include "movgen.h"
#include "debug.h"

typedef struct magic {
	uint64_t number;
	int shift;
} magic;
//TEST
magic bishopmagic[64] = { { 0x7552ab10290467ff, 59 },
		{ 0x5839e626215bff46, 60 }, { 0x299c771164c7a488, 59 }, {
				0x39e2aa096028b0cb, 59 }, { 0x3cc8f24761ab357f, 59 }, {
				0x1d60a25427a35d53, 59 }, { 0x2802c49164fff6d6, 60 }, {
				0x2cffd93a572d7f54, 59 }, { 0x3b75b5955b6e17ff, 60 }, {
				0x1a1ae9733250fbfb, 60 }, { 0x7f8a1c073cbe6d9b, 59 }, {
				0x38beb3c30bb1a2ef, 59 }, { 0x6f1be8f24769ca5d, 59 }, {
				0x7625c0a253a8369d, 59 }, { 0x3533943159b57f40, 60 }, {
				0x31d5005a6d8fff80, 60 }, { 0x48402c8b337aafeb, 60 }, {
				0x1ae009257aecefde, 60 }, { 0x12f00b6604a60023, 57 }, {
				0x691400f80123a004, 57 }, { 0x12c283ec03a023a9, 57 }, {
				0x657302c201011523, 57 }, { 0x75fc09935b34bfb0, 60 }, {
				0x76a1bc384852c4eb, 59 }, { 0x4cb9cbfc52f47970, 59 }, {
				0x34301a0e4d8a7583, 59 }, { 0x209e90080a062200, 57 }, {
				0x19c3835e554f9d56, 54 }, { 0x3b38df8e203940df, 54 }, {
				0x5f2d6a007b00db58, 57 }, { 0x7db781bd38716328, 59 }, {
				0x79ac92b0bac0526, 59 }, { 0x72c64b46433f79f9, 59 }, {
				0x2419f2f91b8bd47a, 59 }, { 0x169a018225500460, 57 }, {
				0x5eb4048c303b6cf4, 54 }, { 0x244ffcef7da10508, 54 }, {
				0x162104d00960091, 57 }, { 0x2bf6ebc518457283, 59 }, {
				0x67ac5372c4e01fc, 59 }, { 0x6632654504de3820, 59 }, {
				0x56affcfa2470a00c, 60 }, { 0x3ab0918401127003, 57 }, {
				0x21965d7144005806, 57 }, { 0x4ced731122037400, 57 }, {
				0x3f2c700a42003dc2, 57 }, { 0x423f99bd7cbde401, 60 }, {
				0x5b7fdd5e74537600, 60 }, { 0x6ee7f5ec6d1366a7, 60 }, {
				0xdcbff0d5965f994, 60 }, { 0x5005b0c1771c48dd, 59 }, {
				0x5129337f2088294d, 59 }, { 0x4057a0281362c021, 59 }, {
				0x43e7874466619f8f, 59 }, { 0x487ff55c2906602f, 60 }, {
				0x127fc9a544d4bf01, 60 }, { 0x28affe6510034bab, 59 }, {
				0x3915f3f931d4a549, 60 }, { 0x739b76a67d12f2e9, 59 }, {
				0x346e00f234a08826, 59 }, { 0x467a614d4fa8bcb5, 59 }, {
				0x164ac70704e651be, 59 }, { 0x785ff6209ecc4cf, 60 }, {
				0x5c7ff5eb25659902, 59 }, };

magic rookmagic[64] = { { 0x35b16bda5a2749bf, 51 }, { 0x28a8a76167875698, 52 },
		{ 0x71d88bb40229b3b9, 52 }, { 0xc0c0ee7c01870f, 52 }, {
				0xbd3a9205d140ba1, 51 }, { 0x42bfc597b3f2d, 51 }, {
				0x599a683c436600e2, 52 }, { 0x3b18df632d20bf7c, 51 }, {
				0x469c72b82358ac1c, 52 }, { 0x70eaad6a74c4d64e, 53 }, {
				0x3b40045600a79005, 53 }, { 0x2862000b24041200, 53 }, {
				0x718ab1e8010002b2, 53 }, { 0x5000138813da4b37, 53 }, {
				0x169c00100c672a08, 54 }, { 0x26bc329567a18d3c, 52 }, {
				0x5f09d57b1f34bd8d, 52 }, { 0x1bdc088010a8152, 53 }, {
				0x7a95c0046b8d209f, 53 }, { 0x15a39d4462bd6f1, 53 }, {
				0x33827001001bc726, 53 }, { 0x4c180a00031c9008, 53 }, {
				0x675e9c0057281e10, 54 }, { 0x188b56000c124d93, 53 }, {
				0x1dece88008c47d96, 52 }, { 0x90059c011ded420, 53 }, {
				0xaa12240041216a8, 53 }, { 0x9bd68301010601c, 53 }, {
				0x70ddb77001017b67, 53 }, { 0x6244ede8009dbd83, 53 }, {
				0x31864d4c00321068, 54 }, { 0x259353020000e784, 53 }, {
				0x4c1880c14010009a, 52 }, { 0x5d5ca47152500133, 53 }, {
				0x453bc9c84004cf75, 53 }, { 0x607dcce810048ff2, 53 }, {
				0x4ecc21d80028286c, 53 }, { 0x30ce940820200a2f, 53 }, {
				0x5a4f591044006608, 54 }, { 0x5a1d478d02001c44, 53 }, {
				0x3797508b0240047a, 52 }, { 0x1d10312c2ec008ce, 53 }, {
				0x3687ce271c400597, 53 }, { 0x79302be70060020a, 53 }, {
				0x4fc7723600b89899, 53 }, { 0x4c25941806280088, 53 }, {
				0x4981d066034c0018, 54 }, { 0x57e4bb22458a0004, 53 }, {
				0x5e090a8009c46300, 53 }, { 0x648d038208442e00, 54 }, {
				0x4afffea536097e00, 54 }, { 0xa81f00101a07900, 54 }, {
				0x7b3bd00861740200, 53 }, { 0xcf7605040041801, 54 }, {
				0x2b19a81007723400, 54 }, { 0x6f1aa96b1e640a00, 53 }, {
				0x5cfff9ef5dfcf02e, 52 }, { 0x4b8a83210510c001, 53 }, {
				0x4564f48062460032, 53 }, { 0x351900100603509, 53 }, {
				0x23e61810505c2a01, 52 }, { 0x18ba0e8400180601, 52 }, {
				0x6f1550017b482e0c, 53 }, { 0x5e658f0401ab4082, 52 }, };

char startfen[] = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "; /* Start position in FEN string */

int initmagicbishoptbls() { /* Returns 0 if malloc() was okay, 1 else */
	int sqi, max, i, j;

	initbishopocc();
	for (sqi = 0; sqi < 64; sqi++) {
		max = 0;
		for (i = 0; i < bishopocc[sqi].size; i++) { /* Get table size */
			j = bishopocc[sqi].occ[i] * bishopmagic[sqi].number
					>> bishopmagic[sqi].shift;
			max = MAX(max, j);
		}

		bishopattset[sqi] = malloc(sizeof(bitboard) * (max + 1));
		if (bishopattset[sqi] == NULL)
			return EXIT_FAILURE;

		for (i = 0; i < bishopocc[sqi].size; i++) { /* Fill table */
			j = (bishopocc[sqi].occ[i] * bishopmagic[sqi].number)
					>> bishopmagic[sqi].shift;
			bishopattset[sqi][j] = getbishopattset(bishopocc[sqi].occ[i], sqi);
		}
	}
	return EXIT_SUCCESS;
}

int initmagicrooktbls() { /* Returns 0 if malloc() was okay, 1 else */
	int sqi, max, i, j;

	initrookocc();
	for (sqi = 0; sqi < 64; sqi++) {
		max = 0;
		for (i = 0; i < rookocc[sqi].size; i++) { /* Get table size */
			j = rookocc[sqi].occ[i] * rookmagic[sqi].number
					>> rookmagic[sqi].shift;
			max = MAX(max, j);
		}

		rookattset[sqi] = malloc(sizeof(bitboard) * (max + 1));
		if (rookattset[sqi] == NULL)
			return EXIT_FAILURE;

		for (i = 0; i < rookocc[sqi].size; i++) { /* Fill table */
			j = (rookocc[sqi].occ[i] * rookmagic[sqi].number)
					>> rookmagic[sqi].shift;
			rookattset[sqi][j] = getrookattset(rookocc[sqi].occ[i], sqi);
		}
	}
	return EXIT_SUCCESS;
}

void destroymagictbls(void) {
	int sqi;
	for (sqi = 0; sqi < 64; sqi++)
		free(rookattset[sqi]);
	for (sqi = 0; sqi < 64; sqi++)
		free(bishopattset[sqi]);

}

int init(void) {
	initpawnmoves();
	initpawnjumps();
	initpawnattacks();
	initepstartsq();
	if (initmagicbishoptbls() == EXIT_FAILURE
			|| initmagicrooktbls() == EXIT_FAILURE)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}

bitboard getpieces(int player, chessposition *position) {
	return position->king[player] | position->queens[player]
			| position->rooks[player] | position->bishops[player]
			| position->knights[player] | position->pawns[player];
}
bitboard getallpieces(chessposition *position) {
	return getpieces(WHITE, position) | getpieces(BLACK, position);
}

void initattsets(chessposition *position) { /* Initialize attack sets */

	int i;
	bitboard tmp, occ;

	for (i = 0; i < 64; i++)
		position->attackset[i] = 0; /* Clear all attack sets */

	i = FSB(position->king[WHITE]) - 1;
	position->attackset[i] = kingmoves[i];

	i = FSB(position->king[BLACK]) - 1;
	position->attackset[i] = kingmoves[i];

	tmp = position->knights[WHITE];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		position->attackset[i] = knightmoves[i];
		CB(tmp, i + 1);
	}

	tmp = position->knights[BLACK];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		position->attackset[i] = knightmoves[i];
		CB(tmp, i + 1);
	}

	tmp = position->pawns[WHITE];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		position->attackset[i] = pawnattacks[WHITE][PAWNINDEX(i)];
		CB(tmp, i + 1);
	}

	tmp = position->pawns[BLACK];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		position->attackset[i] = pawnattacks[BLACK][PAWNINDEX(i)];
		CB(tmp, i + 1);
	}

	tmp = position->bishops[WHITE];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & bishopmask[i];
		position->attackset[i] = bishopattset[i][(occ * bishopmagic[i].number)
				>> bishopmagic[i].shift];
		CB(tmp, i + 1);
	}

	tmp = position->bishops[BLACK];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & bishopmask[i];
		position->attackset[i] = bishopattset[i][(occ * bishopmagic[i].number)
				>> bishopmagic[i].shift];
		CB(tmp, i + 1);
	}

	tmp = position->rooks[WHITE];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & rookmask[i];
		position->attackset[i] = rookattset[i][(occ * rookmagic[i].number)
				>> rookmagic[i].shift];
		CB(tmp, i + 1);
	}

	tmp = position->rooks[BLACK];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & rookmask[i];
		position->attackset[i] = rookattset[i][(occ * rookmagic[i].number)
				>> rookmagic[i].shift];
		CB(tmp, i + 1);
	}

	tmp = position->queens[WHITE];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & bishopmask[i];
		position->attackset[i] = bishopattset[i][(occ * bishopmagic[i].number)
				>> bishopmagic[i].shift];
		occ = getallpieces(position) & rookmask[i];
		position->attackset[i] |= rookattset[i][(occ * rookmagic[i].number)
				>> rookmagic[i].shift];
		CB(tmp, i + 1);
	}

	tmp = position->queens[BLACK];
	while (FSB(tmp) != 0) {
		i = FSB(tmp) - 1;
		occ = getallpieces(position) & bishopmask[i];
		position->attackset[i] = bishopattset[i][(occ * bishopmagic[i].number)
				>> bishopmagic[i].shift];
		occ = getallpieces(position) & rookmask[i];
		position->attackset[i] |= rookattset[i][(occ * rookmagic[i].number)
				>> rookmagic[i].shift];
		CB(tmp, i + 1);
	}
}

//void updateattsets(int player, chessposition *position) {		/* Updates the attack sets of player (0=white, 1=black) */
//
//	bitboard tmp;
//
//}



/* Updates the the attack sets of the sliding pieces assuming a piece moved from startsqi to endsqi and NO piece was captured */
/* It might also work with a capture but is not as efficient since no attack sets on the rays of the endsqi need to be updated */
void updslidpieces(chessposition *position, int startsqi, int endsqi,
		int player, int playeropp) {
	bitboard allpieces, rookocc, rookoccstart, rookoccend, bishopocc,
			bishopoccstart, bishopoccend, updpieces;
	int sqi;

	allpieces = getallpieces(position);
	rookoccstart = allpieces & rookmask[startsqi]; /* Get rook occupancies of startsqi */
	rookoccend = allpieces & rookmask[endsqi]; /* Get rook occupancies of endsqi */
	bishopoccstart = allpieces & bishopmask[startsqi]; /* Get bishop occupancies of startsqi */
	bishopoccend = allpieces & bishopmask[endsqi]; /* Get bishop occupancies of endsqi */

	/* Find rooks on rook rays of startsqi */
	updpieces = (position->rooks[player] | position->rooks[playeropp])
			& rookattset[startsqi][(rookoccstart * rookmagic[startsqi].number)
					>> rookmagic[startsqi].shift];
	/* Add rooks on rook rays of endsqi */
	updpieces |= (position->rooks[player] | position->rooks[playeropp])
			& rookattset[endsqi][(rookoccend * rookmagic[endsqi].number)
					>> rookmagic[endsqi].shift];
	/* Update attack sets of rooks on rook rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find queens on rook rays of startsqi */
	updpieces = (position->queens[player] | position->queens[playeropp])
			& rookattset[startsqi][(rookoccstart * rookmagic[startsqi].number)
					>> rookmagic[startsqi].shift];
	/* Add queens on rook rays of endsqi */
	updpieces |= (position->queens[player] | position->queens[playeropp])
			& rookattset[endsqi][(rookoccend * rookmagic[endsqi].number)
					>> rookmagic[endsqi].shift];
	/* Update attack sets of queens on rook rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		position->attackset[sqi] |= bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find bishops on bishop rays of startsqi */
	updpieces = (position->bishops[player] | position->bishops[playeropp])
			& bishopattset[startsqi][(bishopoccstart
					* bishopmagic[startsqi].number)
					>> bishopmagic[startsqi].shift];
	/* Add bishops on bishop rays of endsqi */
	updpieces |= (position->bishops[player] | position->bishops[playeropp])
			& bishopattset[endsqi][(bishopoccend * bishopmagic[endsqi].number)
					>> bishopmagic[endsqi].shift];
	/* Update attack sets of bishops on bishop rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find queens on bishop rays of startsqi */
	updpieces = (position->queens[player] | position->queens[playeropp])
			& bishopattset[startsqi][(bishopoccstart
					* bishopmagic[startsqi].number)
					>> bishopmagic[startsqi].shift];
	/* Add queens on bishop rays of endsqi */
	updpieces |= (position->queens[player] | position->queens[playeropp])
			& bishopattset[endsqi][(bishopoccend * bishopmagic[endsqi].number)
					>> bishopmagic[endsqi].shift];
	/* Update attack sets of queens on bishop rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		position->attackset[sqi] |= bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}
}

/* Updates the the attack sets of the sliding pieces assuming a piece moved from startsqi to endsqi AND captured a piece there */
void updslidpieces_capt(chessposition *position, int startsqi, int player,
		int playeropp) {
	bitboard allpieces, rookocc, rookoccstart, bishopocc, bishopoccstart,
			updpieces;
	int sqi;

	allpieces = getallpieces(position);
	rookoccstart = allpieces & rookmask[startsqi]; /* Get rook occupancies of startsqi */
	bishopoccstart = allpieces & bishopmask[startsqi]; /* Get bishop occupancies of startsqi */

	/* Find rooks on rook rays of startsqi */
	updpieces = (position->rooks[player] | position->rooks[playeropp])
			& rookattset[startsqi][(rookoccstart * rookmagic[startsqi].number)
					>> rookmagic[startsqi].shift];
	/* Update attack sets of rooks on rook rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find queens on rook rays of startsqi */
	updpieces = (position->queens[player] | position->queens[playeropp])
			& rookattset[startsqi][(rookoccstart * rookmagic[startsqi].number)
					>> rookmagic[startsqi].shift];
	/* Update attack sets of queens on rook rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		position->attackset[sqi] |= bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find bishops on bishop rays of startsqi */
	updpieces = (position->bishops[player] | position->bishops[playeropp])
			& bishopattset[startsqi][(bishopoccstart
					* bishopmagic[startsqi].number)
					>> bishopmagic[startsqi].shift];
	/* Update attack sets of bishops on bishop rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}

	/* Find queens on bishop rays of startsqi */
	updpieces = (position->queens[player] | position->queens[playeropp])
			& bishopattset[startsqi][(bishopoccstart
					* bishopmagic[startsqi].number)
					>> bishopmagic[startsqi].shift];
	/* Update attack sets of queens on bishop rays of startsqi and endsqi */
	while (updpieces != 0) {
		sqi = FSB(updpieces) - 1;
		rookocc = allpieces & rookmask[sqi];
		bishopocc = allpieces & bishopmask[sqi];
		position->attackset[sqi] = rookattset[sqi][(rookocc
				* rookmagic[sqi].number) >> rookmagic[sqi].shift];
		position->attackset[sqi] |= bishopattset[sqi][(bishopocc
				* bishopmagic[sqi].number) >> bishopmagic[sqi].shift];
		CB(updpieces, sqi + 1);
	}
}

bitboard getattsquares(int player, chessposition position) { /* Get attacked squares by player = WHITE or player = BLACK */
	/* Simply reads them of the position.attackset array */

	bitboard team; /* Encodes where all pieces are */
	bitboard attsquares = 0; /* Clear attacked squares */

	team = position.king[player] | position.queens[player]
			| position.rooks[player] | position.bishops[player]
			| position.knights[player] | position.pawns[player];

	while (team != 0) { /* As long as there are pieces to consider */
		attsquares |= position.attackset[FSB(team) - 1]; /* Add attacked squares by that piece */
		CB(team, FSB(team)); /* Clear that piece */
	}

	return attsquares;
}

void clearpos(chessposition *position) { /* Clear the board */

	int i;
	position->king[WHITE] = 0;
	position->king[BLACK] = 0;
	position->queens[WHITE] = 0;
	position->queens[BLACK] = 0;
	position->rooks[WHITE] = 0;
	position->rooks[BLACK] = 0;
	position->bishops[WHITE] = 0;
	position->bishops[BLACK] = 0;
	position->knights[WHITE] = 0;
	position->knights[BLACK] = 0;
	position->pawns[WHITE] = 0;
	position->pawns[BLACK] = 0;
	position->states.kingcastle[WHITE] = 0;
	position->states.kingcastle[BLACK] = 0;
	position->states.queencastle[WHITE] = 0;
	position->states.queencastle[BLACK] = 0;
	position->states.enpassant = -1;
	position->states.plies = 0;
	for (i = 0; i < 64; i++)
		position->attackset[i] = 0;
}

int importpos(char *fen, chessposition *position) { /* Set up internal position from FEN string */

	char *board, *tomove, *castling, *enpassant, *halfmoves, *moves;	// gcc throws warning that *moves is set but not used
																		// this is okay since there is a field for the move number
																		// in FEN which might be used later
	char *tmp, *ptr;
	int i = 7, j = 0, n = 0;

	tmp = (char*) malloc(strlen(fen) * sizeof(char) + 1);
	ptr = tmp;
	strncpy(tmp, fen, strlen(fen));

	board = strsep(&tmp, " ");
	tomove = strsep(&tmp, " ");
	castling = strsep(&tmp, " ");
	enpassant = strsep(&tmp, " ");
	halfmoves = strsep(&tmp, " ");
	moves = strsep(&tmp, " ");

	clearpos(position); /* Clear the board */

	while (board[n] != '\0') { /* Set up the board */
		switch (board[n]) {
		case 'B':
			position->bishops[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'b':
			position->bishops[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'K':
			position->king[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'k':
			position->king[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'N':
			position->knights[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'n':
			position->knights[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'P':
			position->pawns[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'p':
			position->pawns[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'Q':
			position->queens[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'q':
			position->queens[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'R':
			position->rooks[WHITE] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case 'r':
			position->rooks[BLACK] |= UINT64_C(1) << (i * 8 + j);
			j++;
			break;
		case '1':
			j += 1;
			break;
		case '2':
			j += 2;
			break;
		case '3':
			j += 3;
			break;
		case '4':
			j += 4;
			break;
		case '5':
			j += 5;
			break;
		case '6':
			j += 6;
			break;
		case '7':
			j += 7;
			break;
		case '8':
			j += 8;
			break;
		case '/':
			if (j == 8) { /* Have 8 squares been assigned? */
				i--; /* Rank finished, go on to next rank */
				j = 0; /* Set j to A file */
			} else {
				clearpos(position);
				return -2; /* Not enough squares in current rank assigned */
			}
			break;
		default:
			clearpos(position); /* Erroneous FEN string, unknown character */
			return -1;
		}
		n++;
	}

	switch (*tomove) {
	case 'b':
		position->states.tomove = BLACK;
		break;
	case 'w':
		position->states.tomove = WHITE;
		break;
	default:
		clearpos(position); /* Erroneous FEN string, unknown character */
		return -1;
	}

	n = 0;
	while (castling[n] != '\0') {
		switch (castling[n]) {
		case 'K':
			position->states.kingcastle[WHITE] = 1;
			break;
		case 'k':
			position->states.kingcastle[BLACK] = 1;
			break;
		case 'Q':
			position->states.queencastle[WHITE] = 1;
			break;
		case 'q':
			position->states.queencastle[BLACK] = 1;
			break;
		case '-':
			break;
		default:
			clearpos(position); /* Erroneous FEN string, unknown character */
			return -1;
		}
		n++;
	}

	switch (*enpassant) {
	case 'a':
		position->states.enpassant = 0;
		break;
	case 'b':
		position->states.enpassant = 1;
		break;
	case 'c':
		position->states.enpassant = 2;
		break;
	case 'd':
		position->states.enpassant = 3;
		break;
	case 'e':
		position->states.enpassant = 4;
		break;
	case 'f':
		position->states.enpassant = 5;
		break;
	case 'g':
		position->states.enpassant = 6;
		break;
	case 'h':
		position->states.enpassant = 7;
		break;
	case '-':
		position->states.enpassant = -1;
		break;
	default:
		clearpos(position); /* Erroneous FEN string, unknown character */
		return -1;
	}

	position->states.plies = (int) strtol(halfmoves, &tmp, 10); /* Convert string to integer */
	if (*tmp != '\0')
		clearpos(position); /* Was there a character that was not a digit? */

	initattsets(position);

	/* Catch erroneous positions */
	i = position->states.tomove;
	if ((position->king[(i + 1) % 2] & getattsquares(i, *position)) != 0) {
		clearpos(position);
		return (-10) * (i + 1); /* Player who is not to move is in check */
	}
	for (i = WHITE; i <= BLACK; i++) {
		if (POPCNT(position->king[i]) != 1) {
			clearpos(position);
			return (-10) * (i + 1) - 1; /* Wrong amount of kings */
		}
		if ((position->pawns[i] & (CHESSRANK(i, 0) | CHESSRANK(i, 7))) != 0) {
			clearpos(position);
			return (-10) * (i + 1) - 2; /* Pawns on 1st or 8th rank */
		}
		/* Fehlerhaften en passant abfangen: kein Bauer zum Schlagen, andere Figur hat eigentlich den en passant Bauern geblockt */
		/* Rochadefehler */
	}
	free(ptr);
	return EXIT_SUCCESS;
}

int generatemoves(chessposition position, chessposition *moves) {
	/* Generates all legal moves in given position, stores them in "moves" and returns their number */
	/* When moving it does NOT CAPTURE A KING, opponent's king cannot be in check */

	/* PERFORMANCE ISSUE: When king moves (castles), only attack sets of same color sliding pieces need to be updated? */
	/*						Give only pointer to position? */

	bitboard possible, forbidden, pieces, rookocc, bishopocc, allpieces;
	chessposition move = position;
	int movecount = 0; /* Number of legal moves in given position */
	int player, playeropp, sqi, startsqi, endsqi;

	player = position.states.tomove;
	playeropp = (player + 1) % 2; /* The one not to move */

	/* First generate moves for the king */
	/* Calculate possible squares for the king */
	forbidden = position.queens[player] | position.rooks[player]
			| position.bishops[player] | position.knights[player]
			| position.pawns[player] | getattsquares(playeropp, position);

	startsqi = FSB(position.king[player]) - 1;
	possible = kingmoves[startsqi] & ~forbidden
			& getpieces(playeropp, &position); /* First consider the captures */

	while (possible != 0) { /* While there are pieces for the king to capture */
		move = position;

		/* +++ */
		move.attackset[startsqi] = 0; /* Clear attack set of start square */

		move.king[player] = 0; /* Remove king from the board */

		/* +++ */
		endsqi = FSB(possible) - 1;

		SB(move.king[player], endsqi + 1); /* Set king's position to first possible */

		/* +++ */
		move.attackset[endsqi] = kingmoves[endsqi]; /* Update attack set of end square */

		CB(possible, endsqi + 1); /* Clear that square from the possible ones */
		move.queens[playeropp] &= ~move.king[player]; /* Capture the black piece (NO KING CAPTURE) */
		move.rooks[playeropp] &= ~move.king[player];
		move.bishops[playeropp] &= ~move.king[player];
		move.knights[playeropp] &= ~move.king[player];
		move.pawns[playeropp] &= ~move.king[player];

		/* +++ */
		updslidpieces_capt(&move, startsqi, player, playeropp);

		/* PERFORMANCE ISSUE: Not necessary to update ALL attacksets */
		//initattsets(&move); /* Update the attack sets of each square */
		/* MAYBE PERFORMANCE: Check if king is in check beforehand. Then the next check for check might be avoidable. */
		/* Is the king now in check? */
		if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
			move.states.kingcastle[player] = 0; /* Castling is no longer possible */
			move.states.queencastle[player] = 0;
			if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
				move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
			if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
				move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
			move.states.enpassant = -1; /* En passant is not possible */
			move.states.plies = 0; /* King just captured a piece */
			move.states.tomove = playeropp; /* Now the opponent is to move */
			moves[movecount] = move;
			movecount++;
		}
	}

	possible = kingmoves[FSB(position.king[player]) - 1] & ~forbidden
			& ~getpieces(playeropp, &position); /* Consider now the non-captures */

	while (possible != 0) { /* While there are squares for the king to move to */
		move = position;

		/* +++ */
		move.attackset[startsqi] = 0; /* Clear attack set of start square */

		move.king[player] = 0; /* Remove king from the board */

		/* +++ */
		endsqi = FSB(possible) - 1;

		SB(move.king[player], endsqi + 1); /* Set king's position to first possible */

		/* +++ */
		move.attackset[endsqi] = kingmoves[endsqi]; /* Update attack set of end square */

		CB(possible, endsqi + 1); /* Clear that square from the possible ones */

		/* +++ */
		updslidpieces(&move, startsqi, endsqi, player, playeropp);

		/* PERFORMANCE ISSUE: Not necessary to update ALL attacksets */
		//initattsets(&move); /* Update the attack sets of each square */
		/* Is the king now in check? */
		if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
			move.states.kingcastle[player] = 0; /* Castling is no longer possible */
			move.states.queencastle[player] = 0;
			move.states.enpassant = -1; /* En passant is not possible */
			move.states.plies++; /* King did not capture a piece */
			move.states.tomove = playeropp; /* Now the opponent is to move */
			moves[movecount] = move;
			movecount++;
		}
	}

	/* Now the queens */
	pieces = position.queens[player];
	while (FSB(pieces) != 0) { /* While there are queens to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the queen is on */

		/* Calculate possible squares for the queen on sqi */
		forbidden = position.king[player] | position.queens[player]
				| position.rooks[player] | position.bishops[player]
				| position.knights[player] | position.pawns[player];

		possible = position.attackset[startsqi] & ~forbidden
				& getpieces(playeropp, &position); /* First consider the captures */

		while (possible != 0) { /* While there are pieces for the queen to capture */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.queens[player], startsqi + 1); /* Remove that queen from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.queens[player], endsqi + 1); /* Set queen's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */
			move.queens[playeropp] &= ~move.queens[player]; /* Capture the black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.queens[player];
			move.bishops[playeropp] &= ~move.queens[player];
			move.knights[playeropp] &= ~move.queens[player];
			move.pawns[playeropp] &= ~move.queens[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Queen just captured a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}

		possible = position.attackset[startsqi] & ~forbidden
				& ~getpieces(playeropp, &position); /* Now consider the non-captures */

		while (possible != 0) { /* While there are squares for the queen to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.queens[player], startsqi + 1); /* Remove that queen from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.queens[player], endsqi + 1); /* Set queen's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* Queen did not capture a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now the rooks */
	pieces = position.rooks[player];
	while (FSB(pieces) != 0) { /* While there are rooks to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the rook is on */

		/* Calculate possible squares for the rook on startsqi */
		forbidden = position.king[player] | position.queens[player]
				| position.rooks[player] | position.bishops[player]
				| position.knights[player] | position.pawns[player];

		possible = position.attackset[startsqi] & ~forbidden
				& getpieces(playeropp, &position); /* First consider the captures */

		while (possible != 0) { /* While there are pieces for the rook to capture */
			move = position; /* Set move to current position */
			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.rooks[player], startsqi + 1); /* Remove that rook from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.rooks[player], endsqi + 1); /* Set rook's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */
			move.queens[playeropp] &= ~move.rooks[player]; /* Capture the black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.rooks[player];
			move.bishops[playeropp] &= ~move.rooks[player];
			move.knights[playeropp] &= ~move.rooks[player];
			move.pawns[playeropp] &= ~move.rooks[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Performance Issue: Is there a need for checking check when the rook in the corner moved? */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[player] & ACORNER(player)) == 0)
					move.states.queencastle[player] = 0; /* Adjust castling rights if a rook moved */
				if ((move.rooks[player] & HCORNER(player)) == 0)
					move.states.kingcastle[player] = 0;
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Rook just captured a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}

		possible = position.attackset[startsqi] & ~forbidden
				& ~getpieces(playeropp, &position); /* Now consider the non-captures */

		while (possible != 0) { /* While there are squares for the rook to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.rooks[player], startsqi + 1); /* Remove that rook from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.rooks[player], endsqi + 1); /* Set rook's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Performance Issue: Is there a need for checking check when the rook in the corner moved? */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[player] & ACORNER(player)) == 0)
					move.states.queencastle[player] = 0; /* Adjust castling rights if a rook moved */
				if ((move.rooks[player] & HCORNER(player)) == 0)
					move.states.kingcastle[player] = 0;
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* Rook did not capture a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now the bishops */
	pieces = position.bishops[player];
	while (FSB(pieces) != 0) { /* While there are bishops to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the bishop is on */

		/* Calculate possible squares for the bishop on startsqi */
		forbidden = position.king[player] | position.queens[player]
				| position.rooks[player] | position.bishops[player]
				| position.knights[player] | position.pawns[player];

		possible = position.attackset[startsqi] & ~forbidden
				& getpieces(playeropp, &position); /* First consider the captures */

		while (possible != 0) { /* While there are pieces for the bishop to capture */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.bishops[player], startsqi + 1); /* Remove that bishop from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.bishops[player], endsqi + 1); /* Set bishop's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */
			move.queens[playeropp] &= ~move.bishops[player]; /* Capture the black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.bishops[player];
			move.bishops[playeropp] &= ~move.bishops[player];
			move.knights[playeropp] &= ~move.bishops[player];
			move.pawns[playeropp] &= ~move.bishops[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Bishop just captured a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}

		possible = position.attackset[startsqi] & ~forbidden
				& ~getpieces(playeropp, &position); /* Now consider the non-captures */

		while (possible != 0) { /* While there are squares for the bishop to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.bishops[player], startsqi + 1); /* Remove that bishop from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.bishops[player], endsqi + 1); /* Set bishop's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* Bishop did not capture a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now the knights */
	pieces = position.knights[player];
	while (FSB(pieces) != 0) { /* While there are knights to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the knight is on */

		/* Calculate possible squares for the knight on startsqi */
		forbidden = position.king[player] | position.queens[player]
				| position.rooks[player] | position.bishops[player]
				| position.knights[player] | position.pawns[player];

		possible = position.attackset[startsqi] & ~forbidden
				& getpieces(playeropp, &position); /* First consider the captures */

		while (possible != 0) { /* While there are pieces for the knight to capture */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.knights[player], startsqi + 1); /* Remove that knight from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.knights[player], endsqi + 1); /* Set knight's position to first possible */

			/* +++ */
			move.attackset[endsqi] = knightmoves[endsqi]; /* Update attack set of end square */

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */

			move.queens[playeropp] &= ~move.knights[player]; /* Capture the black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.knights[player];
			move.bishops[playeropp] &= ~move.knights[player];
			move.knights[playeropp] &= ~move.knights[player];
			move.pawns[playeropp] &= ~move.knights[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);
			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Knight just captured a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}

		possible = position.attackset[startsqi] & ~forbidden
				& ~getpieces(playeropp, &position); /* Now consider the non-captures */

		while (possible != 0) { /* While there are squares for the knight to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.knights[player], startsqi + 1); /* Remove that knight from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.knights[player], endsqi + 1); /* Set knight's position to first possible */

			/* +++ */
			move.attackset[endsqi] = knightmoves[endsqi]; /* Update attack set of end square */

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* Knight did not capture a piece */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now the pawns */
	forbidden = (getallpieces(&position)); /* Pawns cannot move (not capture) to these squares */

	pieces = position.pawns[player] & CHESSRANK(player, 6); /* First consider pawn promotions */
	while (FSB(pieces) != 0) { /* While there are pawns to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the pawn is on */

		/* First consider possible captures */
		/* No capture of a pawn possible on last rank */
		possible = position.attackset[startsqi]
				& (position.queens[playeropp] | position.rooks[playeropp]
						| position.bishops[playeropp]
						| position.knights[playeropp]);

		while (possible != 0) { /* While there are squares for the pawn to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.queens[player], endsqi + 1); /* Promote pawn to queen */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			//CB(possible, endsqi + 1);				/* Clear that move from the possible ones */
			move.queens[playeropp] &= ~move.queens[player]; /* Capture a black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.queens[player];
			move.bishops[playeropp] &= ~move.queens[player];
			move.knights[playeropp] &= ~move.queens[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
				/* Since the promotion to a queen was a legal move, all other promotions are as well */
				/* We just have to change the queen to the other pieces and add the moves */

				CB(move.queens[player], endsqi + 1); /* Clear promoted queen */
				SB(move.rooks[player], endsqi + 1); /* Promote to rook */

				/* +++ */
				/* Update attack set of end square */
				rookocc = allpieces & rookmask[endsqi];
				move.attackset[endsqi] = rookattset[endsqi][(rookocc
						* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];
				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;

				CB(move.rooks[player], endsqi + 1); /* Clear promoted rook */
				SB(move.bishops[player], endsqi + 1); /* Promote to bishop */

				/* +++ */
				/* Update attack set of end square */
				bishopocc = allpieces & bishopmask[endsqi];
				move.attackset[endsqi] = bishopattset[endsqi][(bishopocc
						* bishopmagic[endsqi].number)
						>> bishopmagic[endsqi].shift];
				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;

				CB(move.bishops[player], endsqi + 1); /* Clear promoted bishop */
				SB(move.knights[player], endsqi + 1); /* Promote to knight */

				/* +++ */
				/* Update attack set of end square */
				move.attackset[endsqi] = knightmoves[endsqi];

				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;
			}
			CB(possible, FSB(possible));
		}

		/* Now consider promotions with moves forward */
		possible = pawnmoves[player][PAWNINDEX(startsqi)] & ~forbidden;

		if (possible != 0) { /* if instead of while since pawn can only move to one square */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.queens[player], endsqi + 1); /* Promote pawn to queen */

			/* +++ */
			/* Update attack set of end square */
			allpieces = getallpieces(&move);
			rookocc = allpieces & rookmask[endsqi];
			bishopocc = allpieces & bishopmask[endsqi];
			move.attackset[endsqi] = rookattset[endsqi][(rookocc
					* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];
			move.attackset[endsqi] |= bishopattset[endsqi][(bishopocc
					* bishopmagic[endsqi].number) >> bishopmagic[endsqi].shift];

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* Pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
				/* Since the promotion to a queen was a legal move, all other promotions are as well */
				/* We just have to change the queen to the other pieces and add the moves */

				CB(move.queens[player], endsqi + 1); /* Clear promoted queen */
				SB(move.rooks[player], endsqi + 1); /* Promote to rook */

				/* +++ */
				/* Update attack set of end square */
				rookocc = allpieces & rookmask[endsqi];
				move.attackset[endsqi] = rookattset[endsqi][(rookocc
						* rookmagic[endsqi].number) >> rookmagic[endsqi].shift];

				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;

				CB(move.rooks[player], endsqi + 1); /* Clear promoted rook */
				SB(move.bishops[player], endsqi + 1); /* Promote to bishop */

				/* +++ */
				/* Update attack set of end square */
				bishopocc = allpieces & bishopmask[endsqi];
				move.attackset[endsqi] = bishopattset[endsqi][(bishopocc
						* bishopmagic[endsqi].number)
						>> bishopmagic[endsqi].shift];
				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;

				CB(move.bishops[player], endsqi + 1); /* Clear promoted bishop */
				SB(move.knights[player], endsqi + 1); /* Promote to knight */

				/* +++ */
				/* Update attack set of end square */
				move.attackset[endsqi] = knightmoves[endsqi];

				/* PERFORMANCE ISSUE: Only new attack set for the promoted piece necessary */
				//initattsets(&move);
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	pieces = position.pawns[player] & ~(CHESSRANK(player, 6)); /* Now consider non special pawn moves */
	while (FSB(pieces) != 0) { /* While there are pawns to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the pawn is on */

		/* First consider possible captures */
		possible = position.attackset[startsqi]
				& (position.queens[playeropp] | position.rooks[playeropp]
						| position.bishops[playeropp]
						| position.knights[playeropp]
						| position.pawns[playeropp]);

		while (possible != 0) { /* While there are squares for the pawn to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.pawns[player], endsqi + 1); /* Set pawn's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			move.attackset[endsqi] = pawnattacks[player][PAWNINDEX(endsqi)];

			CB(possible, endsqi + 1); /* Clear that move from the possible ones */
			move.queens[playeropp] &= ~move.pawns[player]; /* Capture a black piece (NO KING CAPTURE) */
			move.rooks[playeropp] &= ~move.pawns[player];
			move.bishops[playeropp] &= ~move.pawns[player];
			move.knights[playeropp] &= ~move.pawns[player];
			move.pawns[playeropp] &= ~move.pawns[player];

			/* +++ */
			updslidpieces_capt(&move, startsqi, player, playeropp);

			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				if ((move.rooks[playeropp] & ACORNER(playeropp)) == 0)
					move.states.queencastle[playeropp] = 0; /* Adjust castling rights of opponent */
				if ((move.rooks[playeropp] & HCORNER(playeropp)) == 0)
					move.states.kingcastle[playeropp] = 0; /* if a rook was captured */
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* A pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}

		/* Now consider possible moves forward */
		possible = pawnmoves[player][PAWNINDEX(startsqi)] & ~forbidden;

		if (possible != 0) { /* If there is a square for the pawn to move to */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.pawns[player], FSB(possible)); /* Set pawn's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			move.attackset[endsqi] = pawnattacks[player][PAWNINDEX(endsqi)];

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);
			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* A pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now consider pawn jumps in the beginning */
	pieces = position.pawns[player] & CHESSRANK(player, 1);
	while (FSB(pieces) != 0) { /* While there are pawns to consider */
		startsqi = FSB(pieces) - 1; /* Square index where the pawn is on */

		/* Possible pawn jumps */
		/* Code behind '|' prevents the square on fourth(white)/fifth(black) to be possible if the pawn is blocked by a piece on third/sixth rank */
		if (player == WHITE)
			possible = pawnjumps[player][GETFILE(startsqi)]
					& ~(forbidden | (forbidden << 8));
		else
			possible = pawnjumps[player][GETFILE(startsqi)]
					& ~(forbidden | (forbidden >> 8));

		if (possible != 0) { /* If pawn jump is possible */
			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.pawns[player], FSB(possible)); /* Set pawn's position to first possible */

			/* +++ */
			/* Update attack set of end square */
			move.attackset[endsqi] = pawnattacks[player][PAWNINDEX(endsqi)];

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp);
			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = GETFILE(startsqi); /* En passant possible on that file */
				move.states.plies = 0; /* A pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
		CB(pieces, FSB(pieces)); /* Clear that piece from the ones to consider */
	}

	/* Now consider en passant captures */
	/* Is en passant possible? */
	if (position.states.enpassant > -1) {
		pieces = position.pawns[player]
				& epstartsq[player][position.states.enpassant];
		while (FSB(pieces) != 0) {
			startsqi = FSB(pieces) - 1;
			possible = pawnattacks[player][PAWNINDEX(startsqi)]
					& ((A3 | A6) << position.states.enpassant); /* Captures possible on 3-rd/6-th rank of given file */

			move = position; /* Set move to current position */

			/* +++ */
			move.attackset[startsqi] = 0; /* Clear attack set of start square */

			CB(move.pawns[player], startsqi + 1); /* Remove that pawn from the board */

			/* +++ */
			endsqi = FSB(possible) - 1;

			SB(move.pawns[player], endsqi + 1); /* Set pawn to new position */

			/* +++ */
			/* Update attack set of end square */
			move.attackset[endsqi] = pawnattacks[player][PAWNINDEX(endsqi)];
			/* +++ */
			sqi = FSB(CHESSRANK(playeropp, 3)
					& CHESSFILE(position.states.enpassant)) - 1;

			move.pawns[playeropp] &= ~(CHESSRANK(playeropp, 3)
					& CHESSFILE(position.states.enpassant)); /* Remove pawn from fourth rank (seen from playeropp) */

			/* +++ */
			updslidpieces(&move, startsqi, endsqi, player, playeropp); /* For the pawn that captured (but did not get the pawn on the square it moved to) */
			updslidpieces_capt(&move, sqi, player, playeropp); /* For the pawn that was captured and "moved away" from the square */
			/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
			//initattsets(&move); /* Update the attack sets of each square */
			/* Is the king now in check? */
			if ((move.king[player] & getattsquares(playeropp, move)) == 0) {
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies = 0; /* A pawn just moved */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
			CB(pieces, FSB(pieces));
		}
	}

	/* Now consider castlings */
	/* Is castling king side possible? */
	if (position.states.kingcastle[player] == 1) {
		/* Is there a piece between king and rook? */
		if ((getallpieces(&position) & ((F1 | G1) << (player * 56))) == 0) {
			/* Is one of the squares that the king touches while castling king side attacked? */
			if ((getattsquares(playeropp, position)
					& ((E1 | F1 | G1) << (player * 56))) == 0) {
				move = position;
				move.king[player] <<= 2; //G1 << (player * 56); /* Put two squares to the right */
				move.rooks[player] &= ~(H1 << (player * 56)); /* Remove rook from H1/H8 */
				move.rooks[player] |= F1 << (player * 56); /* Put rook on F1/F8 */

				/* +++ */
				move.attackset[7 + player * 56] = 0;	/* Update attack set of corner where rook was in */
				startsqi = 4 + player * 56; /* E1 or E8 */
				endsqi = 5 + player * 56; /* F1 or F8 */
				updslidpieces(&move, startsqi, endsqi, player, playeropp); /* Update attsets as if a piece moved from E1/8 to F1/8 */
				/* The following does not seem to be necessary */
				//startsqi = 7 + player * 56;	/* H1 or H8 */
				//endsqi = 6 + player * 56;	/* G1 or G8 */
				//updslidpieces(&move, startsqi, endsqi, player, playeropp);	/* Update attsets as if a piece moved from H1/8 to G1/8 */
				/* Also the rook that moved seems to get the correct attack sets */

				/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
				//initattsets(&move);
				/* Check if king is in check not necessary? */
				move.states.kingcastle[player] = 0; /* Castling is no longer possible */
				move.states.queencastle[player] = 0;
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* No capture and no pawn move */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
	}

	/* Is castling queen side possible? */
	if (position.states.queencastle[player] == 1) {
		/* Is there a piece between king and rook? */
		if ((getallpieces(&position) & ((B1 | C1 | D1) << (player * 56)))
				== 0) {
			/* Is one of the squares that the king touches while castling queen side attacked? */
			if ((getattsquares(playeropp, position)
					& ((C1 | D1 | E1) << (player * 56))) == 0) {
				move = position;
				move.king[player] >>= 2; //C1 << (player * 56); /* Put king on C1/C8 */
				move.rooks[player] &= ~(A1 << (player * 56)); /* Remove rook from A1/A8 */
				move.rooks[player] |= D1 << (player * 56); /* Put rook on D1/D8 */

				/* +++ */
				move.attackset[0 + player * 56] = 0;	/* Update attack set of corner where rook was in */
				startsqi = 4 + player * 56; /* E1 or E8 */
				endsqi = 3 + player * 56; /* D1 or D8 */
				updslidpieces(&move, startsqi, endsqi, player, playeropp); /* Update attsets as if a piece moved from E1/8 to C1/8 */
				/* The following does not seem to be necessary */
								//startsqi = 7 + player * 56;	/* H1 or H8 */
								//endsqi = 6 + player * 56;	/* G1 or G8 */
								//updslidpieces(&move, startsqi, endsqi, player, playeropp);	/* Update attsets as if a piece moved from H1/8 to G1/8 */
								/* Also the rook that moved seems to get the correct attack sets */
				/* PERFORMANCE ISSUE: Not necessary to update ALL attack sets */
				//initattsets(&move);

				/* Check if king is in check not necessary? */
				move.states.kingcastle[player] = 0; /* Castling is no longer possible */
				move.states.queencastle[player] = 0;
				move.states.enpassant = -1; /* En passant is not possible */
				move.states.plies++; /* No capture and no pawn move */
				move.states.tomove = playeropp; /* Now the opponent is to move */
				moves[movecount] = move;
				movecount++;
			}
		}
	}
	return movecount;
}

char getfile(int sqi) {
	switch (sqi % 8) {
	case 0:
		return 'a';
		break;
	case 1:
		return 'b';
		break;
	case 2:
		return 'c';
		break;
	case 3:
		return 'd';
		break;
	case 4:
		return 'e';
		break;
	case 5:
		return 'f';
		break;
	case 6:
		return 'g';
		break;
	case 7:
		return 'h';
		break;
	}
	return -1;
}

char getrank(int sqi) {
	switch (sqi / 8) {
	case 0:
		return '1';
		break;
	case 1:
		return '2';
		break;
	case 2:
		return '3';
		break;
	case 3:
		return '4';
		break;
	case 4:
		return '5';
		break;
	case 5:
		return '6';
		break;
	case 6:
		return '7';
		break;
	case 7:
		return '8';
		break;
	}
	return -1;
}

int getmovenotation(chessposition *before, chessposition *now, char *notation) {
	// Returns the notation of the move starting in position *before and resulting in position *now
	// in long algebraic notation at notation
	int player;
	int startsqi, endsqi;

	player = before->states.tomove;
	notation[4] = '\0';
	notation[5] = '\0';

	if (before->king[player] != now->king[player]) { /* The king moved */
		startsqi = FSB(before->king[player]) - 1;
		endsqi = FSB(now->king[player]) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		notation[2] = getfile(endsqi);
		notation[3] = getrank(endsqi);
	} else if (before->pawns[player] != now->pawns[player]) { /* A pawn moved */
		startsqi = FSB(before->pawns[player] & ~(now->pawns[player])) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		if (POPCNT(before->pawns[player]) == POPCNT(now->pawns[player])) { /* Amount of players pawns has not changed */
			endsqi = FSB(now->pawns[player] & ~(before->pawns[player])) - 1;
			notation[2] = getfile(endsqi);
			notation[3] = getrank(endsqi);
		} else {
			if (POPCNT(before->queens[player]) != POPCNT(now->queens[player])) { /* Pawn promoted to queen */
				endsqi = FSB(now->queens[player] & ~(before->queens[player]))
						- 1;
				notation[2] = getfile(endsqi);
				notation[3] = getrank(endsqi);
				notation[4] = 'q';
			} else if (POPCNT(
					before->rooks[player]) != POPCNT(now->rooks[player])) { /* Pawn promoted to rook */
				endsqi = FSB(now->rooks[player] & ~(before->rooks[player])) - 1;
				notation[2] = getfile(endsqi);
				notation[3] = getrank(endsqi);
				notation[4] = 'r';
			} else if (POPCNT(
					before->bishops[player]) != POPCNT(now->bishops[player])) { /* Pawn promoted to bishop */
				endsqi = FSB(now->bishops[player] & ~(before->bishops[player]))
						- 1;
				notation[2] = getfile(endsqi);
				notation[3] = getrank(endsqi);
				notation[4] = 'b';
			} else if (POPCNT(
					before->knights[player]) != POPCNT(now->knights[player])) { /* Pawn promoted to knight */
				endsqi = FSB(now->knights[player] & ~(before->knights[player]))
						- 1;
				notation[2] = getfile(endsqi);
				notation[3] = getrank(endsqi);
				notation[4] = 'n';
			}
		}
	} else if (before->queens[player] != now->queens[player]) { /* A queen moved */
		startsqi = FSB(before->queens[player] & ~(now->queens[player])) - 1;
		endsqi = FSB(now->queens[player] & ~(before->queens[player])) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		notation[2] = getfile(endsqi);
		notation[3] = getrank(endsqi);
	} else if (before->rooks[player] != now->rooks[player]) { /* A rook moved */
		startsqi = FSB(before->rooks[player] & ~(now->rooks[player])) - 1;
		endsqi = FSB(now->rooks[player] & ~(before->rooks[player])) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		notation[2] = getfile(endsqi);
		notation[3] = getrank(endsqi);
	} else if (before->bishops[player] != now->bishops[player]) { /* A bishop moved */
		startsqi = FSB(before->bishops[player] & ~(now->bishops[player])) - 1;
		endsqi = FSB(now->bishops[player] & ~(before->bishops[player])) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		notation[2] = getfile(endsqi);
		notation[3] = getrank(endsqi);
	} else if (before->knights[player] != now->knights[player]) { /* A knight moved */
		startsqi = FSB(before->knights[player] & ~(now->knights[player])) - 1;
		endsqi = FSB(now->knights[player] & ~(before->knights[player])) - 1;
		notation[0] = getfile(startsqi);
		notation[1] = getrank(startsqi);
		notation[2] = getfile(endsqi);
		notation[3] = getrank(endsqi);
	}
	return EXIT_SUCCESS;
}

//int main(void) {
//	chessposition position, movelist[200];
//	char testfen[] = "7R/1bk5/6Q1/2B5/8/8/8/2Q1K2R w K - 0 1";
//
//	init();
//	printf("Error code = %d\n", importpos(testfen, &position));
//	//vispos(position);
//	printf("%d possible moves.\n", generatemoves(position, movelist));
//	destroymagictbls();
//	return EXIT_SUCCESS;
//}
