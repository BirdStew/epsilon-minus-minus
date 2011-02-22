/*
 * matrix.h
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */

#ifndef MATRIX_H_
#define MATRIX_H_

typedef struct Matrix
{
	int rows;
	int cols;
	char* data;
} Matrix;


Matrix* newMatrix(int rows, int cols);
void delMatrix(Matrix** m);
Matrix* makeIdentity(int rows);
Matrix* binaryMultiply(Matrix* a, Matrix* b);
void transposeMatrix(Matrix* m);
Matrix* joinMatrix(Matrix* a, Matrix* b);
void printAugMatrix(Matrix* m, int column);
void printMatrix(Matrix* m);
char* matrixToString(Matrix* m);

#endif /* MATRIX_H_ */
