/*
 * unitMatrix.c
 *
 *  Created on: Mar 21, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <string.h>
#include "matrix.h"


void utestNewIdentity(FILE* s)
{

}

void utestBufferedBinaryMultiply(FILE* s)
{

}

void utestTransposeMatrix(FILE* s)
{

}

void utestJoinMatrix(FILE* s)
{

}

void utestMatrixToString(FILE* s)
{

}

void utestVectorAsInt(FILE* s)
{
	fprintf(s,"vectorAsInt - ");

	char t[] = {1,0,0,1,1,1,0}; // 0x4E
	char t2[] = {1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1}; // 0x80AAAAAB
	char t3[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // 0xFFFFFFFF

	Matrix m;
	m.rows = 1;

	m.data = t;
	m.cols = 7;
	if(vectorAsInt(&m) != 0x4E)
	{
		fprintf(s,"test1: failed\n");
		return;
	}

	m.data = t2;
	m.cols = 32;
	if(vectorAsInt(&m) != 0x80AAAAAB)
	{
		fprintf(s,"test2: failed\n");
		return;
	}

	m.data = t3;
	m.cols = 32;
	if(vectorAsInt(&m) != 0xFFFFFFFF)
	{
		fprintf(s,"test3: failed\n");
		return;
	}

	fprintf(s,"passed\n");
}

void utestCopyMatrix(FILE* s)
{

}

void utestReadMatrix(FILE* s)
{
	memcmp()
}


/* local main */
void utestMatrix(FILE* s)
{
	utestNewIdentity(s);
	utestBufferedBinaryMultiply(s);
	utestTransposeMatrix(s);
	utestJoinMatrix(s);
	utestMatrixToString(s);
	utestVectorAsInt(s);
	utestCopyMatrix(s);
	utestReadMatrix(s);
}
