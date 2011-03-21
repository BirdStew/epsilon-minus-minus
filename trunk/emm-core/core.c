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
#include <time.h>
#include "harness.h"

	/* Testing */
	#include "code.h"

#define PROGRAM_NAME "emm-core"

void printUsage();
void parseRange(char* str, char delimiter, int* range);


int main( int argc, char** argv )
{
	signed char c;

	harnessOptions hop;

	hop.wordLen[0]=0,   hop.wordLen[1]=0;
	hop.parityLen[0]=0, hop.parityLen[1]=0;
	hop.errorProb = .01;
	hop.parityFlags = PARITY_FLAG_MAX;
	hop.customMatrixPath = NULL;
	hop.msgPath = NULL;
	hop.outPath = NULL;
	hop.offset = 0;

	if(argc <= 1 || strstr(argv[1],"help"))
	{
		printUsage();
		return EXIT_SUCCESS;
	}

	//Seed Random number generator with system time.
	srand(time(NULL));

	while ((c = getopt (argc, argv, "hw:p:e:t:c:o:s:")) != -1)
	{
		switch(c)
		{
			case 'h':
				printUsage();
				break;

			case 'w':
				parseRange(optarg, ':', hop.wordLen);
				break;

			case 'p':
				parseRange(optarg, ':', hop.parityLen);
				break;

			case 'e':
				hop.errorProb = atof(optarg);

				if(hop.errorProb >= 1 || hop.errorProb <= 0)
				{
					fprintf(stderr, "error: -%c must be between 0 and 1.\n", c);
					return EXIT_FAILURE;
				}

				if(hop.errorProb*100 != (int)(hop.errorProb*100))
				{
					hop.errorProb = ((double)((int)(hop.errorProb * 100)) / 100);
					fprintf(stderr, "warning: -%c rounds to precision 2 decimal places.\n", c);
				}
				break;

			case 't':
				hop.parityFlags = atoi(optarg);
				break;

			case 'c':
				hop.customMatrixPath = optarg;
				break;

			case 'o':
				hop.outPath = optarg;
				break;

			case 's':
				hop.offset = atoi(optarg);
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

	/* require custom path if selected */
	if(hop.parityFlags == CUSTOM)
	{
		if(hop.customMatrixPath == NULL)
		{
			fprintf(stderr, "Missing required -c <custom matrix path> argument.\n");
			return EXIT_FAILURE;
		}

		if(hop.wordLen[0] != 0 || hop.parityLen[0] != 0)
		{
			fprintf(stderr, "warning: ignoring -w and -p options when using custom matrix.\n");
		}
	}

	/* require output path */
	if(hop.outPath == NULL)
	{
		fprintf(stderr, "Missing required -o <output directory> argument.\n");
		return EXIT_FAILURE;
	}

	/* require message path */
	if(argv[optind])
	{
		hop.msgPath = argv[optind];
	}
	else
	{
		fprintf(stderr, "Missing required <message file> argument.\n");
		return EXIT_FAILURE;
	}

	runHarness(&hop);


	fprintf(stderr, "END\n"); //FIXME testing only
	return EXIT_SUCCESS;
}


/*
 * Prints neatly formatted usage / help instructions to the console.
 */

void printUsage()
{
	char* usage=""
	" Usage: " PROGRAM_NAME " [options] <message>\n"
	"\n"
	"Options:\n"
	//" -L                use linear encoding\n"
	//" -R                use Reed–Muller encoding\n"
	//" -H                use Hamming encoding\n"
	" -h                print help.\n"
	" -w <number>       number of word bits.\n"
	" -p <number>       number of parity bits.\n"
	" -e <float>        error probability.\n"
	" -t <number>       flags for parity type.\n"
	" -c <path>         path to custom parity matrix.\n"
	" -o <path>         path to output directory.\n"
	" -s <number>       Offset to start message file.\n"
	//" -k <number>       number of original bits\n"
	//" -n <number>       number of encoded bits\n"
	"\n"
	"Notes:\n"
	" <number>  single unsigned integer or a colon separated range.\n"
	"\n"
	" <float>   floating point number with a precision of 2 decimal places\n"
	"           between 0 and 1.\n"
	"\n"
	" <path>    file path to the output directory. Do not terminate with path\n"
	"           separator.\n"
	"\n"
	" <message> path to the message file.\n"
	"\n"
	" Parity Type Flags: A number where each bit defines the types of parity matrices\n"
	" to use during the simulation. 1.) DENSE, 2.) LOW_DENSITY, 4.) RANDOM. A flag\n"
	" of 0 enables the <Custom Parity Matrix> argument.\n"
	"\n"
	" Custom Parity Matrix: A text file containing ASCII 1's and 0's. Rows should be\n"
	" terminated by a newline character.\n";

	printf("%s", usage);

}


/*
 *
 */

void parseRange(char* str, char delimiter, int* range)
{
	char* found = strchr(str, delimiter);
	if(found == NULL)
	{
		range[0] = atoi(str);
		range[1] = range[0];
	}
	else
	{
		found[0] = '\0';
		found++;
		range[0] = atoi(str);
		range[1] = atoi(found);

		if(range[0] > range[1])
		{
			printf("warning: invalid range of '%d:%d'.  Rewriting as '%d:%d'\n",range[0],range[1], range[1], range[0]);
			int temp = range[0];
			range[0] = range[1];
			range[1] = temp;
		}
	}
}

