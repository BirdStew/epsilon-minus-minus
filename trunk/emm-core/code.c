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

	printMatrix(gen);

	return 0;
}
