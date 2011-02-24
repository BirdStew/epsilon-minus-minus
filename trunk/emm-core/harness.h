/*
 * harness.h
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */

#ifndef HARNESS_H_
#define HARNESS_H_

#include "matrix.h"
#include "code.h"

typedef struct CodeStats
{
	int rows;
	int cols;
	char* data;
} CodeStats;

void testCode(int wordLen, int parityLen, int parityType, double errorProbability, CodeStats* stats);
void transmit(Matrix* packet, double errorProbability);

#endif /* HARNESS_H_ */
