/*
 * timeman.c
 *
 *  Created on: May 7, 2020
 *      Author: sander
 */

#include <time.h>
#include <math.h>
#include "timemgmt.h"

void* timemgmt(void *_timeInfo) { // argument is actually a timeInfo_t* but pthread_create() needs a function pointer to a function taking a void* argument

	struct timespec thinkTime;
	long t;	// time in milliseconds
	timeInfo_t *timeInfo = (timeInfo_t*) _timeInfo;

	if (timeInfo->moveTime > -1)
		t = timeInfo->moveTime;
	else
		t = timeInfo->timeLeft[timeInfo->toMove] / 20;

	thinkTime.tv_sec = t / 1000;
	thinkTime.tv_nsec = (t % 1000) * 1000000;

	nanosleep(&thinkTime, NULL);
	go = 0;
	return NULL;
}
