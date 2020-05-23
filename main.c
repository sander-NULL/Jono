/*
 * ucicomm.c
 *
 *  Created on: Mar 30, 2020
 *      Author: sander
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "bitboards.h"
#include "movgen.h"
#include "alphabeta.h"
#include "timemgmt.h"

int go;
pv prinvar;

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

int makemove(chessposition *position, char *move) { /* Makes a move given in long algebraic notation, does not check for legality */

	int file, rank, startsqi, endsqi;
	int player, playeropp;
	bitboard start, end;

	switch (move[0]) {
	case 'a':
		file = 0;
		break;
	case 'b':
		file = 1;
		break;
	case 'c':
		file = 2;
		break;
	case 'd':
		file = 3;
		break;
	case 'e':
		file = 4;
		break;
	case 'f':
		file = 5;
		break;
	case 'g':
		file = 6;
		break;
	case 'h':
		file = 7;
		break;
	default:
		return EXIT_FAILURE; /* Unknown character */
	}

	switch (move[1]) {
	case '1':
		rank = 0;
		break;
	case '2':
		rank = 1;
		break;
	case '3':
		rank = 2;
		break;
	case '4':
		rank = 3;
		break;
	case '5':
		rank = 4;
		break;
	case '6':
		rank = 5;
		break;
	case '7':
		rank = 6;
		break;
	case '8':
		rank = 7;
		break;
	default:
		return EXIT_FAILURE; /* Unknown character */
	}
	startsqi = 8 * rank + file;

	switch (move[2]) {
	case 'a':
		file = 0;
		break;
	case 'b':
		file = 1;
		break;
	case 'c':
		file = 2;
		break;
	case 'd':
		file = 3;
		break;
	case 'e':
		file = 4;
		break;
	case 'f':
		file = 5;
		break;
	case 'g':
		file = 6;
		break;
	case 'h':
		file = 7;
		break;
	default:
		return EXIT_FAILURE; /* Unknown character */
	}

	switch (move[3]) {
	case '1':
		rank = 0;
		break;
	case '2':
		rank = 1;
		break;
	case '3':
		rank = 2;
		break;
	case '4':
		rank = 3;
		break;
	case '5':
		rank = 4;
		break;
	case '6':
		rank = 5;
		break;
	case '7':
		rank = 6;
		break;
	case '8':
		rank = 7;
		break;
	default:
		return EXIT_FAILURE; /* Unknown character */
	}
	endsqi = 8 * rank + file;

	start = UINT64_C(1) << startsqi;
	end = UINT64_C(1) << endsqi;
	player = position->states.tomove;
	playeropp = (player + 1) % 2; /* The one not to move */

	/* First we update the states for the next move */
	position->states.tomove = playeropp; /* Other player is to move afterwards */
	if (start == A1 || end == A1)
		position->states.queencastle[WHITE] = 0; /* Something happened with the rooks */
	if (start == H1 || end == H1)
		position->states.kingcastle[WHITE] = 0; /* and we have to adjust castling rights */
	if (start == A8 || end == A8)
		position->states.queencastle[BLACK] = 0;
	if (start == H8 || end == H8)
		position->states.kingcastle[BLACK] = 0;
	if (start == E1)
		position->states.kingcastle[WHITE] = 0; /* King must have moved, castling no longer possible */
	if (start == E1)
		position->states.queencastle[WHITE] = 0;
	if (start == E8)
		position->states.kingcastle[BLACK] = 0;
	if (start == E8)
		position->states.kingcastle[BLACK] = 0;

	/* If pawn jumped, en passant is possible */
	if ((start & position->pawns[player]) != 0 && abs(endsqi - startsqi) == 16) /* Has a pawn moved AND did it jump? */
		position->states.enpassant = startsqi % 8;
	else
		position->states.enpassant = -1;

	if ((start & position->pawns[player]) != 0
			|| (end & getpieces(playeropp, position)) != 0) /* Has a pawn moved OR was a piece captured? */
		position->states.plies = 0;
	else
		position->states.plies++;

	if ((position->king[player] & start) != 0) { /* King has moved */
		CB(position->king[player], startsqi + 1); /* Remove king from start square */
		SB(position->king[player], endsqi + 1); /* Put king on end square */
		if (endsqi - startsqi == -2) { /* Did king castle queen side? */
			CB(position->rooks[player], endsqi - 1); /* Remove rook from second square to the left of end square */
			SB(position->rooks[player], endsqi + 2); /* Put it one square to the right */
		}
		if (endsqi - startsqi == 2) { /* Did king castle king side? */
			CB(position->rooks[player], endsqi + 2); /* Remove rook from first square to the right of end square */
			SB(position->rooks[player], endsqi); /* Put it one square to the left */
		}
	}

	else if ((position->queens[player] & start) != 0) { /* A queen has moved */
		CB(position->queens[player], startsqi + 1); /* Remove queen from start square */
		SB(position->queens[player], endsqi + 1); /* Put queen on end square */
	}

	else if ((position->rooks[player] & start) != 0) { /* A Rook has moved */
		CB(position->rooks[player], startsqi + 1); /* Remove rook from start square */
		SB(position->rooks[player], endsqi + 1); /* Put rook on end square */
	}

	else if ((position->bishops[player] & start) != 0) { /* A bishop has moved */
		CB(position->bishops[player], startsqi + 1); /* Remove bishop from start square */
		SB(position->bishops[player], endsqi + 1); /* Put bishop on end square */
	}

	else if ((position->knights[player] & start) != 0) { /* A knight has moved */
		CB(position->knights[player], startsqi + 1); /* Remove knight from start square */
		SB(position->knights[player], endsqi + 1); /* Put knight on end square */
	}

	else if ((position->pawns[player] & start) != 0) { /* A pawn has moved */
		CB(position->pawns[player], startsqi + 1); /* Remove pawn from start square */
		if ((end & CHESSRANK(player, 7)) != 0) { /* Did the pawn promote? */
			switch (move[4]) {
			case 'q':
				SB(position->queens[player], endsqi + 1); /* Put queen on end square */
				break;
			case 'r':
				SB(position->rooks[player], endsqi + 1); /* Put rook on end square */
				break;
			case 'b':
				SB(position->bishops[player], endsqi + 1); /* Put bishop on end square */
				break;
			case 'n':
				SB(position->knights[player], endsqi + 1); /* Put knight on end square */
				break;
			default:
				return EXIT_FAILURE; /* Pawn promotion error */
			}
		} else
			SB(position->pawns[player], endsqi + 1); /* Put pawn on end square */
		/* If pawn left its file, it captured a piece. If also there was no piece of opponent, it must have been en passant capture */
		if (move[0] != move[2] && (getpieces(playeropp, position) & end) == 0) {
			position->pawns[playeropp] &= ~(CHESSRANK(playeropp, 3)
					& CHESSFILE(endsqi % 8)); /* Remove pawn from fourth rank (seen from playeropp) */
		}
	}
	position->queens[playeropp] &= ~end; /* Capture a piece if there was one */
	position->rooks[playeropp] &= ~end;
	position->bishops[playeropp] &= ~end;
	position->knights[playeropp] &= ~end;
	position->pawns[playeropp] &= ~end;
	initattsets(position);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
	int i;
	char fen[100];				// Will contain FEN string
	char rawCommand[5000];		// Will contain on-parsed raw command sent from the GUI
	char *tmp;					// A temporary variable for parsing
	char *command[5000]; 		// Pointers to parsed commands, possible are 5000 commands
	FILE *log;					// For the log file
	chessposition position;
	pthread_t thSearch, thTime;	// For the search thread and the timing thread
	timeInfo_t timeInfo;		// Struct storing chess specific times

	// Open a log file for debugging reasons
	log = fopen("JONOLOG.log", "w");
	if (log == NULL)
		return EXIT_FAILURE;

	while (1) {
		if (fgets(rawCommand, 5000, stdin) != NULL) {

			fputs("GUI: ", log);
			fputs(rawCommand, log); /* Logging the commands */
			fflush(log);

			tmp = rawCommand; /* For parsing */
			for (i = 0; i <= 5000; i++) {
				if (rawCommand[i] == '\n')
					rawCommand[i] = '\0'; /* For parsing reasons */
			}
			for (i = 0; i < 500; i++)
				command[i] = strsep(&tmp, " "); /* Parse the rawCommand, commands are seperated by " " and end with newline \n */

			if (strcmp(command[0], "uci") == 0) {
				printf("id name Jono\n");
				fputs("\t -> id name Jono\n", log);
				printf("id author Sander\n");
				fputs("\t -> id author Sander\n", log);
				printf("uciok\n");
				fputs("\t -> uciok\n", log);
				fflush(stdout);
				fflush(log);
				if (init() == EXIT_FAILURE) {
					fputs("\t init() failed. Exiting.\n", log);
					fflush(stdout);
					return EXIT_FAILURE; /* Initialize the engine */
				}
			}

			else if (strcmp(command[0], "isready") == 0) {
				printf("readyok\n");
				fputs("\t -> readyok\n", log);
				fflush(stdout);
				fflush(log);
			}

			else if (strcmp(command[0], "ucinewgame") == 0) {
			}

			else if (strcmp(command[0], "position") == 0) {
				if (strcmp(command[1], "startpos") == 0) {
					if (importpos(startfen, &position) == 0) {
						fputs("   Set up start position.\n", log);
						fflush(log);
					}
					i = 2; /* Is the next command a "moves"? */
				} else if (strcmp(command[1], "fen") == 0) {
					fen[0] = '\0'; /* Empty the FEN string first */
					strcat(fen, command[2]); /* Concatenate the fen string */
					strcat(fen, " ");
					strcat(fen, command[3]);
					strcat(fen, " ");
					strcat(fen, command[4]);
					strcat(fen, " ");
					strcat(fen, command[5]);
					strcat(fen, " ");
					strcat(fen, command[6]);
					strcat(fen, " ");
					strcat(fen, command[7]);
					if (importpos(fen, &position) == 0)
						fputs("   Set up FEN position.\n", log);
					i = 8; /* Is the next command a "moves"? */
					fflush(log);
				}
				if (command[i] != NULL) {
					if (strcmp(command[i], "moves") == 0) { /* Is the next command a "moves"? */
						fputs("   Playing moves...\n", log);
						fflush(log);
						i++;
						while (command[i] != NULL) {
							makemove(&position, command[i]);
							i++;
						}
					}
				}
			}

			else if (strcmp(command[0], "go") == 0) {
				go = 1;
				i = 1;
				timeInfo.moveTime = -1;
				timeInfo.toMove = position.states.tomove;
				while (command[i] != NULL) {
					if (strcmp(command[i], "wtime") == 0)
						timeInfo.timeLeft[0] = strtol(
								(const char*) (command[i + 1]), NULL, 10);
					else if (strcmp(command[i], "winc") == 0)
						timeInfo.inc[0] = strtol((const char*) (command[i + 1]),
						NULL, 10);
					else if (strcmp(command[i], "btime") == 0)
						timeInfo.timeLeft[1] = strtol(
								(const char*) (command[i + 1]), NULL, 10);
					else if (strcmp(command[i], "binc") == 0)
						timeInfo.inc[1] = strtol((const char*) (command[i + 1]),
						NULL, 10);
					else if (strcmp(command[i], "movetime") == 0)
						timeInfo.moveTime = strtol(
								(const char*) (command[i + 1]), NULL, 10);
					i++;
				}

				// Creating the thread for the timing
				if (pthread_create(&thTime, NULL, (void*) &timemgmt,
						&timeInfo)) {
					printf("ERROR, could not create time thread!\n");
					return EXIT_FAILURE;
				}
				// Creating the thread for the search
				if (pthread_create(&thSearch, NULL, (void*) &search,
						&position)) {
					printf("ERROR, could not create search thread!\n");
					return EXIT_FAILURE;
				}
				pthread_join(thTime, NULL);
				pthread_join(thSearch, NULL);
				fflush(stdout);
				fflush(log);
			}

			else if (strcmp(command[0], "quit") == 0)
				break;
		}
	}
	fclose(log);
	destroymagictbls();
	return EXIT_SUCCESS;
}
