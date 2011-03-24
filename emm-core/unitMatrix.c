/*
 * unitMatrix.c
 *
 *  Created on: Mar 21, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

#define TESTPATH "tests/"

void utestNewIdentity(FILE* s)
{
	char adata[] = {1,0,1,0};
	char bdata[] = {1,0,0,0,1,0,0,0,1};
	Matrix m; m.rows = 2; m.cols = 2; m.data = adata;

	fprintf(s,"newIdentity - test1: ");
	Matrix* i = newIdentity(2);
	if(memcmp(m.data, adata, m.rows*m.cols) && m.rows != 2 && m.cols != 2)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");
	delMatrix(&i);

	fprintf(s,"newIdentity - test2: ");
	Matrix* i2 = newIdentity(3);
	m.rows = 3; m.cols = 3; m.data = bdata;
	if(memcmp(m.data, bdata, m.rows*m.cols) && m.rows != 3 && m.cols != 3)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	delMatrix(&i2);
}


void utestBufferedBinaryMultiply(FILE* s)
{
	char adata[] = {1,0,0,1,1,1};
	char bdata[] = {1,1,1};
	char rdata[] = {0,0};
	char cdata[] = {1,1}; //FIXME
	Matrix a; a.rows = 2; a.cols = 3; a.data = adata;
	Matrix b; b.rows = 3; b.cols = 1; b.data = bdata;
	Matrix r; r.rows = 2; r.cols = 1; r.data = rdata;

	fprintf(s,"bufferedBinaryMultiply - test1: ");
	bufferedBinaryMultiply(&a,&b,&r);
	if(memcmp(r.data, cdata, r.rows*r.cols) && r.rows != 2 && r.cols != 1)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");
}


void utestTransposeMatrix(FILE* s)
{
	char* adata = (char*)malloc(6);
	int i;for(i=0;i<6;i++) adata[i]=i+1;

	char bdata[] = {1,3,5,2,4,6};
	char cdata[] = {1,1,0,0,0,0};
	Matrix m; m.rows = 3; m.cols = 2; m.data = adata;

	fprintf(s,"transposeMatrix - test1: ");
	transposeMatrix(&m);
	if(memcmp(m.data, bdata, m.rows*m.cols) && m.rows != 2 && m.cols != 3)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	fprintf(s,"transposeMatrix - test2: ");
	transposeMatrix(&m);
	if(memcmp(m.data, adata, m.rows*m.cols) && m.rows != 3 && m.cols != 2)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	fprintf(s,"transposeMatrix - test3: ");
	m.rows = 1; m.cols = 6; m.data = cdata;
	transposeMatrix(&m);
	if(memcmp(m.data, cdata, m.rows*m.cols) && m.rows != 6 && m.cols != 1)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	free(adata);
}


void utestJoinMatrix(FILE* s)
{
	char adata[] = {1,0,0,0,0,1};
	char bdata[] = {1,1,0,0};
	char t[] = {1,0,0,1,1,0,0,1,0,0};

	Matrix a; a.rows = 2; a.cols = 3; a.data = adata;
	Matrix b; b.rows = 2; b.cols = 2; b.data = bdata;

	Matrix* j = joinMatrix(&a,&b);

	fprintf(s,"joinMatrix - test1: ");
	if(memcmp(j->data, t, a.rows*a.cols + b.rows*b.cols) && j->rows == a.rows && j->cols == a.cols + b.cols)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	delMatrix(&j);
}


void utestMatrixToString(FILE* s)
{

}


void utestVectorAsInt(FILE* s)
{
	char t[] = {1,0,0,1,1,1,0}; // 0x4E
	char t2[] = {1,0,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1}; // 0x80AAAAAB
	char t3[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}; // 0xFFFFFFFF

	Matrix m;
	m.rows = 1;

	m.data = t;
	m.cols = 7;
	fprintf(s,"vectorAsInt - test1: ");
	if(vectorAsInt(&m) != 0x4E)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	m.data = t2;
	m.cols = 32;

	fprintf(s,"vectorAsInt - test2: ");
	if(vectorAsInt(&m) != 0x80AAAAAB)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	m.data = t3;
	m.cols = 32;
	fprintf(s,"vectorAsInt - test3: ");
	if(vectorAsInt(&m) != 0xFFFFFFFF)
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");
}


void utestCopyMatrix(FILE* s)
{

}


void utestReadMatrix(FILE* s)
{
	char t[] = {1,0,0,1,1,0,1,0,1,0,0,1,0,1,1,1,0,1,1,1,1};
	Matrix m; m.rows = 3; m.cols = 7; m.data = t;
	Matrix* r = readMatrix(TESTPATH "3x7.txt");

	fprintf(s,"readMatrix - test1: ");
	if(memcmp(r->data, m.data, m.rows * m.cols) && r->rows != 3 && r->cols != 7)
	{
		fprintf(s,"failed\n");
		fprintf(s, "warning: critical function 'readMatrix' failed\n");
	}
	else
		fprintf(s,"passed\n");

	delMatrix(&r);

	char t2[] = {0,0,0,1,1,0,1,1};
	m.rows = 4; m.cols = 2; m.data = t2;
	Matrix* r2 = readMatrix(TESTPATH "4x2.txt");


	fprintf(s,"readMatrix - test2: ");
	if(memcmp(r2->data, m.data, m.rows * m.cols) && r2->rows != 4 && r2->cols != 2)
	{
		fprintf(s,"failed\n");
		fprintf(s, "warning: critical function 'readMatrix' failed\n");
	}
	else
		fprintf(s,"passed\n");

	delMatrix(&r2);
}


void utestEqualsMatrix(FILE* s)
{
	Matrix a; a.rows = 3; a.cols = 3; char adata[] = {1,0,0,1,0,0,1,0,0}; a.data = adata;
	Matrix b; b.rows = 3; b.cols = 3; char bdata[] = {0,0,0,1,1,1,0,0,0}; b.data = bdata;
	Matrix c; c.rows = 1; c.cols = 2; char cdata[] = {1,0}; c.data = cdata;
	Matrix d; d.rows = 2; d.cols = 1; char ddata[] = {1,0}; d.data = ddata;

	fprintf(s,"equalsMatrix - test1: ");
	if(equalsMatrix(&a,&b) == 1)
	{
		fprintf(s,"failed\n");
		fprintf(s, "warning: critical function 'equalsMatrix' failed\n");
	}
	else
		fprintf(s,"passed\n");

	fprintf(s,"equalsMatrix - test2: ");
	if(equalsMatrix(&c,&d) == 1)
	{
		fprintf(s,"failed\n");
		fprintf(s, "warning: critical function 'equalsMatrix' failed\n");
	}
	else
		fprintf(s,"passed\n");

	fprintf(s,"equalsMatrix - test3: ");
	if(equalsMatrix(&a,&a) == 0)
	{
		fprintf(s,"failed\n");
		fprintf(s, "warning: critical function 'equalsMatrix' failed\n");
	}
	else
		fprintf(s,"passed\n");
}


/* local main */
void utestMatrix(FILE* s)
{
	utestEqualsMatrix(s); /* Critical */
	utestReadMatrix(s); /* Critical */
	utestNewIdentity(s);
	utestBufferedBinaryMultiply(s);
	utestTransposeMatrix(s);
	utestJoinMatrix(s);
	utestMatrixToString(s);
	utestVectorAsInt(s);
	utestCopyMatrix(s);

	fprintf(s, "=========================\n");
	fprintf(s, "Matrix Testing - Complete\n");
	fprintf(s, "=========================\n");
}
