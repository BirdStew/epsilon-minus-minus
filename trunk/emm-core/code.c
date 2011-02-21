/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */
#include "code.h"
#include <stdio.h> //FIXME

matrix* newGeneratorMatrix(matrix* pce)
{
	matrix* identity = makeIdentity(pce->rows);
	matrix* gen = joinMatrix(identity, pce);
	delMatrix(&identity);
	return gen;
}


matrix* setPCM(int method)
{

	return 0;
}


/*
 * Creates a matrix of dimension  rows, cols and fills it with ones.
 * Starting from the upper left, the diagonal is set to zero's.  Although
 * this function creates an inverted identity, it does not need to be a square
 * matrix.  The result is intended to be used as a parity check matrix.
 */

matrix* newDenseParity(int rows, int cols)
{
	matrix* d = newMatrix(rows, cols);

	int i, j;
	for(i = 0; i < rows; i++)
	{
		for(j = 0; j < cols; j++)
		{
			if(i == j)
				d->data[i * cols + j] = 0;
			else
				d->data[i * cols + j] = 1;
		}
	}
	return d;
}
