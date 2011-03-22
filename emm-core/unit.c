/*
 * unit.c
 *
 *  Created on: Mar 21, 2011
 *      Author: James Bettke
 */

//#ifdef UNIT

#include <stdio.h>
#include <stdlib.h>

/* Not orthodox, but don't care at this point.
 * I see no need to make separate header files
 */
void utestCode(FILE* s);
void utestHarness(FILE* s);
void utestMatrix(FILE* s);
void utestMessage(FILE* s);

int main( int argc, char** argv )
{
	char* filePath = argv[1];
	FILE* s;

	if(filePath)
	{
		s = fopen(filePath, "w");
		if(!s)
		{
			fprintf(stderr,"error: failed to open '%s' for writing\n", filePath);
			return EXIT_FAILURE;
		}
	}
	else
	{
		s = stdout;
	}

	/* run test cases */
	utestCode(s);
	utestHarness(s);
	utestMatrix(s);
	utestMessage(s);

	fprintf(s,"\n\n");
	fprintf(s, "=======================\n");
	fprintf(s, "Unit Testing - Complete\n");
	fprintf(s, "=======================\n");

	if(filePath)
	{
		fclose(s);
	}
	return EXIT_SUCCESS;
}

//#endif /* UNIT */
