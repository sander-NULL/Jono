/*
 * compare.c
 *
 *  Created on: 	Jan 16, 2021
 *      Author: 	sander
 *      Purpose:	Compares two builds of searchtest
 *      Usage: 		compare --timeout t (seconds, integer between 1 and 9999, the latter bound is due to format reasons)
 *      					--pos position (line in testpositions.txt) (only 1000 lines are supported, default is all)
 *      					--iterations n (number of iterations each position will be tested, default is 10, max is 99 due to format reasons)
 */

#define POSARGSIZE 12 // length of " --pos xxxx" including terminating \0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct list_t {
	int length;
	double *entry;
} list_t;

void printRel(FILE *f, double a, double b) {
	// prints the relation between two reals
	if (a < b) {
		printf(" < ");
		fprintf(f, " < ");
	}
	else if (a > b) {
		printf(" > ");
		fprintf(f, " > ");
	}
	else {
		printf(" = ");
		fprintf(f, " = ");
	}
}

void printIterInfo(FILE *f, list_t* list, int max_iter) {
	// prints in how many iterations depth was reached
	// but only if it didn't reach it at least once
	if (list->length < max_iter) {
		printf(" (%2d/%2d)", list->length, max_iter);
		fprintf(f, " (%2d/%2d)", list->length, max_iter);
	}
	else {
		printf("        ");
		fprintf(f, "        ");
	}
}

int minIndex(list_t *list) {
	// returns the index of the minimum of a list of numbers
	int i, minI = 0;

	// return negative index if list has no entries
	if (list->length <= 0)
		return -1;

	for (i = 1; i < list->length; i++) {
		if (list->entry[i] < list->entry[minI])
			minI = i;
	}
	return minI;
}

double getMedian(list_t *list) {
	// returns the median of a list of positive numbers
	int i, j, len, minI;
	double med, tmp1 = 0, tmp2 = 0;
	list_t tmpList;

	// keep the original length of the list
	len = list->length;
	if (len <= 0)
		return -1;

	tmpList.entry = malloc(len * sizeof(double));
	if (tmpList.entry == NULL)
		return -1;

	// copy list to tmpList
	for (i = 0; i < len; i++)
		tmpList.entry[i] = list->entry[i];
	tmpList.length = len;

	// remove the first len/2 (remember (2n+1)/2 = n) elements from the list
	for (j = 1; j <= len / 2; j++) {
		minI = minIndex(&tmpList);
		// note that minI is well-defined since the length
		// of the list is >= 0

		// we are only interested in the value of the last iteration
		// if the list has 2n entries this will be the n-th aka the smaller middle value
		tmp1 = tmpList.entry[minI];

		// remove the minimum from the list
		for (i = minI + 1; i < tmpList.length; i++)
			tmpList.entry[i - 1] = tmpList.entry[i];
		tmpList.length--;
	}

	// if the list has an odd number of entries take the middle one
	// otherwise take the arithmetic mean of the two middle ones
	if (len % 2 == 1)
		med = tmpList.entry[minIndex(&tmpList)];
	else {
		// first get the bigger middle value
		tmp2 = tmpList.entry[minIndex(&tmpList)];
		// then take the arithmetic mean of the two middle values
		med = (tmp1 + tmp2)/2;
	}
	free(tmpList.entry);
	//med = 0.5;
	return med;
}

int main(int argc, char *argv[]) {
	// measurement contains time[i] at which depth i+1
	// was reached for one position and the maximum depth reached
	typedef struct measurement_t {
		int maxDepth;
		double *time;
	} measurement_t;

	measurement_t *measurement[2] = { NULL, NULL };

	// variables to obtain the medians later
	list_t list[2];

	// the results of the two builds
	double average[2];

	// variables to store medians
	double median[2];

	// log of this binary and the one created by the two searchtests
	FILE *cmpLog = NULL, *searchLog = NULL, *testPos = NULL;

	long len;

	// number of iterations, default is 10, and what position shall be tested, default (0) is testing all positions
	int max_iter = 10, iter, max_pos = 0, pos = 0, depth;

	// some counters, a var that counts the lines in testpositions.txt and return value of main()
	int i, j, build, ret = EXIT_FAILURE;

	// the strings we use later to execute the two searchtests
	// partial call string without "--pos n" argument
	char *partCallStr[2] = { NULL, NULL }, *tmp = NULL;
	char *callStr[2] = { NULL, NULL };
	char ch;

	// string containing the --pos argument (up to 4 digits --pos xxxx)
	char posArg[POSARGSIZE];

	// the names of the two binaries of the searchtests
	const char binary[2][14] = { { "./searchtest1" }, { "./searchtest2" } };

	// patch together the two call strings
	for (build = 0; build <= 1; build++) {

		// copy the name of the binary into the call string
		partCallStr[build] = strdup(binary[build]);

		if (partCallStr[build] == NULL) {
			printf("Error: Could not allocate memory for the call string.\n");
			goto cleanUp;
		}

		len = strlen(partCallStr[build]) + 1;

		// now allocate space for the params and copy them
		for (i = 1; i < argc; i++) {
			// do not use the --iterations option for searchtest (it is not supported by searchtest)
			if (strcmp(argv[i], "--iterations") == 0) {
				i++;	// point past the number given after --iterations after next loop iteration
				if (argc <= i) {
					printf("Error: No number after --iterations.\n");
					goto cleanUp;
				}
				max_iter = strtol(argv[i], NULL, 10);					// read only this line
				if (max_iter <= 0 || max_iter >= 100) {
					printf("Error: Iterations not in the range between 1 and 99.\n");
					goto cleanUp;
				}
			}
			else if (strcmp(argv[i], "--pos") == 0) {
				i++;
				if (argc <= i) {
					printf("Error: No line after --pos.\n");
					goto cleanUp;
				}
				pos = strtol(argv[i], NULL, 10);
				if (pos <= 0) {
					printf("Error: No such position.\n");
					goto cleanUp;
				}
				// user has chosen to test only one position
				max_pos = pos;
			}
			else {
				// just forward this option to the two searchtest builds
				len += strlen(argv[i]) + 1;	// + 1 for the additional whitespace
				tmp = realloc(partCallStr[build], len);

				if (tmp == NULL) {
					printf("Error allocating memory for the call string.\n");
					goto cleanUp;
				}

				partCallStr[build] = tmp;
				strcat(strcat(partCallStr[build], " "), argv[i]);
			}
		}

		// if max_pos is still 0, then the user wants to test all positions and we need
		// to determine how many test positions are in testpositions.txt
		if (max_pos == 0) {
			// since we need to test all positions start the test at the first position
			pos = 1;
			testPos = fopen("../Search test/testpositions.txt", "r");
			if (testPos == NULL) {
				printf("Error opening testpositions.txt.\n");
				goto cleanUp;
			}

			// count how many positions (lines) are in testpositions.txt
			// first count all new lines '\n' that have characters coming
			// after it
			do {
				ch = fgetc(testPos);
				if (ch == '\n')
					if (fgetc(testPos) != EOF) {
						// count only if characters are coming after it
						max_pos++;
						// rewind one character
						fseek(testPos, -1, SEEK_CUR);
					}
			} while (ch != EOF);

			// if such a '\n' is found the number of positions
			// (lines) is one more
			if (max_pos > 0)
				max_pos++;

			// if a character in the file is found and no such '\n' is found then
			// the number of positions is one more than max_pos currently has
			rewind(testPos);
			if ((max_pos == 0) && (fgetc(testPos) != EOF))
				max_pos++;
			fclose(testPos);
		}

		// allocate memory for the times and init it with zero
		measurement[build] = calloc(max_iter * max_pos, sizeof(measurement_t));
		if (measurement[build] == NULL) {
			printf("Error: Could not allocate memory for data.\n");
			goto cleanUp;
		}

		// allocate memory for the list to get the median
		list[build].entry = calloc(max_iter, sizeof(double));
		if (list[build].entry == NULL) {
			printf("Error: Could not allocate memory for list.\n");
			goto cleanUp;
		}

		// allocate memory for the partial call string + " --pos xxxx"
		callStr[build] = malloc(strlen(partCallStr[build]) + POSARGSIZE);
		if (callStr[build] == NULL) {
			printf("Error allocating memory for call string.\n");
			goto cleanUp;
		}
	}

	// open log of comparison to write
	cmpLog = fopen("../Search test/compare.log", "w");
	if (cmpLog == NULL) {
		printf("Error creating compare.log\n");
		goto cleanUp;
	}

	j = 0;
	do {
		// prepare the position argument for the call string
		if (snprintf(posArg, POSARGSIZE, " --pos %d", pos + j) < 0) {
			printf("Error writing to position argument.\n");
			goto cleanUp;
		}

		// create the call strings
		strcpy(callStr[0], partCallStr[0]);
		strcat(callStr[0], posArg);
		strcpy(callStr[1], partCallStr[1]);
		strcat(callStr[1], posArg);

		for (iter = 0; iter < max_iter; iter++) {
			for (build = 0; build <= 1; build++) {

				// call searchtest with params
				printf("Calling \"%s\" (%d/%d)\n", callStr[build], iter + 1, max_iter);
				if (system(callStr[build]) != 0) {
					printf("\"%s\" exited with errors.\n", callStr[build]);
					goto cleanUp;
				}

				// open its search log
				searchLog = fopen("../Search test/searchtest.log", "r");
				if (searchLog == NULL) {
					printf("Error reading searchtest.log\n");
					fprintf(cmpLog, "Error reading searchtest.log\n");
					goto cleanUp;
				}

				// read how many lines are in the search log
				i = 0;
				do {
					ch = fgetc(searchLog);
					if (ch == '\n')
						i++;
				} while (ch != EOF);

				// set the maximal depth reached
				// 7 of the lines do not contain relevant data
				measurement[build][j * max_iter + iter].maxDepth = i - 7;
				if (measurement[build][j * max_iter + iter].maxDepth <= 0) {
					printf("Error: Too few lines in searchtest.log\n");
					fprintf(cmpLog, "Error: Too few lines in searchtest.log\n");
					goto cleanUp;
				}

				// allocate space for the data
				measurement[build][j * max_iter + iter].time = malloc(
						measurement[build][j * max_iter + iter].maxDepth * sizeof(double));
				if (measurement[build][j * max_iter + iter].time == NULL) {
					printf("Error: Could not allocate memory for measurements.\n");
					fprintf(cmpLog, "Error: Could not allocate memory for measurements.\n");
					goto cleanUp;
				}

				rewind(searchLog);

				// go to the first relevant (5th) line
				i = 0;
				while (i < 4) {
					if (fgetc(searchLog) == '\n')
						i++;
				}

				// read in the times
				for (i = 0; i < measurement[build][j * max_iter + iter].maxDepth; i++) {
					if (fscanf(searchLog, " %d | %lf", &depth, &(measurement[build][j * max_iter + iter].time[i]))
							< 2) {
						printf("Error reading data.\n");
						fprintf(cmpLog, "Error reading data.\n");
						goto cleanUp;
					}
					// go to end of line
					while (fgetc(searchLog) != '\n') {
					}
				}
				fclose(searchLog);
				searchLog = NULL;
			}
		}
		j++;
	} while (pos + j <= max_pos);

	// calculate and print the results
	// first run through all positions
	for (j = 0; pos + j <= max_pos; j++) {
		printf("Position %2d\n-----------\n", pos + j);
		fprintf(cmpLog, "Position %2d\n-----------\n", pos + j);
		printf("depth                  Average                                    Median\n");
		fprintf(cmpLog, "depth                  Average                                    Median\n");
		depth = 1;
		do {
			average[0] = 0;
			average[1] = 0;
			list[0].length = 0;
			list[1].length = 0;
			for (iter = 0; iter < max_iter; iter++) {
				// check whether depth was reached by first build
				if (depth <= measurement[0][j * max_iter + iter].maxDepth) {
					// add the time to the average
					average[0] += measurement[0][j * max_iter + iter].time[depth - 1];
					// add the time to the list
					list[0].entry[list[0].length] = measurement[0][j * max_iter + iter].time[depth - 1];
					list[0].length++;
				}
				if (depth <= measurement[1][j * max_iter + iter].maxDepth) {
					average[1] += measurement[1][j * max_iter + iter].time[depth - 1];
					// add the time to the list
					list[1].entry[list[1].length] = measurement[1][j * max_iter + iter].time[depth - 1];
					list[1].length++;
				}
			}
			// if any build reached depth then one of the lists
			// has positive length and we have to print something
			if ((list[0].length > 0) || (list[1].length > 0)) {
				// print the depth
				printf(" %2d     ", depth);
				fprintf(cmpLog, " %2d     ", depth);

				// if first list has entries the first build reached the depth
				if (list[0].length > 0) {
					// calculate average
					average[0] /= list[0].length;
					// calculate median
					median[0] = getMedian(&(list[0]));
					// print depth and first average
					printf("%lfs", average[0]);
					fprintf(cmpLog, "%lfs", average[0]);

					// check whether the first build did not reach that depth in all iterations
					// and print in that case in how many iterations it reached depth
					printIterInfo(cmpLog, &(list[0]), max_iter);

					// check whether the second build reached that depth
					if (list[1].length > 0) {
						average[1] /= list[1].length;
						median[1] = getMedian(&(list[1]));

						// print relation
						printRel(cmpLog, average[0], average[1]);

						printf("%lfs", average[1]);
						fprintf(cmpLog, "%lfs", average[1]);
						printIterInfo(cmpLog, &(list[1]), max_iter);

						// now print the medians
						printf("     %lfs", median[0]);
						fprintf(cmpLog, "     %lfs", median[0]);
						printIterInfo(cmpLog, &(list[0]), max_iter);

						printRel(cmpLog, median[0], median[1]);

						printf("%lfs", median[1]);
						fprintf(cmpLog, "%lfs", median[1]);
						printIterInfo(cmpLog, &(list[1]), max_iter);
					}

					// second build did not reach depth in any iteration
					// so no average or median is defined
					else {
						printf(" < n.a.                  %lfs", getMedian(&(list[0])));
						fprintf(cmpLog, " < n.a.                  %lfs", getMedian(&(list[0])));
						printIterInfo(cmpLog, &(list[0]), max_iter);
						printf(" < n.a.");
						fprintf(cmpLog, " < n.a.");
					}
				}
				// if list[0].length == 0 then the first build did not reach
				// depth in any iteration so there is no average or median defined
				else {
					// list[1].length is positive so we can safely devide by it
					average[1] /= list[1].length;
					median[1] = getMedian(&(list[1]));

					printf("n.a. > %lfs", average[1]);
					fprintf(cmpLog, "n.a. > %lfs", average[1]);
					printIterInfo(cmpLog, &(list[1]), max_iter);

					// now print the median
					printf("     n.a. > %lfs", median[1]);
					fprintf(cmpLog, "     n.a. > %lfs", median[1]);
					printIterInfo(cmpLog, &(list[1]), max_iter);
				}
				printf("\n");
				fprintf(cmpLog, "\n");
			}
			depth++;
		} while ((list[0].length > 0) || (list[1].length > 0));
		printf("\n");
		fprintf(cmpLog, "\n");
	}

	printf("\nRaw data:\n");
	fprintf(cmpLog, "\nRaw data:\n");
	for (build = 0; build <= 1; build++) {
		printf("build = %d\n", build);
		fprintf(cmpLog, "build = %d\n", build);
		for (j = 0; pos + j <= max_pos; j++) {
			printf("\tposition = %d\n", pos + j);
			fprintf(cmpLog, "\tposition = %d\n", pos + j);
			for (iter = 0; iter < max_iter; iter++) {
				printf("\t\titer = %d: ", iter);
				fprintf(cmpLog, "\t\titer = %d: ", iter);
				for (i = 0; i < measurement[build][j * max_iter + iter].maxDepth; i++) {
					printf("%lf ", measurement[build][j * max_iter + iter].time[i]);
					fprintf(cmpLog, "%lf ", measurement[build][j * max_iter + iter].time[i]);
				}
				printf("\n");
				fprintf(cmpLog, "\n");
			}
			printf("\n");
			fprintf(cmpLog, "\n");
		}
	}

	// everything went smoothly until here
	ret = EXIT_SUCCESS;

	cleanUp:

	if (searchLog != NULL)
		fclose(searchLog);
	if (cmpLog != NULL)
		fclose(cmpLog);
	for (build = 0; build <= 1; build++) {
		if (measurement[build] != NULL) {
			for (j = 0; pos + j <= max_pos; j++) {
				for (iter = 0; iter < max_iter; iter++) {
					if (measurement[build][j * max_iter + iter].time != NULL)
						free(measurement[build][j * max_iter + iter].time);
				}
			}
			free(measurement[build]);
		}
		if (callStr[build] != NULL)
			free(callStr[build]);
		if (partCallStr[build] != NULL)
			free(partCallStr[build]);
		if (list[build].entry != NULL)
			free(list[build].entry);
	}
	return ret;
}
