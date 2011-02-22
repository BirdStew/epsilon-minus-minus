/*
 * matrix.c
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"

/*
 * Maintainers' Notes:
 * It is very important that the matrix struct be unmodified in future revisions.
 * Maintainers should note that many of the functions are depend on the present
 * fields and the size of their data types.  It should also be understood that this
 * matrix is intend to contain BINARY values only.
 */



/*
 * Creates a matrix of dimensions rows, cols filled with zeros and
 * returns a pointer to the matrix.  Matrix dimensions cannot be negative.
 */
Matrix* newMatrix(int rows, int cols)
{
	if(rows < 1 || cols < 1)
	{
		fprintf(stderr,"error: Matrix dimensions less than 1.  Received %d,%d in 'newMatrix'\n", rows, cols);
	}

	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
	if(m == NULL)
	{
		fprintf(stderr,"error: malloc failed for 'matrix' in 'newMatrix'\n");
	}
	m->rows = rows;
	m->cols = cols;

	char* d = (char*)calloc(rows*cols, sizeof(char));
	if(d == NULL)
	{
		fprintf(stderr,"error: malloc failed for 'data' in 'newMatrix'\n");
	}

	m->data = d;
	return m;
}


/*
 * Receives a pointer to a matrix pointer and safely frees the matrix
 * struct from the heap.   The passed pointer is then dereferenced and
 * set to null to prevent users from reading unmanaged memory.
 */

void delMatrix(Matrix** m)
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


/*
 * Creates an Identity matrix of dimensions rows, rows and
 * returns a pointer to matrix.
 */

Matrix* makeIdentity(int rows)
{
	Matrix* m = newMatrix(rows, rows);

	int i;
	for(i = 0; i < rows; i++)
	{
		m->data[i * rows + i] = 1;
	}
	return m;
}


/*
 * Multiplies two matrices together, but instead of computing with
 * multiplication and addition operators, it uses binary AND and
 * OR operations.  The dimensional constraints are the same as a regular
 * matrix multiply.  This multiplication is based on the assumption that
 * matrix operands A and B contain only binary values.  The return value
 * is a newly created matrix resulting from the multiply.
 */

Matrix* binaryMultiply(Matrix* a, Matrix* b)
{
	//r c  * r c
	if(a->cols != b->rows)
	{
		fprintf(stderr, "error: cannot multiply! A.rows != B.cols in 'binaryMultiply'\n");
	}

	Matrix* c = newMatrix(a->rows, b->cols);

	int i,j,k;
    for(i = 0; i < c->rows; i++)
        for( j = 0; j < c->cols; j++)
            for( k = 0; k < b->rows; k++)
            {
            	c->data[i*c->cols+j] ^= a->data[i*a->cols+k] & b->data[k*b->cols+j];
            }

    return c;
}


/*
 * This function transposes a matrix in-place.
 */

void transposeMatrix(Matrix* m)
{
	int i, j, temp;
	for(i = 0; i < m->rows; i++)
	{
		for(j = 0; j < m->cols; j++)
		{
			temp = m->data[i * m->cols + j];
			m->data[i * m->cols + j] = m->data[j * m->rows + i];
			m->data[j * m->rows + i] = temp;
		}
	}

	temp = m-> rows;
	m->rows = m->cols;
	m->cols = temp;
}


/*
 * Creates and returns a new matrix with dimensions rows,  A.cols + b.cols.
 * In order to join both matrices A and B must the same number of rows.
 * The return value is a third matrix.  It is the responsibility of the user
 * to discard matrices A and B.
 */

Matrix* joinMatrix(Matrix* a, Matrix* b)
{
	if(a->rows != b->rows)
	{
		fprintf(stderr, "error: Matrices must have the same number of rows 'joinMatrix'\n");
		exit(EXIT_FAILURE);
	}

	Matrix* c = newMatrix(a->rows, a->cols + b->cols);

	int i, j;
	for(i = 0; i < c->rows; i++)
	{
		for(j = 0; j < c->cols; j++)
		{
			if(j < a->rows)
			{
				c->data[i * c->cols + j] = a->data[i * a->cols + j];
			}
			else
			{
				c->data[i * c->cols + j] = b->data[i * b->cols + (j - a->cols) ];
			}
		}
	}

	return c;
}


void printAugMatrix(Matrix* m, int column)
{
	int i, j;
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


void printMatrix(Matrix* m)
{
	printAugMatrix(m, -1);
}


/*
 * Returns the contents of a matrix represented as a string.
 * Rows a delimited by a 'LF' (Line Feed) character.
 */

char* toString(Matrix* m)
{
	int strLen = m->rows * m->cols * sizeof(char) + m->rows + 1;
	char* str = (char*)malloc(strLen);
	int i, j;
	const char asciiOffset = 48;
	for(i = 0; i < m->rows; i++)
	{
		for(j = 0; j < m->cols; j++)
		{
			str[i * m->cols + j + i] = (char)(m->data[i * m->cols + j] + asciiOffset);
		}
		str[i * m->cols + j + i] = '\n';
	}
	str[strLen - 1] = '\0';
	return str;
}
