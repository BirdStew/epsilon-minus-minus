/*
 * harness.c
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */
#include <stdlib.h>
#include "harness.h"


void testCode(int wordLen, int parityLen, int parityType, double errorProbability, CodeStats* stats)
{

}

void transmit(Matrix* packet, double errorProbability)
{
	int i;
	int n = packet->rows + packet->cols;
	for(i = 0; i < n; i++)
	{
		if(rand() % 1 <= errorProbability)
		{
			packet->data[i] ^= 1;
		}
	}
}
