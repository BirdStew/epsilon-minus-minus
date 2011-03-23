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
	//Matrix* dp1 = readMatrix();
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

