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
void utestCode();
void utestHarness();
void utestMatrix();
void utestMessage();

int main( int argc, char** argv )
{
	//utestCode();
	//utestHarness();
	utestMatrix();
	//utestMessage();

	return EXIT_SUCCESS;
}

//#endif /* UNIT */
