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
Matrix* newDenseParity(int rows, int cols);
Matrix* newControlMatrix(Matrix* pcm);

#endif /* CODE_H_ */
