/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */
#include "code.h"


matrix* newGeneratorMatrix(int n, int k)
{
	matrix* gen = newMatrix(n, k);
	setIdentity(gen);

	printAugMatrix(gen, 3);

	return 0;
}


void setPCE(matrix *m, int n)
{
 // j should always start at offset
 // offset = k-n
}
