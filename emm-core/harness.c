/*
 * harness.c
 *
 *  Created on: Feb 22, 2011
 *      Author: James Bettke
 */
#include <stdlib.h>
#include "harness.h"



void runHarness(harnessOptions* hop)
{
	clock_t startHarness = clock();
	CodeStats stats;
	Message* msg = readMessage(hop->msgPath);
	Message* decodedMsg = copyMessage(msg);
	char pathBuffer[1024];

	if(hop->offset >= msg->len)
	{
		fprintf(stderr, "error: offset cannot be greater than or equal to message length in 'runHarness'\n");
		exit(EXIT_FAILURE);
	}

	/* initialize for for writing otherwise set file descriptor to stdout */
	FILE* fh;
	if(!hop->outPath)
	{
		fh = stdout;
		/* Write start record array for JSON */
		fprintf(fh, "[");
	}

	/* Run test ranges */
	int w, p, pType;
	for(w = hop->wordLen[0]; w <= hop->wordLen[1]; w++)
	{
		for(p = hop->parityLen[0]; p <= hop->parityLen[1]; p++)
		{
			for(pType = 1; pType <= PARITY_FLAG_MAX; pType <<= 1 )
			{
				if(pType & hop->parityFlags || pType == CUSTOM)
				{

					/* Create code */
					clock_t startSetup = clock();
					Code* code = newCode(w, p, pType, hop->customMatrixPath);
					clock_t endSetup = clock();

					if(hop->outPath)
					{
						/* Open output file for writing */
						sprintf(pathBuffer, "%s/%d-%d-%d-%02d.json", hop->outPath, code->wordLen, code->parityLen, pType, (int)(hop->errorProb*100));

						fh = fopen(pathBuffer, "w");
						if(!fh)
						{
							fprintf(stderr, "error: failed to open '%s' for writing 'runHarness'\n", pathBuffer);
							exit(EXIT_FAILURE);
						}
					}

					/* Clear stats struct and probabiltiy*/
					initCodeStats(&stats);
					stats.errorProb = hop->errorProb;


					/* Run the code through encode/decode tests */
					clock_t startCodeExec = clock();
					testCode(code, msg, decodedMsg, hop->offset, &stats);
					clock_t endCodeExec = clock();

					/* write decoded message to file */
					sprintf(pathBuffer, "%s/%d-%d-%d-%02d%s", hop->outPath, code->wordLen, code->parityLen, pType, (int)(hop->errorProb*100), strrchr(hop->msgPath,'.'));
					saveMessage(decodedMsg, pathBuffer);

					/* save execution measures in stats struct */
					stats.setupTime = getExecTime(endSetup,startSetup);
					stats.codeExecTime = getExecTime(endCodeExec, startCodeExec);

					/* write JSON output to output steam */
					exportResults(code, &stats, fh);
					delCode(&code);

					/* Close file if we're not using stdout */
					if(hop->outPath)
					{
						fclose(fh);
					}
					else
					{
						/* stdout*/
						fprintf(fh, ",");
					}
				}
			} /* parity type */
		} /* parity */
	} /* word */

	/* Delete message buffers now that we're done with them */
	delMessage(&msg);
	delMessage(&decodedMsg);
	clock_t endHarness = clock();

	/* stdout */
	if(!hop->outPath)
	{
		fprintf(fh, "{\"" HARNESS_EXEC_TIME "\":%ld}", getExecTime(endHarness, startHarness));
		fprintf(fh, "]");
	}

}


void testCode(Code* code, Message* msg, Message* decodedMsg, int offset, CodeStats* stats)
{
	/* Allocate space for packet buffers*/
	Matrix* packet = newMatrix(1, code->wordLen);						    /* Must be 1 x W Matrix */
	Matrix* encodedPacket = newMatrix(1, code->wordLen + code->parityLen);  /* Must be 1 x (W + P) Matrix */
	Matrix* receivedPacket = newMatrix(code->wordLen + code->parityLen, 1); /* Must be (W + P) x 1Matrix */
	Matrix* syndromeIndexBuffer = newMatrix(code->parityLen, 1); 		    /* Must be P x 1 Matrix */
	Matrix* decodedPacket = newMatrix(code->wordLen, 1);				    /* Must be W x 1 Matrix */

	/* Reset message byte offset */
	msg->byteOffset = offset;
	msg->bitOffset = 0;

	/* Reset decoded message byte offset */
	decodedMsg->byteOffset = offset;
	decodedMsg->bitOffset = 0;

	while(nextPacket(msg, packet))
	{
		//fprintf(stderr,"start encode\n");

	//	fprintf(stdout, "Packet:\n");
	//	printMatrix(packet);

		encode(packet, encodedPacket, code);

	//	fprintf(stdout, "Encoded Packet:\n");
	//	printMatrix(encodedPacket);

		copyMatrix(encodedPacket, receivedPacket);

	//	fprintf(stdout, "Received Packet b4 trasnmit:\n");
	//	printMatrix(receivedPacket);

		//fprintf(stderr,"end encode\n");
		transmit(receivedPacket, stats->errorProb);
		//fprintf(stderr,"start decode\n");

	//fprintf(stdout, "Received encoded Packet:\n");
	//	printMatrix(receivedPacket);

		decode(receivedPacket, syndromeIndexBuffer, decodedPacket, code);

		//fprintf(stdout, "Decoded Packet:\n");
	//	printMatrix(decodedPacket);

		//fprintf(stderr,"end decode\n");
		detectErrors(packet, encodedPacket, receivedPacket, decodedPacket, code, stats);

		packetToMessage(decodedPacket, decodedMsg);

		stats->packets++;
	}

	delMatrix(&packet);
	delMatrix(&encodedPacket);
	delMatrix(&receivedPacket);
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
	int n = packetBuffer->rows * packetBuffer->cols;
	for(i = 0; i < n; i++)
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


void transmit(Matrix* encodedPacket, double errorProb)
{
	int i;
	int prob = errorProb * 100;
	int n = encodedPacket->rows * encodedPacket->cols;
	for(i = 0; i < n; i++)
	{
		if( rand() % 100 <= prob)
		{
			//printf("flip: %d\n", i);
			encodedPacket->data[i] ^= 1;
		}
	}
}

void detectErrors(Matrix* packet, Matrix* encodedPacket, Matrix* receivedPacket, Matrix* decodedPacket, Code* code, CodeStats* stats)
{
	int diffPD = 0;
	int diffER = 0;
	int wordLen = packet->rows * packet->cols;
	int encodedLen = encodedPacket->rows * encodedPacket->cols;

	/* Pseudo vertical vector N x 1*/
	Matrix r;
	r.rows = code->control->rows;
	r.cols = 1;
	char d[r.rows];
	r.data = (char*)d;
	Matrix* result = &r;

	int i;
	for(i = 0; i < wordLen; i++)
	{
		if(packet->data[i] != decodedPacket->data[i])
			diffPD++;
	}

	for(i = 0; i < encodedLen; i++)
	{
		if(encodedPacket->data[i] != receivedPacket->data[i])
			diffER++;
	}

	/* we only care about packets corrupted, uncorrupted packets are irrelevant */
	if(diffER != 0)
	{
		/*
		if the received pack is a valid word (ie control * received == 0 )
		and we know that encoded and received are different in here
		then the error must have been undetected
		*/
		bufferedBinaryMultiply(code->control, receivedPacket, result);

		if(vectorAsInt(result) == 0)
		{
			stats->undetectedErrors++;
		}
		else /* otherwise the error has at least been detected */
		{
			/*
			 *here the original and the decoded are the same
			 *but we know that encoded and received are different in here
			 *therefore we must have successfully decoded
			 */
			if(diffPD == 0)
			{
				stats->successfulDecodes++;
			}
			else
			{
				/* otherwise we have at least detected an error
				 * even though we were not able to recover properly from it
				 * this will happen if it decodes improperly
				 */
				stats->detectedErrors++;
			}
		}
	}

}


int packetToMessage(Matrix* packetBuffer, Message* msg)
{
	int run = 1;
	int shift;
	int i;
	int n = packetBuffer->rows * packetBuffer->cols;
	for(i = 0; i < n; i++)
	{
		if(msg->byteOffset < msg->len)
		{
			shift = (7 - msg->bitOffset);
			msg->data[msg->byteOffset] = msg->data[msg->byteOffset] & (0xFF ^ (1 << shift));
			msg->data[msg->byteOffset] = msg->data[msg->byteOffset] | (packetBuffer->data[i] << shift);

			msg->bitOffset++;
			if(msg->bitOffset >= 8)
			{
				msg->bitOffset = 0;
				msg->byteOffset++;
			}
		}
		else
		{
			run = 0;
		}
	}

	return run;
}



void exportResults(Code* code, CodeStats* stats, FILE* fh)
{
	/* setup JSON format string */
	char* jStats  = "{\"n\":%d,\"k\":%d,\"d\":%d,\"" PARITY_TYPE "\":%d,\"" ERROR_PROB "\":%.2f,\"" PACKETS "\":%d,\"" SUCCESSFUL_DECODES "\":%d,\""
					UNDETECTED_ERRORS "\":%d,\"" DETECTED_ERRORS "\":%d,\"" SETUP_TIME "\":%d,\"" CODE_EXEC_TIME "\":%d";

	char* jMatrices= "\"" GENERATOR "\":\"%s\",\"" CONTROL"\":\"%s\",\"" SYNDROME "\":\"%s\"";

	/* Writes statistics portion to file stream */
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

	/* close JSON string  */
	fprintf(fh, "}");
}
