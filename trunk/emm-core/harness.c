/*
 * harness.c
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */
#include <stdlib.h>
#include "harness.h"
#include <stdio.h> /* LoadMessage / exportJson */




void runHarness(int* wordLen, int* parityLen, double errorProb, int parityFlags,  char* msgPath)
{

	CodeStats stats;
	Message msg;

	loadMessage(msgPath, &msg);

	int w, p, pType;
	for(w = wordLen[0]; w <= wordLen[1]; w++)
	{
		for(p = parityLen[0]; p <= parityLen[1]; p++)
		{
			for(pType = 1; pType <= PARITY_FLAG_MAX; pType <<= 1  )
			{
				if(pType & parityFlags)
				{
					Code* code = newCode(w, p, pType);
					testCode(code, &msg, errorProb, &stats);
					delCode(&code);
				}
			}
		}
	}
}


void testCode(Code* code, Message* msg, double errorProb, CodeStats* stats)
{
	/*
	 * --Need to write util function to read message from file.
	 * Need to write nextPacket function to consume message in parts.  track bits. byte boundry issue.
	 * Need to write until function to export code/stats struct to JSON
	 * Send fredo php constants for parity types
	 */

	//fprintf(stderr, "start testCode\n"); //FIXME
	Matrix* packet = newMatrix(1, code->wordLen);
	Matrix* encodedPacket = newMatrix(1, code->wordLen + code->parityLen);

	//fill packet with message chunk?

	//encode packet - buffered
	encode(packet, encodedPacket, code);

	//printMatrix(packet);
	//printMatrix(encodedPacket);

	printf("Generator\n");
	printAugMatrix(code->generator, code->wordLen - 1);
	printf("\n\n");
	printf("Control\n");
	printAugMatrix(code->control, code->control->cols - code->control->rows - 1);
	printf("\n\n");

	transmit(encodedPacket, errorProb);

	// decode
	//fprintf(stderr, "end testCode\n"); //FIXME

	//extract packet - aka go from vector back to bytes
}


void initCodeStats(CodeStats* codeStats)
{

}


void transmit(Matrix* packet, double errorProb)
{
	int i;
	int n = packet->rows * packet->cols;
	for(i = 0; i < n; i++)
	{
		if(rand() % 1 <= errorProb)
		{
			packet->data[i] ^= 1;
		}
	}
}


void loadMessage(char* filePath, Message* msg)
{
	FILE* fh = fopen(filePath, "rb");
	if(fh)
	{
		/* Get File Size */
		fseek(fh, 0, SEEK_END);
		long fileSize = ftell(fh);
		fseek(fh, 0, SEEK_SET);

		char* buffer = (char*)malloc(fileSize);

		if(buffer)
		{
			long result = fread(buffer, sizeof(char), fileSize, fh);

			if(result == fileSize)
			{
				msg->len = fileSize;
				msg->byteOffset = 0;
				msg->bitOffset = 0;
				msg->data = buffer;
			}
			else
			{
				fprintf(stderr, "error: only read %ld of %ld bytes in 'loadMessage'\n", result, fileSize );
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			fprintf(stderr, "error: malloc failed in 'loadMessage'\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "error: failed to open '%s' in 'loadMessage'\n", filePath);
		exit(EXIT_FAILURE);
	}

	fclose(fh);
}
