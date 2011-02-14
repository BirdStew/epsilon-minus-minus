/*
 * matrix.h
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */

#ifndef MATRIX_H_
#define MATRIX_H_

struct matrix
{
	int rows;
	int cols;
	char* data;
};

typedef struct matrix matrix;

matrix* newMatrix(int rows, int cols);

#endif /* MATRIX_H_ */
