/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <stdlib.h>
#include "code.h"
#include <math.h>		// used for pow()
#include <time.h>		// Used for Rand()


/*
 * Creates a new code struct and fills all fields with the necessary matrices.
 * This function receives the length of original word (bits) and the number of
 * parity bits used. The third argument specifies which method of parity check
 * equations to use when making the Generator and Control matrices. The forth argument
 * is only used when parityType is set to CUSTOM.
 */

Code* newCode(int wordLen, int parityLen, int parityType, char* pcmPath)
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
		case CUSTOM:
			tempPCM = readMatrix(pcmPath);
			break;
		default:
			fprintf(stderr, "error: Invalid parity type '%d' in 'newCode'\n", parityType);
			exit(EXIT_FAILURE);
	}

	Code* c = (Code*)malloc(sizeof(Code));
	c->wordLen = tempPCM->rows;
	c->parityLen = tempPCM->cols;
	c->parityType = parityType;
	c->generator = newGeneratorMatrix(tempPCM);
	c->control =  newControlMatrix(tempPCM);

	Matrix* validWords = calcValidWords(c->generator);
	c->distance = calcMinDistance(validWords);
	delMatrix(&validWords);

	c->syndrome = newSyndromeMatrix(c->control);

	delMatrix(&tempPCM);

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
 * Additional Notes:
 * Con->rows = parityLen
 * Con->cols = wordLen + parityLen
 * wordLen = Con->cols - parityLen
 */

Matrix* newSyndromeMatrix(Matrix* control)
{
	Matrix* syn = newMatrix(pow(2,control->rows), control->cols); //?  are these even the right dimensions?!!!
	Matrix* allWords = wordsByWeight(control->cols);
	Matrix* temp = newMatrix(control->rows, 1);

	/* Pseudo vector - Transposed(row in allWords) */
	Matrix w;
	w.rows = allWords->cols;
	w.cols = 1;
	Matrix* word = &w;

	/* Pseudo vector - row in syndrome */
	Matrix r;
	r.rows = 1;
	r.cols = syn->cols;
	Matrix* result = &r;

	int i, j;
	unsigned int index;
	int insertions = 0;
	for(i = 1; i < allWords->rows && insertions < syn->rows; i++)
	{
		/* Set word pointer to row in words table */
		word->data = (allWords->data + i*allWords->cols);

		bufferedBinaryMultiply(control, word, temp);

		/* Resolve vector to int */
		index = vectorAsInt(temp);

		/* Set result pointer to row in syndrome table */
		result->data = (char*)(syn->data + (index * syn->cols));

		/* Copy into syndrome table if row does not exist and is not the first row */
		if(vectorAsInt(result) == 0 && index != 0)
		{
			for(j = 0; j < result->cols; j++)
			{
				result->data[j] = word->data[j];
			}
			insertions++;
		}
	}

	delMatrix(&allWords);
	delMatrix(&temp);

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
	int i,j;
	Matrix *temp = newMatrix(rows,cols);
	Matrix *valid = wordsByWeight(rows);
	for(i = 0; i < cols; i++)
	{
		for(j = 0; j < rows; j++)
		{
			temp->data[j*cols+i] = valid->data[(valid->rows-1-i)*rows+j];
		}
	}
	return temp;
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
		for(j = wordBuffer->cols-1; j >= 0 ; j--)
		{
			temp = tempShift & mask;
			wordBuffer->data[j] = (char)temp;
			tempShift >>= 1;
		}

		/* Set result pointer to row in validWords matrix */
		result->data = (char*)(validWords->data + (i * validWords->cols));

		/* Multiply word through generator matrix and store in validWords */
		bufferedBinaryMultiply(wordBuffer, generator, result);
	}

	delMatrix(&wordBuffer);
	return validWords;
}


/*
 * Generates a large table (matrix) of dimensions 2^N x N.
 * This is one of the most costly memory functions in emm-core.
 */

Matrix* wordsByWeight(int wordLen){
	unsigned int iter;
	unsigned int i,j,k;
	unsigned int *weight;
	unsigned int mask;
	unsigned int bits;
	Matrix *allWords = newMatrix(pow(2,wordLen),wordLen);

	weight=(unsigned int*)malloc((allWords->rows)*sizeof(unsigned int));

	for(iter = 0; iter < allWords->rows; iter++)
	{
		bits = 0;
		for(mask = 0x01; mask <= allWords->rows; mask<<=1)
		{
			if(mask & iter)
			{
				bits++;
			}
		}
		weight[iter]=bits;
	}

	i = 0;
	for(j = 0; j<=wordLen; j++)
	{
		for(iter = 0; iter < allWords->rows; iter++)
		{
			if(j == weight[iter])
			{
				for(k = 0; k < (allWords->cols); k++)
				{
					allWords->data[(i*(allWords->cols))+k] = ((0x01<<k) & iter)? 1 : 0;
				}
				i++;
			}
		}
	}

	return allWords;
}


/*
 * Function to calculate the minimum distance between valid words. This
 * function requires that validWords be pre-computed. The calculation
 * is very expensive because it does N X N comparison of rows that are
 * length M. The return value is single integer that represents the fewest
 * number of bit differences between the comparisons.
 */

int calcMinDistance(Matrix* validWords)
{
	unsigned int minDist = 0xFFFFFFFF;
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
 * Function to produce an encoded packet. A prerequisite to this function is a code
 * struct where the generator matrix has already been computed. Packet must be a
 * 1 x W matrix and the resulting encodedPacket should be a 1 x (W+P) matrix. W
 * represents word length while represents parity length.
 */

void encode(Matrix* packet, Matrix* encodedPacket, Code* c)
{
	bufferedBinaryMultiply(packet, c->generator, encodedPacket);
}


/*
 * Function to produce a decoded packet. A prerequisite to this function is a
 * code struct where the generator matrix and syndrome table have already been
 * computed. Due to nature of matrix multiplication, both input and output buffers
 * (matrices) must be the correct dimensions.  ReceivedPacket must be a (W+P) x 1
 * matrix. SyndromeIndexBuffer must be P x 1. DecodedPacket must be a W x 1 matrix.
 * W represents word length while represents parity length.
 */

void decode(Matrix* receivedPacket, Matrix* syndromeIndexBuffer, Matrix* decodedPacket, Code* c)
{
	/* Pseudo vector - row in syndrome table */
	Matrix co;
	co.rows = 1;
	co.cols = c->wordLen + c->parityLen;
	Matrix* cosetLeader = &co;

	/* Store resulting syndrome in encodedBuffer */
	bufferedBinaryMultiply(c->control, receivedPacket, syndromeIndexBuffer);

	int i;
	int index = vectorAsInt(syndromeIndexBuffer);
	if( index == 0) /* Valid word */
	{

		/* write bits into decodedPacket (No Parity bits)
		 * Note: decodePacket is a vertical vector.  Therefore element count equals the rows
		 */
		for(i = 0; i <  decodedPacket->rows; i++)
		{
			decodedPacket->data[i] = receivedPacket->data[i];
		}
	}
	else
	{
		cosetLeader->data = c->syndrome->data + index * c->syndrome->cols;

		for(i = 0; i < decodedPacket->rows; i++)
		{
			decodedPacket->data[i] = cosetLeader->data[i] ^ receivedPacket->data[i];
		}
	}
}

