/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <stdlib.h>
#include "code.h"
#include <math.h>	// used for pow()
#include <time.h>	// Used for Rand()


/*
 * Creates a new code struct and fills all fields with the necessary matrices.
 * This function receives the length of original word (bits) and the number of
 * parity bits used.  The third argument specifies which method of parity check
 * equations to use when making the Generator and Control matrices.
 */

Code* newCode(int wordLen, int parityLen, int parityType)
{
	Matrix* tempPCM;

	switch(parityType)
	{
		case DENSE:
			tempPCM = newDenseParity(wordLen, parityLen);
			break;
		case LOW_DENSITY:
			tempPCM = newLowDensityParity(wordLen, parityLen);
			break;
		case RANDOM:
			tempPCM = newRandomParity(wordLen, parityLen);
			break;
		default:
			fprintf(stderr, "error: Invalid parity type '%d' in 'newCode'\n", parityType);
			exit(EXIT_FAILURE);
	}

	Code* c = (Code*)malloc(sizeof(Code));
	c->wordLen = wordLen;
	c->parityLen = parityLen;
	c->generator = newGeneratorMatrix(tempPCM);
	c->control =  newControlMatrix(tempPCM);
	c->syndrome = newSyndromeMatrix(wordLen, parityLen);

	delMatrix(&tempPCM);

	return c;
}

//FIXME
void delCode(Code** c)
{
	delMatrix(&c->generator);
	delMatrix(&c->control);
	delMatrix(&c->syndrome);
}

/*
 * Creates and returns a new generator matrix.  A generator matrix is the
 * result of joining an identity matrix on the left with a “parity check matrix”
 * on the right.
 */

Matrix* newGeneratorMatrix(Matrix* pcm)
{
	Matrix* identity = makeIdentity(pcm->rows);
	Matrix* gen = joinMatrix(identity, pcm);
	delMatrix(&identity);
	return gen;
}


/*
 * Creates and returns a control matrix.  A control matrix is
 * a transposed “parity check matrix” joined with an identity
 * matrix on the right.
 */

Matrix* newControlMatrix(Matrix* pcm)
{
	transposeMatrix(pcm);
	Matrix* identity = makeIdentity(pcm->rows);
	Matrix* con = joinMatrix(pcm,identity);
	delMatrix(&identity);
	return con;
}


/*
 * Creates a syndrome table represented as a matrix for linear decoding.
 * The syndrome table will always be dimensions (2 ^ partiy length)
 * by (word length + parity length).
 */

Matrix* newSyndromeMatrix(int wordLen, int parityLen)
{
	Matrix* syn = newMatrix((int)pow(2,parityLen), wordLen + parityLen);
	fprintf(stderr, "error: newSyndromeMatrix Unimplemented!");
	exit(1);//FIXME
	return syn;
}


/*
 * Creates a matrix of dimension rows, cols and fills it with ones.
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
			if(i != j)
			{
				d->data[i * cols + j] = 1;
			}
		}
	}
	return d;
}


/*
 * Creates a matrix of dimension rows, cols and fills it zeros and ones.
 * The values in this case are dictated by the Low Density method of creating
 * parity check equations.  The values in the matrix are entirely dependent on
 * the dimensions of the matrix.
 */

Matrix* newLowDensityParity(int rows, int cols)
{
	//FIXME
	return 0;
}


/*
 * Creates and returns matrix of random zeros and ones of
 * dimension rows, cols. This is one of the methods of creating
 * a parity check matrix.
 */

Matrix* newRandomParity(int rows, int cols)
{
		Matrix* m = newMatrix(rows, cols);
		int i;
		int n = rows * cols;

		srand(time(NULL));
		for(i = 0; i < n; i++)
		{
			m->data[i] = rand() % 2;
		}
		return m;
}
