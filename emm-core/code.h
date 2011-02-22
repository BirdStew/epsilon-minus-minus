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
	Matrix* generator;
	Matrix* control;
	Matrix* syndrome;
} Code;

Matrix* newGeneratorMatrix(Matrix* pce);
Matrix* newControlMatrix(Matrix* pcm);
Matrix* newDenseParity(int rows, int cols);
Matrix* newLowDenseParity(int rows, int cols);


#endif /* CODE_H_ */
