/*
 * harness.c
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */
#include <stdlib.h>
#include "harness.h"
#include <stdio.h> /* LoadMessage / exportJson */




void runHarness(int* wordLen, int* parityLen, double errorProb, int parityFlags,  char* msgPath, char* outPath)
{

	CodeStats stats;
	Message* msg = readMessage(msgPath);

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
					testCode(code, msg, errorProb, &stats);
					exportResults(code, &stats, outPath);
					delCode(&code);
				}
			}
		}
	}

	delMessage(&msg);
}


void testCode(Code* code, Message* msg, double errorProb, CodeStats* stats)
{
	/* Allocate space for packet buffers*/
	Matrix* packet = newMatrix(1, code->wordLen);
	Matrix* encodedPacket = newMatrix(1, code->wordLen + code->parityLen);
	Matrix* decodedPacket = newMatrix(1, code->wordLen);

	initCodeStats(stats);
	stats->errorProb = errorProb;

	while(nextPacket(msg, packet))
	{
		encode(packet, encodedPacket, code);
		transmit(encodedPacket, errorProb);
		decode(packet, decodedPacket, code);
		detectErrors(packet, decodedPacket, stats);
	}

	//extract packet - aka go from vector back to bytes

	delMatrix(&packet);
	delMatrix(&encodedPacket);
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
	int n = packet->rows * packet->cols;
	for(i = 0; i < n; i++)
	{
		if(rand() % 1 <= errorProb)
		{
			packet->data[i] ^= 1;
		}
	}
}


void detectErrors(Matrix* packet, Matrix* decodedPacket, CodeStats* stats)
{

}


void exportResults(Code* code, CodeStats* stats, char* filePath)
{

	FILE* fh;

	if(filePath)
	{
		fh = fopen(filePath, "w");
	}
	else
	{
		fh = stdout;
	}

	/* setup JSON format string */
	char* jsonCode  = "%s:{n:%d,k:%d,d:%d," PARITY_TYPE ":%d," GENERATOR ":\"%s\"," CONTROL":\"%s\"," SYNDROME ":\"%s\"}";
	char* jsonStats = "%s:{" ERROR_PROB ":%f," PACKETS ":%d," SUCCESSFUL_DECODES ":%d," UNDETECTED_ERRORS ":%d," DETECTED_ERRORS ":%d,"
					  SETUP_TIME ":%d," CODE_EXEC_TIME ":%d}";

	/* Allocate matrices as strings */
	char* gen = matrixToString(code->generator);
	char* con = matrixToString(code->control);
	char* syn = matrixToString(code->syndrome);

	fprintf(fh,"{");
	fprintf(fh,jsonCode,  "code", code->wordLen, code->wordLen+code->parityLen, code->distance, code->parityType , gen, con, syn);
	fprintf(fh,",");
	fprintf(fh,jsonStats, "stats", stats->errorProb, stats->packets, stats->successfulDecodes, stats->undetectedErrors,
						  stats->detectedErrors, stats->setupTime, stats->codeExecTime);
	fprintf(fh,"}");

	/* Deallocate matrix strings */
	free(gen);
	free(con);
	free(syn);

	if(filePath)
	{
		fclose(fh);
	}
}
