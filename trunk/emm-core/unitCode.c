/*
 * unitCode.c
 *
 *  Created on: Mar 21, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include "code.h"

#define TESTPATH "tests/"

void utestNewCode(FILE* s)
{

}


void utestDelCode(FILE* s)
{

}


void utestNewGeneratorMatrix(FILE* s)
{

}


void utestNewControlMatrix(FILE* s)
{

}


void utestNewSyndromeMatrix(FILE* s)
{

}


void utestNewDenseParity(FILE* s)
{
	Matrix* dp1 = newDenseParity(4,4);
	Matrix* dp2 = newDenseParity(3,5);
	Matrix* exp1 = readMatrix(TESTPATH "denseparity4x4");
	Matrix* exp2 = readMatrix(TESTPATH "denseparity3x5");

	fprintf(s,"newDenseParity - test1: ");
	if(!equalsMatrix(dp1,exp1))
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	fprintf(s,"newDenseParity - test2: ");
	if(!equalsMatrix(dp2,exp2))
		fprintf(s,"failed\n");
	else
		fprintf(s,"passed\n");

	delMatrix(&dp1);
	delMatrix(&dp2);
	delMatrix(&exp1);
	delMatrix(&exp2);
}


void utestNewLowDensityParity(FILE* s)
{

}


void utestCalcValidWords(FILE* s)
{

}


void utestWordsByWeight(FILE* s)
{

}


void utestCalcMinDistance(FILE* s)
{

}


void utestEncode(FILE* s)
{

}


void utestDecode(FILE* s)
{

}



/* local main */
void utestCode(FILE* s)
{

	utestNewCode(s);
	utestDelCode(s);
	utestNewGeneratorMatrix(s);
	utestNewControlMatrix(s);
	utestNewSyndromeMatrix(s);
	utestNewDenseParity(s);
	utestNewLowDensityParity(s);
	utestCalcValidWords(s);
	utestWordsByWeight(s);
	utestCalcMinDistance(s);
	utestEncode(s);
	utestDecode(s);

	fprintf(s, "=======================\n");
	fprintf(s, "Code Testing - Complete\n");
	fprintf(s, "=======================\n");
}

