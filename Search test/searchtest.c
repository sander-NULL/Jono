/*
 * searchtest.c
 *
 *  Created on: Dec 20, 2020
 *      Author: sander
 *      Usage: searchtest	--timeout t (seconds, integer between 1 and 9999, the latter bound is due to format reasons, default is 10s)
 *      					--pos position (line in testpositions.txt)
 */

#define SEARCHTEST		// Let the preprocessor know that the search is on a test bench
						// This will put some counters in the code for testing

#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "../bitboards.h"
#include "../movgen.h"
#include "../alphabeta.h"

int go;
pv prinvar;
FILE *logfile;
long nodes;			// Number of calls of alphabeta_negamax_pv()/alphabeta_negamax_pvguess()
long staticEvals;	// Number of calls of staticeval()
long gameEnds;		// Number of leaf nodes in which a game ended through check mate or stalemate
int timeOut = 10;	// how much time to invest for each position

void* timer(void *dummy) {	// argument is not needed, but pthread_create() requires a ptr to such a function
	struct timespec time;

	time.tv_sec = timeOut;
	time.tv_nsec = 0;
	nanosleep(&time, NULL);
	go = 0;
	printf("Timed out after %ds.\n", timeOut);
	fprintf(logfile, "Timed out after %ds.\n", timeOut);
	return NULL;
}

int main(int argc, char *argv[]) {
	chessposition pos, tmpPos, tmpMoveList[200];
	clock_t start, end;
	pthread_t thTimeOut;
	FILE *searchtest;
	char fenString[100], moveNota[6];
	int maxDepth, line, i, j, stop = 0;			// stop determines whether to stop after one position, default is no (0)
	double cpuTime;
	pv curPV;

	init();
	logfile = fopen("../Search test/searchtest.log", "w");
	searchtest = fopen("../Search test/testpositions.txt", "r");
	if (logfile == NULL || searchtest == NULL) {
		printf("Error creating searchtest.log or reading testpositions.txt.\n");
		return EXIT_FAILURE;
	}

	i = 1;
	while (i < argc) {											// while there is something to parse
		if (strcmp(argv[i], "--pos") == 0) {					// position option is used
			i++;
			if (argc <= i) {
				printf("Error: No line after --pos.\n");
				fprintf(logfile, "Error: No line given after --pos.\n");
				return EXIT_FAILURE;
			}
			line = strtol(argv[i], NULL, 10);					// read only this line in testpositions.txt
			if (line <= 0) {
				printf("Error: No such line.\n");
				fprintf(logfile, "Error: No such line.\n");
				return EXIT_FAILURE;
			}
			stop = 1;											// stop after one position
			for (j = 1; j < line; j++)							// set the file pointer to beginning of line-th line
				if (fgets(fenString, 100, searchtest) == NULL) {
					printf("Error reading FEN.\n");
					fprintf(logfile, "Error reading FEN.\n");
					return EXIT_FAILURE;
				}
		}
		else if (strcmp(argv[i], "--timeout") == 0) {
			i++;
			if (argc <= i) {
				printf("Error: No time given.\n");
				fprintf(logfile, "Error: No time given.\n");
				return EXIT_FAILURE;
			}
			timeOut = strtol(argv[i], NULL, 10);
			if (timeOut <= 0 || timeOut >= 10000) {
				printf("Error: Timeout not in range between 1s and 9999s.\n");
				fprintf(logfile, "Error: Timeout not in range between 1s and 9999s.\n");
				return EXIT_FAILURE;
			}
		}
		i++;
	}
	do {
		if (fgets(fenString, 100, searchtest) == NULL) {
			if (feof(searchtest)) {
				printf("End of file reached.\n");
				fprintf(logfile, "End of file reached.\n");
				return EXIT_SUCCESS;
			}
			printf("Error reading FEN.\n");
			fprintf(logfile, "Error reading FEN.\n");
			return EXIT_FAILURE;
		}
		if (importpos(fenString, &pos) != 0) {
			printf("Error importing position %s\n", fenString);
			fprintf(logfile, "Error importing position %s\n", fenString);
			return EXIT_FAILURE;
		}

		printf("Position %s imported: %d\n\n", strtok(fenString, "\n"), importpos(fenString, &pos));
		fprintf(logfile, "Position %s imported: %d\n\n", strtok(fenString, "\n"), importpos(fenString, &pos));
		printf("depth |    time    | nodes visited |  static evals | game ends |\t\tuci text\n");
		printf("---------------------------------------------------------------------------------------------\n");
		fprintf(logfile, "depth |    time    | nodes visited |  static evals | game ends |\t\tuci text\n");
		fprintf(logfile,
				"---------------------------------------------------------------------------------------------\n");
		// The search shouldn't use something that might be left over
		// from previous searches in the PV
		prinvar.len = 0;
		maxDepth = 1;
		go = 1;
		pthread_create(&thTimeOut, NULL, &timer, NULL); // Create a thread for the time out
		do {
			nodes = 0;
			staticEvals = 0;
			gameEnds = 0;
			start = clock();
			curPV = alphabeta_negamax_pvguess(&pos, maxDepth, 0, &prinvar, -INF,
			INF);
			//curPV = alphabeta_negamax_pv(&pos, maxDepth, 0, -INF,
			//				INF);
			end = clock();
			// MINOR ISSUE: What if above line completed but at this line of code go changes to 0?
			if (go) { // Last call of alphabeta_negamax_pvguess was not aborted
				cpuTime = (double) (end - start) / CLOCKS_PER_SEC;
				printf("%5d | %9.5fs | %13ld | %13ld | %9ld | ", maxDepth, cpuTime, nodes, staticEvals, gameEnds);
				fprintf(logfile, "%5d | %9.5fs | %13ld | %13ld | %9ld | ", maxDepth, cpuTime, nodes, staticEvals,
						gameEnds);

				prinvar = curPV;

				if (abs(prinvar.eval) > CHECKMATE / 2) {
					// Mate is coming

					if (prinvar.eval > 0)
						// Engine is mating
						printf("score mate %d ", (CHECKMATE - prinvar.eval) / 2 + 1); // Mate in moves - not plies
					else
						// Engine is getting mated
						printf("score mate %d ", (-CHECKMATE - prinvar.eval) / 2); // Mate in moves - not plies
				}
				else {
					printf("score cp %-6d ", prinvar.eval);
					fprintf(logfile, "score cp %-6d ", prinvar.eval);
				}

				// Print the PV
				printf("pv ");
				fprintf(logfile, "pv ");
				tmpPos = pos;
				for (j = 0; j <= prinvar.len - 1; j++) {
					generatemoves(tmpPos, tmpMoveList);
					getmovenotation(&tmpPos, tmpMoveList + prinvar.pvi[j], moveNota);
					printf("%s ", moveNota);
					fprintf(logfile, "%s ", moveNota);
					tmpPos = tmpMoveList[prinvar.pvi[j]];
				}
				printf("\n");
				fprintf(logfile, "\n");
				fflush(stdout);
			}

			// If PV is shorter then searched depth, then a mate or
			// stalemate is detected and we can stop the search
			if (prinvar.len < maxDepth)
				go = 0;
			maxDepth++;
		} while (go);

		// Print best move found
		if (generatemoves(pos, tmpMoveList) > 0) {
			getmovenotation(&pos, tmpMoveList + prinvar.pvi[0], moveNota);
			printf("bestmove %s\n", moveNota);
			fprintf(logfile, "bestmove %s\n", moveNota);
		}
		pthread_join(thTimeOut, NULL);
		printf("\n");
		fprintf(logfile, "\n");
	} while (!feof(searchtest) && (stop == 0));
	fclose(logfile);
	fclose(searchtest);
	destroymagictbls();
	return EXIT_SUCCESS;
}
