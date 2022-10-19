/*
 * perft.c
 *
 *  Created on: Mar 30, 2020
 *      Author: sander
 */

#include <stdio.h>
#include <time.h>
#include "../bitboards.h"
#include "../movgen.h"
#include "../alphabeta.h"

int go = 1;		// global variable defined in Jono/main.c - needed to compile Perft correctly
pv prinvar;		// global variable defined in Jono/main.c - needed to compile Perft correctly

long countMovs(chessposition pos, int depth) {
	int i, movCount;
	long cnt = 0;
	chessposition movList[200];

	movCount = generatemoves(pos, movList);

	if (depth == 1)
		return movCount;
	else {
		for (i = 0; i < movCount; i++)
			cnt += countMovs(movList[i], depth - 1);
	}
	return cnt;
}

int main(void) {
	chessposition pos;
	clock_t start, startTotal, end, endTotal;
	double cpuTime;
	long nodes, result[10];
	int i, depth;
	FILE *log, *perft;
	char tmp[20], fenString[100];

	init();
	log = fopen("../Perft/perft.log", "w");
	perft = fopen("../Perft/perftresults_quick.txt", "r");
	if (log == NULL || perft == NULL) {
		printf("Error creating perft.log or reading perftres_quick.txt\n");
		return EXIT_FAILURE;
	}

	startTotal = clock();
	while (!feof(perft)) {
		if (fgets(fenString, 100, perft) == NULL) {			// Read the line containing the position in FEN
			printf("Error reading FEN.\n");
			fprintf(log, "Error reading FEN.\n");
			return EXIT_FAILURE;
		}

		if (fgets(tmp, 20, perft) == NULL) {
			printf("Error reading maximal depth.\n");		// Next line contains the maximal depth up to which the test runs
			fprintf(log, "Error reading maximal depth.\n");
			return EXIT_FAILURE;
		}
		depth = strtol(tmp, NULL, 10);
		for (i = 1; i <= depth; i++) {
			if (fgets(tmp, 20, perft) == NULL) {			// Read the (depth) results
				printf("Error reading results.\n");
				fprintf(log, "Error reading results.\n");
				return EXIT_FAILURE;
			}
			result[i - 1] = strtol(tmp, NULL, 10);
		}

		printf("Position %s imported: %d\n", strtok(fenString, "\n"),
				importpos(fenString, &pos));
		fprintf(log, "Position %s imported: %d\n", strtok(fenString, "\n"),
				importpos(fenString, &pos));
		for (i = 1; i <= depth; i++) {
			start = clock();
			nodes = countMovs(pos, i);
			end = clock();
			cpuTime = (double) (end - start) / CLOCKS_PER_SEC;
			printf("%ld nodes\t\t\tat depth %d found in %.2fs.\n", nodes, i,
					cpuTime);
			fprintf(log, "%ld nodes\t\t\tat depth %d found in %.2fs.\n", nodes,
					i, cpuTime);
			if (nodes != result[i - 1]) {
				printf(
						"*** Unexpected number of nodes! Expected were %ld nodes. ***\n",
						result[i - 1]);
				fprintf(log,
						"*** Unexpected number of nodes! Expected were %ld nodes. ***\n",
						result[i - 1]);
			}
		}
		printf("\n");
		fprintf(log, "\n");
		if (fgets(tmp, 20, perft) == NULL)	// Read the empty line between positions
			break;
	}
	endTotal = clock();
	cpuTime = (double) (endTotal - startTotal) / CLOCKS_PER_SEC;
	printf("Task completed in %.2fs.\n", cpuTime);
	fprintf(log, "Task completed in %.2fs.\n", cpuTime);
	fclose(log);
	fclose(perft);
	destroymagictbls();
	return EXIT_SUCCESS;
}
