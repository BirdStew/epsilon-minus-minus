/*
 * main.c
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <stdlib.h>

void printUsage();



int main( int argc, const char** argv )
{
	printUsage();
	return EXIT_SUCCESS;
}


void printUsage()
{
	char* usage=""
	" Usage: emm-core [options] <message>\n"
	"\n"
	"Options:\n"
	" -L                use linear encoding\n"
	" -R                use Reed–Muller encoding\n"
	" -H                use Hamming encoding\n"
	" -h                print help.\n"
	" -k <number>       number of original bits\n"
	" -n <number>       number of encoded bits\n"
	"\n"
	"Notes:\n"
	" <number> can be a single be a unsigned integer or a colon separated range.\n"
	" <message> is the path to the mesage file.\n";

	printf("%s", usage);

}
