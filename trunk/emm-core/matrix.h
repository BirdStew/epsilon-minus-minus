/*
 * matrix.h
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */

#ifndef MATRIX_H_
#define MATRIX_H_

struct struc_mx
{
	int rows;
	int cols;
	char* data;
};

typedef struct struc_mx matrix;

matrix* newMatrix(int rows, int cols);
void delMatrix(matrix** m);
void makeIdentity(matrix* m);
matrix* binaryMultiply(matrix* a, matrix* b);

#endif /* MATRIX_H_ */
