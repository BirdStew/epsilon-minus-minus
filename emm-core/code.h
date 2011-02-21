/*
 * code.h
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */

#ifndef CODE_H_
#define CODE_H_

#include "matrix.h"

matrix* newGeneratorMatrix(matrix* pce);
matrix* setPCE(int method);
matrix* newDenseParity(int rows, int cols);

#endif /* CODE_H_ */
