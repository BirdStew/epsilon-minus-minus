/*
 * code.c
 *
 *  Created on: Feb 17, 2011
 *      Author: James Bettke
 */
#include "code.h"
#include <stdio.h> //FIXME

matrix* newGeneratorMatrix(matrix* pce)
{
	matrix* identity = newMatrix(pce->rows, pce->rows);
	setIdentity(identity);

	matrix* gen = joinMatrix(identity, pce);

	delMatrix(&identity);

	return gen;
}


matrix* setPCE(int method)
{
 // j should always start at offset
 // offset = k-n
}
