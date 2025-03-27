/*
 * alphabeta.c
 *
 *  Created on: Apr 4, 2020
 *      Author: sander
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bitboards.h"
#include "movgen.h"
#include "debug.h"
#include "alphabeta.h"

//extern int getmovenotation(chessposition *before, chessposition *now,
//		char *notation);

// If we are on a test bench we will need these variables
#ifdef SEARCHTEST
#pragma message "Compiling for test bench."
extern long nodes;
extern long staticEvals;
extern long gameEnds;
#endif

int staticeval(chessposition pos) {
#ifdef SEARCHTEST
	staticEvals++;
#endif
	return 900 * POPCNT(pos.pieces[QUEENS][WHITE]) - 900 * POPCNT(pos.pieces[QUEENS][BLACK])
			+ 500 * POPCNT(pos.pieces[ROOKS][WHITE]) - 500 * POPCNT(pos.pieces[ROOKS][BLACK])
			+ 325 * POPCNT(pos.pieces[BISHOPS][WHITE])
			- 325 * POPCNT(pos.pieces[BISHOPS][BLACK])
			+ 300 * POPCNT(pos.pieces[KNIGHTS][WHITE])
			- 300 * POPCNT(pos.pieces[KNIGHTS][BLACK]) + 100 * POPCNT(pos.pieces[PAWNS][WHITE])
			- 100 * POPCNT(pos.pieces[PAWNS][BLACK]);
}

struct bestmov alphabeta_bestmov(chessposition *position, int depth, int alpha,
		int beta) {

	int i = 0, movcount, player, playeropp, sign;
	chessposition movlist[200];
	struct bestmov tmp;
	struct bestmov ret;

	player = position->states.toMove;
	playeropp = (player + 1) % 2;
	sign = (player == WHITE) ? 1 : -1;

	//dynamic evaluation
//	if (position->states.plies >= 100) {
//		ret.eval = 0;	//50 move rule
//		ret.i = 0;	//first move is as good as any
//		return ret;
//	}

	if (depth == 0) { /* Are we at a leaf node? */
		ret.eval = staticeval(*position);
		ret.i = -1;
		return ret;
	} else {
		movcount = generatemoves(*position, movlist);
		if (movcount == 0) { /* Checkmate or stalemate detected */
			if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
					== 0) {
				ret.eval = 0;
				ret.i = -1;
				return ret;
			} else {
				ret.eval = -sign * CHECKMATE;
				ret.i = -1;
				return ret;
			}
		} else {
			if (player == WHITE) { /* We are at a maximizing node */
				ret.eval = alpha;
				while (ret.eval < beta && i < movcount) {
					tmp = alphabeta_bestmov(movlist + i, depth - 1, ret.eval,
							beta);
					if (tmp.eval > ret.eval) {
						ret.eval = tmp.eval;
						ret.i = i;
					}
					i++;
				}
			} else {
				ret.eval = beta;
				while (ret.eval > alpha && i < movcount) {
					tmp = alphabeta_bestmov(movlist + i, depth - 1, alpha,
							ret.eval);
					if (tmp.eval < ret.eval) {
						ret.eval = tmp.eval;
						ret.i = i;
					}
					i++;
				}
			}
		}
		return ret;
	}
}

struct bestmov alphabeta_negamax_bestmov(chessposition *position, int maxDepth,
		int depth, int alpha, int beta) {

	int i = 0, movcount, player, playeropp, sign;
	chessposition movlist[200];
	struct bestmov tmp;
	struct bestmov ret = { .eval = alpha, .i = 0 };

	player = position->states.toMove;
	playeropp = (player + 1) % 2;
	sign = (player == WHITE) ? 1 : -1;

	// Are we at a leaf node?
	if (maxDepth == 0) {
		ret.eval = sign * staticeval(*position);
		ret.i = 0;
		return ret;
	}

	movcount = generatemoves(*position, movlist);
	if (movcount == 0) { // Checkmate or stalemate detected
		if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
				== 0) {
			ret.eval = 0;
			ret.i = 0;
			return ret;
		} else {
			ret.eval = -CHECKMATE + depth;
			ret.i = 0;
			return ret;
		}
	}

	while (ret.eval < beta && i < movcount) {
		tmp = alphabeta_negamax_bestmov(movlist + i, maxDepth - 1, depth + 1,
				-beta, -ret.eval);
		if (-tmp.eval > ret.eval) {
			ret.eval = -tmp.eval;
			ret.i = i;
		}
		i++;
	}
	return ret;
}

pv alphabeta_negamax_pv(chessposition *position, int maxDepth, int depth,
		int alpha, int beta) {

	int i = 0, bestIndex, movCount, player, playerOpp, sign;
	chessposition movList[200];
	pv tmp;
	pv ret = { .eval = alpha, .len = 0 };

#ifdef SEARCHTEST
	nodes++;
#endif

	player = position->states.toMove;
	playerOpp = (player + 1) % 2;
	sign = (player == WHITE) ? 1 : -1;

	if (maxDepth == 0) {
		// We are at a leaf node
		ret.eval = sign * staticeval(*position);
		ret.pvi[0] = 0;
		ret.len = 0;
		return ret;
	}

	movCount = generatemoves(*position, movList);
	if (movCount == 0) {
		// Checkmate or stalemate detected
#ifdef SEARCHTEST
		gameEnds++;
#endif
		if ((position->pieces[KING][player] & getattsquares(playerOpp, *position))
				== 0) {
			// Stalemate detected
			ret.eval = 0;
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		} else {
			// Checkmate detected
			ret.eval = -CHECKMATE + depth;
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		}
	}

	while (ret.eval < beta && i < movCount && go) {
		tmp = alphabeta_negamax_pv(movList + i, maxDepth - 1, depth + 1, -beta,
				-ret.eval);
		if (-tmp.eval > ret.eval) {
			ret = tmp;
			ret.eval = -tmp.eval;
			bestIndex = i;
		}
		i++;
	}
	ret.len++;
	//ret.pvi[ret.len - 1] = bestIndex;
	ret.pvi[depth] = bestIndex;
	return ret;
}

pv alphabeta_negamax_pvguess(chessposition *position, int maxDepth, int depth,
		pv *pvguess, int alpha, int beta) {

#pragma message "To do: Add counter for how many times this function is called."
#pragma message "Check correctness of algorithm with position in comments."
	// Check r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1 in an iterative deepening framework
	// It seems to be slower than going for the depth right away
	int i = 0, firstIndex, bestIndex, movCount, player, playerOpp;
	chessposition movList[200];
	pv tmp;
	pv ret = { .eval = alpha, .len = 0 };

#ifdef SEARCHTEST
	nodes++;
#endif

	player = position->states.toMove;
	playerOpp = (player + 1) % 2;

	// Are we at a leaf node?
//	if (maxDepth == 0) {
//		ret.eval = sign * staticeval(*position);
//		ret.i = 0;
//		return ret;
//	}

	movCount = generatemoves(*position, movList);
	if (movCount == 0) {
		// Checkmate or stalemate detected
		if ((position->pieces[KING][player] & getattsquares(playerOpp, *position))
				== 0) {
			// Stalemate detected
			ret.eval = 0;
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		} else {
			// Checkmate detected
			ret.eval = -CHECKMATE + depth;
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		}
	}

	if (depth <= pvguess->len - 1) {
		// We are not at the end of the PV
		// This function only calls itself recursively if there is another move in the PV
		firstIndex = pvguess->pvi[depth];
		tmp = alphabeta_negamax_pvguess(movList + firstIndex, maxDepth - 1,
				depth + 1, pvguess, -beta, -ret.eval);
		if (-tmp.eval > ret.eval) {
			ret = tmp;
			ret.eval = -tmp.eval;
			bestIndex = firstIndex;
		}
	} else {
		// We are at the end of the PV
		// from here on call the normal alpha beta algo
		firstIndex = 0;
		tmp = alphabeta_negamax_pv(movList + firstIndex, maxDepth - 1,
				depth + 1, -beta, -ret.eval);
		if (-tmp.eval > ret.eval) {
			ret = tmp;
			ret.eval = -tmp.eval;
			bestIndex = firstIndex;
		}
	}

	// Examine the branches left of the PV branch with the normal alpha beta algo
	while (ret.eval < beta && i < firstIndex && go) {
		tmp = alphabeta_negamax_pv(movList + i, maxDepth - 1, depth + 1, -beta,
				-ret.eval);
		if (-tmp.eval > ret.eval) {
			ret = tmp;
			ret.eval = -tmp.eval;
			bestIndex = i;
		}
		i++;
	}
	// Now examine the branches right of the PV branch with the normal alpha beta algo
	i = firstIndex + 1;
	while (ret.eval < beta && i < movCount && go) {
		tmp = alphabeta_negamax_pv(movList + i, maxDepth - 1, depth + 1, -beta,
				-ret.eval);
		if (-tmp.eval > ret.eval) {
			ret = tmp;
			ret.eval = -tmp.eval;
			bestIndex = i;
		}
		i++;
	}
	ret.len++;
	ret.pvi[depth] = bestIndex;
	return ret;
}

pv iterativeDeepening(chessposition *position, int maxDepth) {

	int i;
	pv ret = { .len = 0 }, curPV = { .len = 0 };

	for (i = 1; i <= maxDepth; i++) {
		ret = alphabeta_negamax_pvguess(position, i, 0, &curPV, -INF, INF);
		curPV = ret;
	}
	return ret;
}

void* search(void* _position) { // argument is actually a chessposition* but pthread_create() needs a function pointer to a function taking a void* argument

	int maxDepth = 1, j;
	char moveNota[6];
	chessposition tmpPos, tmpMoveList[200];
	chessposition* position = (chessposition*) _position;
	pv curPV;

	// The search shouldn't use something that might be left over
	// from previous searches in the PV
	prinvar.len = 0;

	do {
		curPV = alphabeta_negamax_pvguess(position, maxDepth, 0, &prinvar, -INF,
		INF);
		// MINOR ISSUE: What if above line completed but at this line of code go changes to 0?
		if (go) {
			// Last call of alphabeta_negamax_pvguess was not aborted
			prinvar = curPV;

			printf("info depth %d ", maxDepth);
			if (abs(prinvar.eval) > CHECKMATE / 2) {
				// Mate is coming

				if (prinvar.eval > 0)
					// Engine is mating
					printf("score mate %d ",
							(CHECKMATE - prinvar.eval) / 2 + 1); // Mate in moves - not plies
				else
					// Engine is getting mated
					printf("score mate %d ", (-CHECKMATE - prinvar.eval) / 2); // Mate in moves - not plies
			} else
				printf("score cp %d ", prinvar.eval);

			// Print the PV to the GUI
			printf("pv ");
			tmpPos = *position;
			for (j = 0; j <= prinvar.len - 1; j++) {
				generatemoves(tmpPos, tmpMoveList);
				getmovenotation(&tmpPos, tmpMoveList + prinvar.pvi[j],
						moveNota);
				printf("%s ", moveNota);
				tmpPos = tmpMoveList[prinvar.pvi[j]];
			}
			printf("\n");
			fflush(stdout);
		}

		// If PV is shorter then searched depth, then a mate or
		// stalemate is detected and we can stop the search
		if (prinvar.len < maxDepth)
			go = 0;

		maxDepth++;
	} while (go);

	// Print best move found to GUI
	if(generatemoves(*position, tmpMoveList) > 0) {
		getmovenotation(position, tmpMoveList + prinvar.pvi[0], moveNota);
		printf("bestmove %s\n", moveNota);
	}
	fflush(stdout);
	return NULL;
}

pv minimaxinfo(chessposition *position, int depth, int level) {

	int i;
	int pvitmp;
	int player, playeropp;
	int sign;
	pv ret, tmp;
	int movcount;
	chessposition movlist[200];

	if (depth == 0) {
		ret.eval = staticeval(*position);
		ret.pvi[0] = 0;
		ret.len = 0;
		return ret;
	}

	player = position->states.toMove;
	playeropp = (player + 1) % 2;
	sign = (player == WHITE) ? -1 : 1;
	movcount = generatemoves(*position, movlist);

	if (movcount == 0) { /* Checkmate or stalemate detected */
		if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
				== 0) {
			ret.eval = 0;
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		} else {
			ret.eval = sign * (CHECKMATE - level);
			ret.pvi[0] = 0;
			ret.len = 0;
			return ret;
		}
	}
	ret = minimaxinfo(movlist, depth - 1, level + 1);
	pvitmp = 0;
	if (player == WHITE) {
		for (i = 1; i < movcount; i++) {
			tmp = minimaxinfo(movlist + i, depth - 1, level + 1);
			if (tmp.eval > ret.eval) {
				ret = tmp;
				pvitmp = i;
			}
		}
	} else {
		for (i = 1; i < movcount; i++) {
			tmp = minimaxinfo(movlist + i, depth - 1, level + 1);
			if (tmp.eval < ret.eval) {
				ret = tmp;
				pvitmp = i;
			}
		}
	}
	for (i = ret.len; i >= 1; i--)
		ret.pvi[i] = ret.pvi[i - 1];
	ret.pvi[0] = pvitmp;
	ret.len++;
	return ret;
}

//char getfile(int sqi) {
//	switch (sqi % 8) {
//	case 0:
//		return 'a';
//		break;
//	case 1:
//		return 'b';
//		break;
//	case 2:
//		return 'c';
//		break;
//	case 3:
//		return 'd';
//		break;
//	case 4:
//		return 'e';
//		break;
//	case 5:
//		return 'f';
//		break;
//	case 6:
//		return 'g';
//		break;
//	case 7:
//		return 'h';
//		break;
//	}
//	return -1;
//}
//
//char getrank(int sqi) {
//	switch (sqi / 8) {
//	case 0:
//		return '1';
//		break;
//	case 1:
//		return '2';
//		break;
//	case 2:
//		return '3';
//		break;
//	case 3:
//		return '4';
//		break;
//	case 4:
//		return '5';
//		break;
//	case 5:
//		return '6';
//		break;
//	case 6:
//		return '7';
//		break;
//	case 7:
//		return '8';
//		break;
//	}
//	return -1;
//}
//
//int getmovenotation(chessposition *before, chessposition *now, char *notation) {
//	int player;
//	int startsqi, endsqi;
//
//	player = before->states.toMove;
//	notation[4] = '\0';
//	notation[5] = '\0';
//
//	if (before->pieces[KING][player] != now->pieces[KING][player]) { /* The king moved */
//		startsqi = FSB(before->pieces[KING][player]) - 1;
//		endsqi = FSB(now->pieces[KING][player]) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		notation[2] = getfile(endsqi);
//		notation[3] = getrank(endsqi);
//	} else if (before->pieces[PAWNS][player] != now->pieces[PAWNS][player]) { /* A pawn moved */
//		startsqi = FSB(before->pieces[PAWNS][player] & ~(now->pieces[PAWNS][player])) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		if (POPCNT(before->pieces[PAWNS][player]) == POPCNT(now->pieces[PAWNS][player])) { /* Amount of players pawns has not changed */
//			endsqi = FSB(now->pieces[PAWNS][player] & ~(before->pieces[PAWNS][player])) - 1;
//			notation[2] = getfile(endsqi);
//			notation[3] = getrank(endsqi);
//		} else {
//			if (POPCNT(before->pieces[QUEENS][player]) != POPCNT(now->pieces[QUEENS][player])) { /* Pawn promoted to queen */
//				endsqi = FSB(now->pieces[QUEENS][player] & ~(before->pieces[QUEENS][player]))
//						- 1;
//				notation[2] = getfile(endsqi);
//				notation[3] = getrank(endsqi);
//				notation[4] = 'q';
//			} else if (POPCNT(
//					before->pieces[ROOKS][player]) != POPCNT(now->pieces[ROOKS][player])) { /* Pawn promoted to rook */
//				endsqi = FSB(now->pieces[ROOKS][player] & ~(before->pieces[ROOKS][player])) - 1;
//				notation[2] = getfile(endsqi);
//				notation[3] = getrank(endsqi);
//				notation[4] = 'r';
//			} else if (POPCNT(
//					before->pieces[BISHOPS][player]) != POPCNT(now->pieces[BISHOPS][player])) { /* Pawn promoted to bishop */
//				endsqi = FSB(now->pieces[BISHOPS][player] & ~(before->pieces[BISHOPS][player]))
//						- 1;
//				notation[2] = getfile(endsqi);
//				notation[3] = getrank(endsqi);
//				notation[4] = 'b';
//			} else if (POPCNT(
//					before->pieces[KNIGHTS][player]) != POPCNT(now->pieces[KNIGHTS][player])) { /* Pawn promoted to knight */
//				endsqi = FSB(now->pieces[KNIGHTS][player] & ~(before->pieces[KNIGHTS][player]))
//						- 1;
//				notation[2] = getfile(endsqi);
//				notation[3] = getrank(endsqi);
//				notation[4] = 'n';
//			}
//		}
//	} else if (before->pieces[QUEENS][player] != now->pieces[QUEENS][player]) { /* A queen moved */
//		startsqi = FSB(before->pieces[QUEENS][player] & ~(now->pieces[QUEENS][player])) - 1;
//		endsqi = FSB(now->pieces[QUEENS][player] & ~(before->pieces[QUEENS][player])) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		notation[2] = getfile(endsqi);
//		notation[3] = getrank(endsqi);
//	} else if (before->pieces[ROOKS][player] != now->pieces[ROOKS][player]) { /* A rook moved */
//		startsqi = FSB(before->pieces[ROOKS][player] & ~(now->pieces[ROOKS][player])) - 1;
//		endsqi = FSB(now->pieces[ROOKS][player] & ~(before->pieces[ROOKS][player])) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		notation[2] = getfile(endsqi);
//		notation[3] = getrank(endsqi);
//	} else if (before->pieces[BISHOPS][player] != now->pieces[BISHOPS][player]) { /* A bishop moved */
//		startsqi = FSB(before->pieces[BISHOPS][player] & ~(now->pieces[BISHOPS][player])) - 1;
//		endsqi = FSB(now->pieces[BISHOPS][player] & ~(before->pieces[BISHOPS][player])) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		notation[2] = getfile(endsqi);
//		notation[3] = getrank(endsqi);
//	} else if (before->pieces[KNIGHTS][player] != now->pieces[KNIGHTS][player]) { /* A knight moved */
//		startsqi = FSB(before->pieces[KNIGHTS][player] & ~(now->pieces[KNIGHTS][player])) - 1;
//		endsqi = FSB(now->pieces[KNIGHTS][player] & ~(before->pieces[KNIGHTS][player])) - 1;
//		notation[0] = getfile(startsqi);
//		notation[1] = getrank(startsqi);
//		notation[2] = getfile(endsqi);
//		notation[3] = getrank(endsqi);
//	}
//	return EXIT_SUCCESS;
//}
//
//int go = 1;
//pv prinvar;
//
//int main(int argc, char *argv[]) {
//
//	chessposition pos, tmp;
//	char fen[] =
//			"kN6/BRR5/N1P5/8/5p2/2K5/5P2/8 b - - 3 35 ";
//	//pv prinvar;
//	struct bestmov best;
//	char mov[6];
//	chessposition movlist[200];
//	int i = 0;
//	const int maxDepth = 4;
//	clock_t start, end;
//
//	init();
//	if (!importpos(fen, &pos))
//		printf("Position imported: OK.\n\n");
//	else {
//		printf("Position import ERROR.");
//		return -1;
//	}
//
//	search(&pos);
//	go = 1;
////	start = clock();
////	best = alphabeta_bestmov(&pos, maxDepth, -INF, INF);
////	end = clock();
////	printf("alphabeta_bestmov() eval = %.2f\n", (double) (best.eval) / 100);
////	generatemoves(pos, movlist);
////	getmovenotation(&pos, movlist + best.i, mov);
////	printf("Best move %c%c%c%c.\n", mov[0], mov[1], mov[2], mov[3]);
////	printf("Calculated in %.2f sec\n\n",
////			((double) (end - start)) / CLOCKS_PER_SEC);
////
////	start = clock();
////	best = alphabeta_negamax_bestmov(&pos, maxDepth, 0, -INF, INF);
////	end = clock();
////	printf("alphabeta_negamax_bestmov() eval = %.2f\n",
////			(double) (best.eval) / 100);
////	if (abs(best.eval) >= CHECKMATE / 2)
////		printf("Mate in %d plies.\n", abs(best.eval) - CHECKMATE);
////	generatemoves(pos, movlist);
////	getmovenotation(&pos, movlist + best.i, mov);
////	printf("Best move %c%c%c%c.\n", mov[0], mov[1], mov[2], mov[3]);
////	printf("Calculated in %.2f sec\n\n",
////			((double) (end - start)) / CLOCKS_PER_SEC);
//
//	start = clock();
//	prinvar = alphabeta_negamax_pv(&pos, maxDepth, 0, -INF, INF);
//	end = clock();
//	if (abs(prinvar.eval) >= CHECKMATE / 2)
//		printf("Mate in %d plies.\n", prinvar.eval - CHECKMATE);
//	else
//		printf("alphabeta_negamax_pv() eval = %.2f\n",
//				(double) (prinvar.eval) / 100);
//	printf("PV length = %d\n", prinvar.len);
//	printf("PV: ");
//	tmp = pos;
//	for (i = 0; i <= prinvar.len - 1; i++) {
//		generatemoves(tmp, movlist);
//		getmovenotation(&tmp, movlist + prinvar.pvi[i], mov);
//		printf("%s - ", mov);
//		tmp = movlist[prinvar.pvi[i]];
//	}
//	printf("\nCalculated in %.2f sec\n\n",
//			((double) (end - start)) / CLOCKS_PER_SEC);
//
//	/***************************************/
////	pvguess = alphabeta_negamax_pv(&pos, maxDepth - 1, 0, -INF, INF);
////	start = clock();
////	prinvar = alphabeta_negamax_pvguess(&pos, maxDepth, 0, &pvguess, -INF, INF);
////	end = clock();
////	if (abs(prinvar.eval) >= CHECKMATE / 2)
////		printf("Mate in %d plies.\n", abs(prinvar.eval - CHECKMATE));
////	else
////		printf("alphabeta_negamax_bestmov_pvguess() eval = %.2f\n",
////				(double) (prinvar.eval) / 100);
////	printf("PV length = %d\n", prinvar.len);
////	printf("PV: ");
////	tmp = pos;
////	for (i = 0; i <= prinvar.len - 1; i++) {
////		generatemoves(tmp, movlist);
////		getmovenotation(&tmp, movlist + prinvar.pvi[i], mov);
////		printf("%s - ", mov);
////		tmp = movlist[prinvar.pvi[i]];
////	}
////	printf("\nCalculated in %.2f sec\n\n",
////			((double) (end - start)) / CLOCKS_PER_SEC);
//	start = clock();
//	prinvar = iterativeDeepening(&pos, maxDepth);
//	end = clock();
//	if (abs(prinvar.eval) >= CHECKMATE / 2)
//		printf("Mate in %d plies.\n", abs(prinvar.eval - CHECKMATE));
//	else
//		printf("iterativeDeepening() eval = %.2f\n",
//				(double) (prinvar.eval) / 100);
//	printf("PV length = %d\n", prinvar.len);
//	printf("PV: ");
//	tmp = pos;
//	for (i = 0; i <= prinvar.len - 1; i++) {
//		generatemoves(tmp, movlist);
//		getmovenotation(&tmp, movlist + prinvar.pvi[i], mov);
//		printf("%s - ", mov);
//		tmp = movlist[prinvar.pvi[i]];
//	}
//	printf("\nCalculated in %.2f sec\n\n",
//			((double) (end - start)) / CLOCKS_PER_SEC);
//	/***************************************/
//
////	start = clock();
////	prinvar = minimaxinfo(&pos, maxDepth, 0);
////	end = clock();
////	if (abs(prinvar.eval) >= CHECKMATE / 2)
////		printf("Mate in %d plies.\n", abs(prinvar.eval - CHECKMATE));
////	else
////		printf("minimaxinfo() eval = %.2f\n", (double) (prinvar.eval) / 100);
////	printf("PV length = %d\n", prinvar.len);
////	printf("PV: ");
////	tmp = pos;
////	for (i = 0; i < prinvar.len; i++) {
////		generatemoves(tmp, movlist);
////		getmovenotation(&tmp, movlist + prinvar.pvi[i], mov);
////		printf("%s - ", mov);
////		tmp = movlist[prinvar.pvi[i]];
////	}
////	printf("\nCalculated in %.2f sec\n\n",
////			((double) (end - start)) / CLOCKS_PER_SEC);
//	destroymagictbls();
//	return EXIT_SUCCESS;
//}
//
////int alphabeta(chessposition *position, int depth, int alpha, int beta) {
////
////	int i = 0, tmp, movcount, player, playeropp, sign, value;
////	chessposition movlist[200];
////
////	player = position->states.toMove;
////	playeropp = (player + 1) % 2;
////	sign = (player == WHITE) ? 1 : -1;
////
////	// Are we at a leaf node?
////	if (depth == 0)
////		return staticeval(*position);
////
////	movcount = generatemoves(*position, movlist);
////	if (movcount == 0) { // Checkmate or stalemate detected
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position)) == 0)
////			return 0;
////		else
////			return -sign * CHECKMATE;
////	}
////
////	if (player == WHITE) { // We are at a maximizing node
////		value = alpha;
////		while (value < beta && i < movcount) {
////			tmp = alphabeta(movlist + i, depth - 1, value, beta);
////			if (tmp > value)
////				value = tmp;
////			i++;
////		}
////	} else {	// We are at a minimizing node
////		value = beta;
////		while (value > alpha && i < movcount) {
////			tmp = alphabeta(movlist + i, depth - 1, alpha, value);
////			if (tmp < value)
////				value = tmp;
////			i++;
////		}
////	}
////	return value;
////}
////
////int alphabeta_negamax(chessposition *position, int depth, int alpha, int beta) {
////
////	int i = 0, tmp, movcount, player, playeropp, sign, value = alpha;
////	chessposition movlist[200];
////
////	player = position->states.toMove;
////	playeropp = (player + 1) % 2;
////	sign = (player == WHITE) ? 1 : -1;
////
////	// Are we at a leaf node?
////	if (depth == 0)
////		return sign * staticeval(*position);
////
////	movcount = generatemoves(*position, movlist);
////	if (movcount == 0) { // Checkmate or stalemate detected
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position)) == 0)
////			return 0;
////		else
////			return -CHECKMATE;
////	}
////	// What should happen below this line?
////	//value = alpha;
////	while (value < beta && i < movcount) {
////		tmp = -alphabeta_negamax(movlist + i, depth - 1, -beta, -value);
////		if (tmp > value)
////			value = tmp;
////		i++;
////	}
////	return value;
////}
////
////struct bestmov alphabeta_negamax_bestmov(chessposition *position, int depth,
////		int alpha, int beta) {
////
////	int i = 0, movcount, player, playeropp, sign;
////	chessposition movlist[200];
////	struct bestmov tmp;
////	struct bestmov ret = { .eval = alpha, .i = 0 };
////
////	player = position->states.toMove;
////	playeropp = (player + 1) % 2;
////	sign = (player == WHITE) ? 1 : -1;
////
////	// Are we at a leaf node?
////	if (depth == 0) {
////		ret.eval = sign * staticeval(*position);
////		ret.i = 0;
////		return ret;
////	}
////
////	movcount = generatemoves(*position, movlist);
////	if (movcount == 0) { // Checkmate or stalemate detected
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
////				== 0) {
////			ret.eval = 0;
////			ret.i = 0;
////			return ret;
////		} else {
////			ret.eval = -CHECKMATE;
////			ret.i = 0;
////			return ret;
////		}
////	}
////	// What should happen below this line?
////	//value = alpha;
////	while (ret.eval < beta && i < movcount) {
////		tmp = alphabeta_negamax_bestmov(movlist + i, depth - 1, -beta,
////				-ret.eval);
////		if (-tmp.eval > ret.eval) {
////			ret.eval = -tmp.eval;
////			ret.i = i;
////		}
////		i++;
////	}
////	return ret;
////}
////
////int minimax(chessposition *position, int depth) {
////
////	int i;
////	int player, playeropp;
////	int sign;
////	int value;
////	int tmp;
////	int movcount;
////	chessposition movlist[200];
////
////	if (depth == 0)
////		return staticeval(*position);
////
////	player = position->states.toMove;
////	playeropp = (player + 1) % 2;
////	sign = (player == WHITE) ? -1 : 1;
////	movcount = generatemoves(*position, movlist);
////
////	if (movcount == 0) { /* Checkmate or stalemate detected */
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position)) == 0)
////			return 0;
////		else
////			return sign * CHECKMATE;
////	}
////	value = minimax(movlist, depth - 1);
////	if (player == WHITE) {
////		for (i = 1; i < movcount; i++) {
////			tmp = minimax(movlist + i, depth - 1);
////			if (tmp > value)
////				value = tmp;
////		}
////	} else {
////		for (i = 1; i < movcount; i++) {
////			tmp = minimax(movlist + i, depth - 1);
////			if (tmp < value)
////				value = tmp;
////		}
////	}
////	return value;
////}
////
////struct bestmov minimax_bestmov(chessposition *position, int depth, int level) {
////
////	int i;
////	int best;
////	int player, playeropp;
////	int sign;
////	int movcount;
////	chessposition movlist[200];
////	struct bestmov tmp;
////	struct bestmov ret;
////
//////	if (position->states.plies >= 100) {
//////		ret.eval = 0;	//50 move rule
//////		ret.i = 0;	//first move is as good as any
//////		return ret;
//////	}
////
////	if (depth == 0) {
////		ret.eval = staticeval(*position);
////		ret.i = 0;	//first move is as good as any at leaf node
////		return ret;
////	}
////
////	player = position->states.toMove;
////	playeropp = (player + 1) % 2;
////	sign = (player == WHITE) ? -1 : 1;
////	movcount = generatemoves(*position, movlist);
////
////	if (movcount == 0) { /* Checkmate or stalemate detected */
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
////				== 0) {
////			ret.eval = 0;
////			ret.i = -1;
////			return ret;
////		} else {
////			ret.eval = sign * (CHECKMATE - level);
////			ret.i = -1;
////			return ret;
////		}
////	}
////	ret = minimax_bestmov(movlist, depth - 1, level + 1);
////	best = 0;
////	if (player == WHITE) {
////		for (i = 1; i < movcount; i++) {
////			tmp = minimax_bestmov(movlist + i, depth - 1, level + 1);
////			if (tmp.eval > ret.eval) {
////				ret.eval = tmp.eval;
////				best = i;
////			}
////		}
////	} else {
////		for (i = 1; i < movcount; i++) {
////			tmp = minimax_bestmov(movlist + i, depth - 1, level + 1);
////			if (tmp.eval < ret.eval) {
////				ret.eval = tmp.eval;
////				best = i;
////			}
////		}
////	}
////	ret.i = best;
////	return ret;
////}
////
////int negamax(chessposition *position, int depth) {
////	int value = -INF;
////	int i, tmp;
////	const int player = position->states.toMove;
////	const int playeropp = (player + 1) % 2;
////	const int sign = (player == WHITE) ? 1 : -1;
////	int movcount;
////	chessposition movlist[200];
////
////	if (depth == 0) {
////		return sign * staticeval(*position);
////	}
////
////	movcount = generatemoves(*position, movlist);
////	if (movcount == 0) { /* Checkmate or stalemate detected */
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position)) == 0)
////			return 0;
////		else
////			return -CHECKMATE;
////	}
////	for (i = 0; i < movcount; i++) {
////		tmp = (-1) * negamax(movlist + i, depth - 1);
////		if (tmp > value)
////			value = tmp;
////	}
////	return value;
////}
////
////struct bestmov negamax_bestmov(chessposition *position, int depth) {
////	int i, movcount;
////	const int player = position->states.toMove;
////	const int playeropp = (player + 1) % 2;
////	const int sign = (player == WHITE) ? 1 : -1;
////	chessposition movlist[200];
////	struct bestmov tmp;
////	struct bestmov ret = { .eval = -INF };
////
////	if (depth == 0) {
////		ret.eval = sign * staticeval(*position);
////		ret.i = 0;
////		return ret;
////	}
////
////	movcount = generatemoves(*position, movlist);
////	if (movcount == 0) { /* Checkmate or stalemate detected */
////		if ((position->pieces[KING][player] & getattsquares(playeropp, *position))
////				== 0) {
////			ret.eval = 0;
////			ret.i = -1;
////			return ret;
////		} else {
////			ret.eval = -CHECKMATE;
////			ret.i = -1;
////			return ret;
////		}
////	}
////	for (i = 0; i < movcount; i++) {
////		tmp = negamax_bestmov(movlist + i, depth - 1);
////		tmp.eval *= -1;
////		if (tmp.eval > ret.eval) {
////			ret = tmp;
////			ret.i = i;
////		}
////	}
////	return ret;
////}
