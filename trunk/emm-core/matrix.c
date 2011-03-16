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
	if(rows < 0 || cols < 0)
	{
		fprintf(stderr,"error: Matrix dimensions less than 0.  Received %d,%d in 'newMatrix'\n", rows, cols);
		exit(EXIT_FAILURE);
	}

	Matrix* m = (Matrix*)malloc(sizeof(Matrix));
	if(m == NULL)
	{
		fprintf(stderr,"error: malloc failed for 'matrix' in 'newMatrix'\n");
	}
	m->rows = rows;
	m->cols = cols;

	char* data = (char*)calloc(rows*cols, sizeof(char));
	if(data == NULL)
	{
		fprintf(stderr,"error: malloc failed for 'data' in 'newMatrix'\n");
	}

	m->data = data;
	return m;
}


/*
 * Receives a pointer to a matrix pointer and safely frees the matrix
 * struct from the heap. The passed pointer is then dereferenced and
 * set to null to prevent users from reading unmanaged memory.
 */

void delMatrix(Matrix** m)
{
	if(*m == NULL || m == NULL)
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

Matrix* newIdentity(int rows)
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
 * OR operations. The dimensional constraints are the same as a regular
 * matrix multiply. This multiplication is based on the assumption that
 * matrix operands A and B contain only binary values. The result is stored
 * in matrix C.
 */

void bufferedBinaryMultiply(Matrix* a, Matrix* b, Matrix* c)
{
	//r c  * r c
	if(a->cols != b->rows)
	{
		fprintf(stderr, "error: cannot multiply! A.cols != B.rows in 'bufferedBinaryMultiply'\n");
		fprintf(stderr, "       Trying to multiply A[%d x %d] * B[%d x %d] \n", a->rows, a->cols, b->rows, b->cols);
		exit(EXIT_FAILURE);
	}

	//Matrix* c = newMatrix(a->rows, b->cols); //Used for old BinaryMultiply where result is malloc'd on the fly
	if(a->rows != c->rows || b->cols != c->cols)
	{
		fprintf(stderr, "error: cannot multiply! Result matrix of invalid dimensions 'bufferedBinaryMultiply'\n");
		fprintf(stderr, "       Product should be %d x %d, given %d x %d.\n", a->rows, b->cols, c->rows, c->cols);
		exit(EXIT_FAILURE);
	}

	int i,j,k;
    for(i = 0; i < c->rows; i++)
    {
    	for( j = 0; j < c->cols; j++)
        {
    		c->data[i*c->cols+j] = 0;
        	for( k = 0; k < b->rows; k++)
            {
				c->data[i*c->cols+j] ^= a->data[i*a->cols+k] & b->data[k*b->cols+j];
			}
        }
    }
}


/*
 * This function transposes a matrix in-place.  If either the
 * rows or the columns is one then it is a one dimensional array.
 * There is no need to transposed the values. Simply reverse reverse
 * the dimensions.
 */

void transposeMatrix(Matrix* m)
{
	int i, j, temp;

	if(m->rows != 1 && m->cols != 1)
	{
		char* tData = malloc(m->rows * m->cols);

		for(i = 0; i < m->rows; i++)
		{
			for(j = 0; j < m->cols ; j++)
			{
				tData[j * m->rows + i] = m->data[i * m->cols + j];
			}
		}
		free(m->data);
		m->data = tData;
	}

	temp = m-> rows;
	m->rows = m->cols;
	m->cols = temp;
}


/*
 * Creates and returns a new matrix with dimensions rows,  A.cols + b.cols.
 * In order to join both matrices A and B must the same number of rows.
 * The return value is a third matrix. It is the responsibility of the user
 * to discard matrices A and B.
 */

Matrix* joinMatrix(Matrix* a, Matrix* b)
{
	if(a == NULL || b == NULL)
	{
		fprintf(stderr, "error: passed null ptr in 'joinMatrix'\n");
		exit(EXIT_FAILURE);
	}

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
			if(j < a->cols)
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
 * Rows a delimited by a commas.
 */

char* matrixToString(Matrix* m)
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
		str[i * m->cols + j + i] = ',';
	}
	str[strLen - 1] = '\0';
	return str;
}


int vectorAsInt(Matrix* m)
{
	int value = 0;
	if(m->rows != 1 && m->cols != 1)
	{
		fprintf(stderr, "warning: passed matrix [%d, %d] is not a vector in 'vectorAsInt'\n", m->rows, m->cols);
	}

	int i, b;
	int n = m->cols * m->rows;
	for(i = n-1, b=0; i >= 0; i--, b++)
	{
		value = value | (m->data[i] << b);
	}

	return value;
}


void copyMatrix(Matrix* src, Matrix* dst)
{
	if(src->rows != dst->rows || src->cols != dst->cols)
	{
		fprintf(stderr, "error: Matrices must have the same dimensions in 'copyMatrix'\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		int i;
		int n = src->rows * src->cols;
		for(i = 0; i < n; i++)
		{
			dst->data[i] = src->data[i];
		}
	}
}


Matrix* readMatrix(char* filePath)
{
	FILE* fh = fopen(filePath,"r");

	if(fh == NULL)
	{
		fprintf(stderr, "error: failed to open '%s' for reading in 'readMatrix'\n", filePath);
		exit(EXIT_FAILURE);
	}

	/* Get File Size */
	fseek(fh, 0, SEEK_END);
	long fileSize = ftell(fh);
	fseek(fh, 0, SEEK_SET);

	/* allocate space for buffer */
	Matrix* m = newMatrix(1, fileSize);

	long result = fread(m->data, sizeof(char), fileSize, fh);

	if(result != fileSize)
	{
		fprintf(stderr, "error: only read %ld of %ld bytes in 'readMatrix'\n", result, fileSize);
		exit(EXIT_FAILURE);
	}

	int firstRowLen = 0;
	int lineCount = 0;
	int shift = 0;
	char last;

	int i;
	for(i = 0; i < fileSize; i++)
	{
		last = m->data[i];
		if(last == '0' || last == '1' )
			m->data[i - shift] = last - 48;
		else
		{
			shift++;
			if(last == '\n')
			{
				lineCount++;
				if(firstRowLen == 0)
					firstRowLen = i-shift+1;
			}
		}
	}
	m->data = realloc(m->data, i - shift);
	m->rows = lineCount;
	m->cols = firstRowLen;

	if(m->rows * m->cols != fileSize - shift)
	{
		fprintf(stderr, "error: invalid matrix format in 'readMatrix'\n");
		exit(EXIT_FAILURE);
	}

	fclose(fh);
	return m;
}
