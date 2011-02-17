/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */
#include "code.h"
#include <stdio.h> //FIXME

matrix* newGeneratorMatrix(int k, int n)
{
	//matrix* gen = newMatrix(k, n);
	//setIdentity(gen);

	//printAugMatrix(gen, 3);
	//transposeMatrix(gen);
	//printMatrix(gen);

	matrix* a = newMatrix(4,4);
	matrix* b = newMatrix(4,4);
	printf("\nA\n");
	setIdentity(a);
	printMatrix(a);
	printf("\nB\n");
	setIdentity(b);
	printMatrix(b);
	printf("\nC\n");
	matrix* c = joinMatrix(a,b);
	printAugMatrix(c, 3);

	return 0;
}


void setPCE(matrix *m, int k)
{
 // j should always start at offset
 // offset = k-n
}
