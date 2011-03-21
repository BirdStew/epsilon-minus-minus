/*
 * unitMatrix.c
 *
 *  Created on: Mar 21, 2011
 *      Author: James Bettke
 */

#include "matrix.h"


void utestNewIdentity()
{

}

void utestBufferedBinaryMultiply()
{

}

void utestTransposeMatrix()
{

}

void utestJoinMatrix()
{

}

void utestMatrixToString()
{

}

void utestVectorAsInt()
{
	printf("vectorAsInt - ");

	char t[] = {1,0,0,1,1,1,0}; // 0x4E
	char t2[] = {1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1}; // 0x80AAAAAB
	char t3[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // 0xFFFFFFFF

	Matrix m;
	m.rows = 1;
	m.cols = 7;

	m.data = &t;
	if(vectorAsInt(&m) != 0x4E)
	{
		printf("failed\n");
		return;
	}

	m.data = &t2;
	if(vectorAsInt(&m) != 0x80AAAAAB)
	{
		printf("failed\n");
		return;
	}

	m.data = &t3;
	if(vectorAsInt(&m) != 0xFFFFFFFF)
	{
		printf("failed\n");
		return;
	}

	printf("passed\n");
}

void utestCopyMatrix()
{

}

void utestReadMatrix()
{

}


/* local main */
void utestMatrix()
{
	utestNewIdentity();
	utestBufferedBinaryMultiply();
	utestTransposeMatrix();
	utestJoinMatrix();
	utestMatrixToString();
	utestVectorAsInt();
	utestCopyMatrix();
	utestReadMatrix();
}
