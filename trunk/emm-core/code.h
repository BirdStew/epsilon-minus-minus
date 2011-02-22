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
	Matrix* generator;
	Matrix* control;
	Matrix* syndrome;
} Code;

enum PARITY_TYPE
{
	DENSE,
	LOW_DENSITY,
	RANDOM
};

Code* newCode(int wordLen, int parityLen, int parityType);
void delCode(Code** c);
Matrix* newGeneratorMatrix(Matrix* pce);
Matrix* newControlMatrix(Matrix* pcm);
Matrix* newSyndromeMatrix(int wordLen, int parityLen);
Matrix* newDenseParity(int rows, int cols);
Matrix* newLowDensityParity(int rows, int cols);
Matrix* newRandomParity(int rows, int cols);


#endif /* CODE_H_ */
