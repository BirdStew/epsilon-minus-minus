/*
 * main.c
 *
 *  Created on: Feb 14, 2011
 *      Author: James Bettke
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

				/* TESTING ONLY */
				#include "code.h"
				#include "matrix.h"

#define PROGRAM_NAME "emm-core"

void printUsage();



int main( int argc, const char** argv )
{
	signed char c;
	char linFlag = 0;
	char hamFlag = 0;
	char lflag = 0;

	int n = {0,0};
	int k = {0,0};
	int p = .01;


	if(argc <= 1 || strstr(argv[1],"help"))
		printUsage();

	while ((c = getopt (argc, argv, "LRHhk:n:")) != -1)
	{
		switch(c)
		{
			case 'L':
				newGeneratorMatrix(4,8);
			break;

			case 'R':
			break;

			case 'H':
			break;

			case 'h':
				printUsage();
			break;

			case 'k':
			break;

			case 'n':
			break;

			case ':':
				fprintf(stderr, "Option -%c requires an operand\n", optopt);
			break;

			case '?':
				fprintf(stderr, "Try `%s --help' for more information.\n",  PROGRAM_NAME);
				return EXIT_FAILURE;
			break;
		}

	}

	return EXIT_SUCCESS;
}


void printUsage()
{
	char* usage=""
	" Usage: " PROGRAM_NAME " [options] <message>\n"
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
	" <message> is the path to the message file.\n";

	printf("%s", usage);

}
