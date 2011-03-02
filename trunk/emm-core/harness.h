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

typedef struct CodeStats
{
	int packets;
	int successfulDecodes;
	int undetectedErrors;
	int detectedErrors;
	int codeTime;			/* (endDecode - startEncode) */
} CodeStats;

typedef struct Message
{
	long len;
	int byteOffset;
	int bitOffset;
	char* data;
} Message;


void runHarness(int* wordLen, int* parityLen, double errorProb, int parityFlags, char* msgPath, char* outPath);
void testCode(Code* code, Message* msg, double errorProb, CodeStats* stats);
void initCodeStats(CodeStats* codeStats);
void transmit(Matrix* packet, double errorProb);
Message* newMessage(char* filePath);
void delMessage(Message** m);

#endif /* HARNESS_H_ */
