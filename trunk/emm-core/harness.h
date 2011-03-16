/*
 * harness.h
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */

#ifndef HARNESS_H_
#define HARNESS_H_

#include "matrix.h"
#include "code.h"
#include "message.h"
#include <time.h>
#include <stdio.h>
#define getExecTime(e,s)((long)((double)((e)-(s))/(double)CLOCKS_PER_SEC * 1000))

/* JSON Constants for CodeStruct */
#define PARITY_TYPE "pyt"
#define GENERATOR "gen"
#define CONTROL "con"
#define SYNDROME "syn"

/* JSON Constants for CodeStatsStruct */
#define ERROR_PROB "p"
#define PACKETS "pk"
#define SUCCESSFUL_DECODES "sd"
#define UNDETECTED_ERRORS "ue"
#define DETECTED_ERRORS "de"
#define SETUP_TIME "st"
#define CODE_EXEC_TIME "ct"

/* JSON Constants program record */
#define HARNESS_EXEC_TIME "ht"


typedef struct CodeStats
{
	double errorProb;
	int packets;
	int successfulDecodes;
	int undetectedErrors;
	int detectedErrors;
	int setupTime;	  /* endNewCode - startNewCode */
	int codeExecTime; /* (endDecode - startEncode) */
} CodeStats;

typedef struct harnessOptions
{
	int wordLen[2];
	int parityLen[2];
	double errorProb;
	int parityFlags;
	char* customMatrixPath;
	char* msgPath;
	char* outPath;
	int offset;
} harnessOptions;


void runHarness(harnessOptions* hop);
void testCode(Code* code, Message* msg, Message* decodedMsg, int offset, CodeStats* stats);
void initCodeStats(CodeStats* stats);
int nextPacket( Message* msg, Matrix* packetBuffer);
void transmit(Matrix* encodedPacket, double errorProb);
void detectErrors(Matrix* packet, Matrix* encodedPacket, Matrix* receivedPacket, Matrix* decodedPacket, Code* code, CodeStats* stats);
int packetToMessage(Matrix* packetBuffer, Message* msg);
void exportResults(Code* code, CodeStats* stats, FILE* fh);

#endif /* HARNESS_H_ */
