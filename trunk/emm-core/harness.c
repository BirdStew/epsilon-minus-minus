/*
 * harness.c
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */
#include <stdlib.h>
#include "harness.h"



void runHarness(int* wordLen, int* parityLen, double errorProb, int parityFlags,  char* msgPath, char* outPath)
{
	clock_t startHarness = clock();
	CodeStats stats;
	Message* msg = readMessage(msgPath);

	/* initialize for for writing otherwise set file descriptor to stdout */
	FILE* fh;
	if(outPath)
	{
		fh = fopen(outPath, "w");
		if(fh == NULL)
		{
			fprintf(stderr, "Failed to open '%s' for writing in 'runHarness'.\n",outPath);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fh = stdout;
	}


	/* Write start record array for JSON */
	fprintf(fh, "[");


	/* Run test ranges */
	int w, p, pType;
	for(w = wordLen[0]; w <= wordLen[1]; w++)
	{
		for(p = parityLen[0]; p <= parityLen[1]; p++)
		{
			for(pType = 1; pType <= PARITY_FLAG_MAX; pType <<= 1  )
			{
				if(pType & parityFlags)
				{


					clock_t startSetup = clock();
					Code* code = newCode(w, p, pType);
					clock_t endSetup = clock();


					initCodeStats(&stats);
					stats.errorProb = errorProb;

					clock_t startCodeExec = clock();
					testCode(code, msg, &stats);
					clock_t endCodeExec = clock();

					stats.setupTime = getExecTime(endSetup,startSetup);
					stats.codeExecTime = getExecTime(endCodeExec, startCodeExec);

					exportResults(code, &stats, fh);
					fprintf(fh, ",");
					delCode(&code);
				}
			}
		}
	}

	delMessage(&msg);
	clock_t endHarness = clock();

	/* Last record is program execution time */
	fprintf(fh, "{\"" HARNESS_EXEC_TIME "\":%ld}", getExecTime(endHarness, startHarness));

	/* Write end record array for JSON */
	fprintf(fh, "]");

	/* Close file if we're not using stdout */
	if(outPath)
	{
		fclose(fh);
	}
}


void testCode(Code* code, Message* msg, CodeStats* stats)
{
	/* Allocate space for packet buffers*/
	Matrix* packet = newMatrix(1, code->wordLen);						   /* Must be 1 x W Matrix */
	Matrix* encodedPacket = newMatrix(1, code->wordLen + code->parityLen); /* Must be 1 x (W + P) Matrix */
	Matrix* syndromeIndexBuffer = newMatrix(code->parityLen, 1); 		   /* Must be P x 1 Matrix */
	Matrix* decodedPacket = newMatrix(code->wordLen, 1);				   /* Must be W x 1 Matrix */

	/* Reset message byte offset */
	msg->byteOffset = 0;
	msg->bitOffset = 0;

	//printMatrix(code->generator);

	while(nextPacket(msg, packet))
	{
		//fprintf(stderr,"start encode\n");

		fprintf(stdout, "Packet:\n");
		printMatrix(packet);

		encode(packet, encodedPacket, code);

		fprintf(stdout, "Encoded Packet:\n");
		printMatrix(encodedPacket);

		//fprintf(stderr,"end encode\n");
		transmit(encodedPacket, stats->errorProb);
		//fprintf(stderr,"start decode\n");

		fprintf(stdout, "Received encoded Packet:\n");
		printMatrix(encodedPacket);

		decode(encodedPacket, syndromeIndexBuffer, decodedPacket, code);

		fprintf(stdout, "Decoded Packet:\n");
		printMatrix(decodedPacket);

		//fprintf(stderr,"end decode\n");
		detectErrors(packet, decodedPacket, stats);
		stats->packets++;
	}
	//extract packet - aka go from vector back to bytes

	delMatrix(&packet);
	delMatrix(&encodedPacket);
	delMatrix(&syndromeIndexBuffer);
	delMatrix(&decodedPacket);
}


void initCodeStats(CodeStats* stats)
{
	stats->errorProb = 0;
	stats->packets = 0;
	stats->successfulDecodes = 0;
	stats->undetectedErrors = 0;
	stats->detectedErrors = 0;
	stats->setupTime = 0;
	stats->codeExecTime = 0;
}


int nextPacket(Message* msg, Matrix* packetBuffer)
{
	int mask = 1;
	int run = 1;
	int i;
	for(i = 0; i < packetBuffer->cols; i++)
	{
		if(msg->byteOffset < msg->len)
		{
			packetBuffer->data[i] = (msg->data[msg->byteOffset] >> (7 - msg->bitOffset)) & mask;

			msg->bitOffset++;
			if(msg->bitOffset >= 8)
			{
				msg->bitOffset = 0;
				msg->byteOffset++;
			}
		}
		else
		{
			packetBuffer->data[i] = 0;
			run = 0;
		}
	}

	return run;
}


void transmit(Matrix* packet, double errorProb)
{
	int i;
	int prob = errorProb * 100;
	int n = packet->rows * packet->cols;
	for(i = 0; i < n; i++)
	{
		if( rand() % 100 <= prob)
		{
			packet->data[i] ^= 1;
		}
	}
}

//FIXME !!! is this even right? !!!
void detectErrors(Matrix* packet, Matrix* decodedPacket, CodeStats* stats)
{

	int diff = 0;
	int i;
	int n = packet->rows * packet->cols;

	for(i = 0; i < n; i++)
	{
		if(packet->data[i] != decodedPacket->data[i])
		{
			diff++;
		}
	}

	/* Decoded Successfully */
	if(diff == 0)
	{
		stats->successfulDecodes++;
	}

	/* Undetected Errors - number of bits that differ between original/decoded */
	stats->undetectedErrors += diff;
}


void exportResults(Code* code, CodeStats* stats, FILE* fh)
{
	/* setup JSON format string */
	char* jStats  = "{\"n\":%d,\"k\":%d,\"d\":%d,\"" PARITY_TYPE "\":%d,\"" ERROR_PROB "\":%f,\"" PACKETS "\":%d,\"" SUCCESSFUL_DECODES "\":%d,\""
					UNDETECTED_ERRORS "\":%d,\"" DETECTED_ERRORS "\":%d,\"" SETUP_TIME "\":%d,\"" CODE_EXEC_TIME "\":%d";

	char* jMatrices= "\"" GENERATOR "\":\"%s\",\"" CONTROL"\":\"%s\",\"" SYNDROME "\":\"%s\"";



	fprintf(fh,jStats, code->wordLen, code->wordLen+code->parityLen, code->distance, code->parityType,
					   stats->errorProb, stats->packets, stats->successfulDecodes, stats->undetectedErrors, stats->detectedErrors,
					   stats->setupTime, stats->codeExecTime);


	/* Allocate matrices as strings */
	char* gen = matrixToString(code->generator);
	char* con = matrixToString(code->control);
	char* syn = matrixToString(code->syndrome);

	fprintf(fh, ",");
	fprintf(fh, jMatrices, gen, con, syn);

	/* Deallocate matrix strings */
	free(gen);
	free(con);
	free(syn);

	fprintf(fh, "}");
}
