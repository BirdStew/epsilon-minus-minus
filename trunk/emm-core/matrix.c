/*
 * matrix.c
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"


matrix* newMatrix(int rows, int cols)
{
	matrix* m = (matrix*)malloc(sizeof(matrix));
	if(m == NULL)
		fprintf(stderr,"error: malloc failed for 'matrix' in newMatrix\n");

	// Fill matrix with zeros
	memset(m, 0, rows * cols);

	m->rows = rows;
	m->cols = cols;

	char* d = (char*)malloc(sizeof(char)*rows*cols);
	if(d == NULL)
		fprintf(stderr,"error: malloc failed for 'data' in newMatrix\n");

	m->data = d;
	return m;
}

void delMatrix(matrix** m)
{
	if(*m == NULL)
	{
		fprintf(stderr,"error: passed ptr is NULL in delMatrix\n");
	}
	else
	{
		free((*m)->data);
		free(*m);
		*m = NULL;
	}

}

void makeIdentity(matrix* m)
{
	int i;
	for(i = 0; i < m->rows; i += m->rows)
	{
		m->data[i];
	}
}

matrix* binaryMultiply(matrix* a, matrix* b)
{
	//r c  * r c
	if(a->cols != b->rows)
	{
		fprintf(stderr, "error: cannot multiply! A.rows != B.cols in binaryMultiply\n");
	}

	matrix* c = newMatrix(a->rows, b->cols);

	int i,j,k;
    for(i = 0; i < a->cols; i++)
        for( j = 0; j < b->rows; j++)
            for( k = 0; k < 3; k++)
            {
            	c->data[i*c->cols+j] ^= a->data[i*a->cols+k] & b->data[k*b->cols+j];
            }

    return c;
}
