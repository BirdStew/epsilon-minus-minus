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
matrix* makeIdentity(int rows);
matrix* binaryMultiply(matrix* a, matrix* b);
void transposeMatrix(matrix* m);
matrix* joinMatrix(matrix* a, matrix* b);
void printAugMatrix(matrix* m, int column);
void printMatrix(matrix* m);
char* toString(matrix* m);

#endif /* MATRIX_H_ */
