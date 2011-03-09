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

	/*
	char linFlag = 0;
	char hamFlag = 0;
	char lflag = 0;
	 */

	int wordLen[] = {0,0};
	int parityLen[] = {0,0};
	double errorProb = .01;
	int parityFlags = PARITY_FLAG_MAX;
	char* msgPath = NULL;
	char mode = INTERACTIVE;
	char* outPath = NULL;

	if(argc <= 1 || strstr(argv[1],"help"))
	{
		printUsage();
		return EXIT_SUCCESS;
	}

	// Seed Random number generator with system time.
	//srand(time(NULL));

	while ((c = getopt (argc, argv, "LRHhw:p:e:t:ro:")) != -1)
	{
		switch(c)
		{
			case 'L':

				break;

			case 'R':
				break;

			case 'H':
				break;

			case 'h':
				printUsage();
				break;

			case 'w':
				parseRange(optarg, ':', wordLen);
				break;

			case 'p':
				parseRange(optarg, ':', parityLen);
				break;

			case 'e':
				errorProb = atof(optarg);
				if(errorProb >= 1 || errorProb <= 0)
				{
					fprintf(stderr, "error: -%c most be between 0 and 1.\n", c); //FIXME testing only
					return EXIT_FAILURE;
				}
				break;

			case 't':
				parityFlags = atoi(optarg);
				break;

			case 'r':
				mode = REPORT;
				break;

			case 'o':
				outPath = optarg;
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

	/* After getopt switches set required arguments */
	if(argv[optind])
	{
		msgPath = argv[optind];
	}
	else
	{
		fprintf(stderr, "Missing required <message file> argument.\n");
		return EXIT_FAILURE;
	}

	runHarness(wordLen, parityLen, errorProb, parityFlags, msgPath, mode, outPath);

/*
	Matrix* pcm = newDenseParity(3,3);
	Matrix* g = newGeneratorMatrix(pcm);
	printAugMatrix(g,2-1);
	printf("\n\n");
	Matrix* vw = calcValidWords(g);
	printf("\n\n");
	printMatrix(vw);

*/
	/*
	Matrix* ma = newRandomParity(1, 3);
	Matrix* mb = newRandomParity(3, 5);
	Matrix* mc = newMatrix(1,5);

	printf("Matrix: A\n");
	printMatrix(ma);
	printf("\n\n");

	printf("Matrix: B\n");
	printMatrix(mb);
	printf("\n\n");

	bufferedBinaryMultiply(ma,mb,mc);
	printf("Matrix: C\n");
	printMatrix(mc);

	printf("\n\n");
	int i;
	printf("Matrix: C (for loop)\n");
	for(i=0; i<mc->cols; i++)
		printf(" %d", mc->data[i]);
	*/

	fprintf(stderr, "END\n"); //FIXME testing only
	return EXIT_SUCCESS;
}


/*
 * Prints neatly formatted usage / help instructions to the console.
 */

void printUsage()
{
	char* usage=""
	" Usage: " PROGRAM_NAME " [options] <message file>\n"
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
	" -r                Report mode. Exclude matrices from report.\n"
	" -o <path>         path to optional output file(s).\n"
	//" -k <number>       number of original bits\n"
	//" -n <number>       number of encoded bits\n"
	"\n"
	"Notes:\n"
	" <number> can be a single be a unsigned integer or a colon separated range.\n"
	"  .\n"
	" <message file> is the path to the message file.\n";

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

