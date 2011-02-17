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
		fprintf(stderr,"error: malloc failed for 'matrix' in 'newMatrix'\n");

	m->rows = rows;
	m->cols = cols;

	char* d = (char*)calloc(rows*cols, sizeof(char));
	if(d == NULL)
		fprintf(stderr,"error: malloc failed for 'data' in 'newMatrix'\n");

	m->data = d;
	return m;
}

void delMatrix(matrix** m)
{
	if(*m == NULL)
	{
		fprintf(stderr,"error: passed ptr is NULL in 'delMatrix'\n");
	}
	else
	{
		free((*m)->data);
		free(*m);
		*m = NULL;
	}

}

void setIdentity(matrix* m, int offset)
{
	if(m->rows + offset > m->cols)
	{
		fprintf(stderr, "error: Setting identity would extend out-of-bounds in 'setIdentity'\n");
		exit(EXIT_FAILURE);
	}

	int i;
	for(i = 0; i < m->rows; i++)
	{
		m->data[i * m->cols + i + offset] = 1;
	}
}

matrix* binaryMultiply(matrix* a, matrix* b)
{
	//r c  * r c
	if(a->cols != b->rows)
	{
		fprintf(stderr, "error: cannot multiply! A.rows != B.cols in 'binaryMultiply'\n");
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


void printAugMatrix(matrix* m, int column)
{
	int i,j;
	for(i = 0; i < m->rows; i++)
	{
		for(j = 0; j < m->cols; j++)
		{
			printf("%d ", m->data[i * m->cols + j]);
			if(column > -1 && column == j)
				printf("| ");
		}
		printf("\n");
	}
}


void printMatrix(matrix* m)
{
	printAugMatrix(m, -1);
}
