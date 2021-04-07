/*
 * alphabeta.h
 *
 *  Created on: Apr 7, 2020
 *      Author: sander
 */

#ifndef ALPHABETA_H_
#define ALPHABETA_H_

#define INF 3000000
#define CHECKMATE 100000

typedef struct pv {
	int eval;
	int len;
	int pvi[10];
} pv;

struct bestmov {
	int eval;
	int i;
};

extern int go;
extern pv prinvar;

void search(chessposition* position);
int staticeval(chessposition pos);
struct bestmov alphabeta_bestmov(chessposition *position, int depth, int alpha,
		int beta);
pv alphabeta_negamax_pv(chessposition *position, int maxDepth,
		int depth, int alpha, int beta);
pv iterativeDeepening(chessposition *position, int maxDepth);
pv minimaxinfo(chessposition *position, int depth, int level);

#ifdef SEARCHTEST		// Only offer this function if searchtest.c includes this header
pv alphabeta_negamax_pvguess(chessposition *position, int maxDepth, int depth,
		pv *pvguess, int alpha, int beta);
#endif

#endif /* ALPHABETA_H_ */
