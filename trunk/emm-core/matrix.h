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
void setIdentity(matrix* m, int offset);
matrix* binaryMultiply(matrix* a, matrix* b);
void transposeMatrix(matrix* m);
void printAugMatrix(matrix* m, int column);
void printMatrix(matrix* m);

#endif /* MATRIX_H_ */
