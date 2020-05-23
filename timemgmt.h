/*
 * timeman.h
 *
 *  Created on: May 7, 2020
 *      Author: sander
 */

#ifndef TIMEMGMT_H_
#define TIMEMGMT_H_

typedef struct timeInfo_t {
	long timeLeft[2];
	long inc[2];
	long moveTime;
	int toMove;
} timeInfo_t;

extern int go;

void timemgmt (timeInfo_t* timeInfo);
#endif /* TIMEMGMT_H_ */
