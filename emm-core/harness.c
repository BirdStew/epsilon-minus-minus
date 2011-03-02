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
	Message* msg = newMessage(msgPath);

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
					delCode(&code);
				}
			}
		}
	}

	delMessage(&msg);
}


void testCode(Code* code, Message* msg, double errorProb, CodeStats* stats)
{
	/*
	 * Need to write nextPacket function to consume message in parts.  track bits. byte boundry issue.
	 * Need to write until function to export code/stats struct to JSON
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


int nextPacket(Matrix* packetBuffer, Message* msg)
{
	int mask = 1;
	int i;
	for(i = 0; i < packetBuffer->cols; i++)
	{
		if(msg->byteOffset >= msg->len)
		{
			return 0;
		}

		packetBuffer->data[i] = (msg->data[msg->byteOffset] >> (7 - msg->bitOffset)) & mask;

		msg->bitOffset++;
		if(msg->bitOffset >= 8)
		{
			msg->bitOffset = 0;
			msg->byteOffset++;
		}
	}

	return 1;
}
