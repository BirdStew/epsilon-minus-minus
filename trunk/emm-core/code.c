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

	//Matrix* validWords = calcValidWords(c->generator);
	//c->distance = calcMinDistance(validWords);
	c->syndrome = newSyndromeMatrix(NULL);

	delMatrix(&tempPCM);
	//delMatrix(&validWords);
	return c;
}


/*
 * Receives a pointer to a code pointer and safely frees the code
 * struct from the heap. The passed pointer is then dereferenced and
 * set to null to prevent users from reading unmanaged memory.
 */

void delCode(Code** c)
{
	if(*c == NULL || c == NULL)
	{
		fprintf(stderr,"error: passed ptr is NULL in 'delCode'\n");
	}
	else
	{
		delMatrix(&(*c)->generator);
		delMatrix(&(*c)->control);
		delMatrix(&(*c)->syndrome);
		free(*c);
		*c = NULL;
	}
}

/*
 * Creates and returns a new generator matrix.  A generator matrix is the
 * result of joining an identity matrix on the left with a “parity check matrix”
 * on the right.
 */

Matrix* newGeneratorMatrix(Matrix* pcm)
{
	if(pcm == NULL)
	{
		fprintf(stderr, "error: pcm is null in 'newGeneratorMatrix'\n");
	}

	Matrix* identity = newIdentity(pcm->rows);
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
	Matrix* identity = newIdentity(pcm->rows);
	Matrix* con = joinMatrix(pcm,identity);
	delMatrix(&identity);
	return con;
}


/*
 * Creates a syndrome table represented as a matrix for linear decoding.
 * The syndrome table will always be dimensions (2 ^ partiy length)
 * by (word length + parity length).
 */

Matrix* newSyndromeMatrix(Matrix* validWords)
{
	//Matrix* syn = newMatrix((int)pow(2,parityLen), wordLen + parityLen);
	Matrix* syn = newMatrix(2,2);
	//fprintf(stderr, "error: newSyndromeMatrix Unimplemented!\n"); //FIXME
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
	Matrix* m = newMatrix(rows, cols);
	//fprintf(stderr, "error: newLowDensityParity is Unimplemented\n"); //FIXME
	return m;
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


void encode(Matrix* packet, Matrix* encodedPacket, Code* c)
{
	bufferedBinaryMultiply(packet, c->generator, encodedPacket);
}


int calcMinDistance(Matrix* validWords)
{
	int minDist = 0xFFFFFFFF;
	int diffBits;
	int i, j, k;

	for(i = 0; i < (validWords->rows)-1; i++)
	{
		for(j = i + 1; j < validWords->rows; j++)
		{
			diffBits = 0;
			for(k = 0; k <  validWords->cols; k++)
			{
				if(validWords->data[i * validWords->cols + k] != validWords->data[j * validWords->cols + k])
				{
					diffBits++;
				}
			}

			if(minDist > diffBits)
			{
				minDist = diffBits;
			}
		}
	}
	return minDist;
}


/*
 * Spawns validWords stored as a matrix. Using the generator matrix
 * it loops over all the values from 0 to word length -1. The validWords
 * matrix will allays be 2^word length by encoded length.
 * wordBuffer 1 X wordLen * Generator wordLen X (wordLen + parityLen)
 */

Matrix* calcValidWords(Matrix* generator)
{
	Matrix* validWords = newMatrix(pow(2,generator->rows),generator->cols);
	Matrix* wordBuffer = newMatrix(1, generator->rows);

	Matrix r;
	Matrix* result = &r;
	result->rows = 1;
	result->cols = validWords->cols;


	int i, j, tempShift, temp;
	int mask = 1;

	for(i = 0; i < validWords->rows; i++)
	{
		/* Break i into bits and fill buffer */
		tempShift = i;
		for(j = wordBuffer->cols; j >= 0 ; j--)
		{
			temp = tempShift & mask;
			wordBuffer->data[j] = (char)temp;
			tempShift >>= 1;
		}
		printf("\nBuffer %d\n", i);
		printMatrix(wordBuffer);

		/* Set result pointer to row in validWords matrix */
		result->data = (char*)(validWords + (i * validWords->cols));

		/* Multiply word through generator matrix and store in validWords */
		//bufferedBinaryMultiply(wordBuffer, generator, result);

		printf("\nResult %d\n", i);
		printMatrix(result);
	}

	delMatrix(&wordBuffer);
	return validWords;
}

