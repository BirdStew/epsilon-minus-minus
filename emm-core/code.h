/*
 * code.h
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */

#ifndef CODE_H_
#define CODE_H_

#include "matrix.h"

typedef struct Code
{
	int wordLen;
	int parityLen;
	int distance;
	int parityType;
	Matrix* generator;
	Matrix* control;
	Matrix* syndrome;
} Code;

#define PARITY_FLAG_MAX 7 /* Should always be 2^len(PARITY_TYPE) */
enum PARITY_TYPE
{
	DENSE = 1,
	LOW_DENSITY = 2,
	RANDOM = 4
};

Code* newCode(int wordLen, int parityLen, int parityType);
void delCode(Code** c);
Matrix* newGeneratorMatrix(Matrix* pcm);
Matrix* newControlMatrix(Matrix* pcm);
Matrix* newSyndromeMatrix(Matrix* validWords);
Matrix* newDenseParity(int rows, int cols);
Matrix* newLowDensityParity(int rows, int cols);
Matrix* newRandomParity(int rows, int cols);
Matrix* calcValidWords(Matrix* generator);
int calcMinDistance(Matrix* validWords);
void encode(Matrix* packet, Matrix* encodedPacket, Code* c);
void decode(Matrix* encodedPacket, Matrix* decodedPacket, Code* c);



#endif /* CODE_H_ */
