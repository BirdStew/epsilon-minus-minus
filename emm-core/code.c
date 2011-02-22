/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */
#include "code.h"
#include <stdio.h> //FIXME


Matrix* newGeneratorMatrix(Matrix* pcm)
{
	Matrix* identity = makeIdentity(pcm->rows);
	Matrix* gen = joinMatrix(identity, pcm);
	delMatrix(&identity);
	return gen;
}


/*
 * Creates a matrix of dimension  rows, cols and fills it with ones.
 * Starting from the upper left, the diagonal is set to zero's.  Although
 * this function creates an inverted identity, it does not need to be a square
 * matrix.  The result is intended to be used as a parity check matrix.
 */

Matrix* newDenseParity(int rows, int cols)
{
	Matrix* d = newMatrix(rows, cols);

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



Matrix* newControlMatrix(Matrix* pcm)
{
	transposeMatrix(pcm);
	Matrix* identity = makeIdentity(pcm->rows);
	Matrix* con = joinMatrix(pcm,identity);
	delMatrix(&identity);
	return con;
}