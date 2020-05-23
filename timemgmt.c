/*
 * timeman.c
 *
 *  Created on: May 7, 2020
 *      Author: sander
 */

#include <time.h>
#include <math.h>
#include "timemgmt.h"

void timemgmt(timeInfo_t *timeInfo) {

	struct timespec thinkTime;
	long t;	// Time in milliseconds

	if (timeInfo->moveTime > -1)
		t = timeInfo->moveTime;
	else
		t = timeInfo->timeLeft[timeInfo->toMove] / 20;

	thinkTime.tv_sec = t / 1000;
	thinkTime.tv_nsec = (t % 1000) * 1000000;

//	thinkTime.tv_sec = 3;
//	thinkTime.tv_nsec = 500000000;
	//printf("thinktime = %d sec + %ld nsec\n", thinkTime.tv_sec, thinkTime.tv_nsec);

			nanosleep(&thinkTime, NULL);
	go = 0;
}
