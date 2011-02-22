/*
 * code.h
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */

#ifndef CODE_H_
#define CODE_H_

#include "matrix.h"

Matrix* newGeneratorMatrix(Matrix* pce);
Matrix* setPCE(int method);
Matrix* newDenseParity(int rows, int cols);

#endif /* CODE_H_ */
