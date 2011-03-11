/*
 * message.c
 *
 *  Created on: Mar 2, 2011
 *      Author: James Bettke
 */
#include <stdio.h>
#include <stdlib.h>
#include "message.h"

Message* newMessage(long size)
{
	Message* msg = (Message*)malloc(sizeof(Message));
	if(!msg)
	{
		fprintf(stderr, "error: malloc failed for 'msg' struct in 'newMessage'\n");
		exit(EXIT_FAILURE);
	}

	char* data = (char*)malloc(size);
	if(!data)
	{
		fprintf(stderr, "error: malloc failed for 'd' in 'newMessage'\n");
		exit(EXIT_FAILURE);
	}

	msg->len = size;
	msg->byteOffset = 0;
	msg->bitOffset = 0;
	msg->data = data;

	return msg;
}


void delMessage(Message** msg)
{
	if(*msg == NULL || msg == NULL)
	{
		fprintf(stderr,"error: passed ptr is NULL in 'delMessage'\n");
	}
	else
	{
		free((*msg)->data);
		free(*msg);
		*msg = NULL;
	}

}


Message* readMessage(char* filePath)
{
	Message* msg;
	FILE* fh = fopen(filePath, "rb");
	if(fh)
	{
		/* Get File Size */
		fseek(fh, 0, SEEK_END);
		long fileSize = ftell(fh);
		fseek(fh, 0, SEEK_SET);

		msg = newMessage(fileSize);

		long result = fread(msg->data, sizeof(char), msg->len, fh);

		if(result != fileSize)
		{
			fprintf(stderr, "error: only read %ld of %ld bytes in 'readMessage'\n", result, fileSize);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "error: failed to open '%s' in 'readMessage'\n", filePath);
		exit(EXIT_FAILURE);
	}

	fclose(fh);
	return msg;
}


void saveMessage(Message* msg, char* filePath)
{
	FILE* fh = fopen(filePath, "wb");
	if(fh)
	{
		long result = fwrite(msg->data, sizeof(char), msg->len, fh);
		if(result != msg->len)
		{
			fprintf(stderr, "error: only wrote %ld of %ld bytes in 'saveMessage'\n", result, msg->len);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fprintf(stderr, "error: failed to open '%s' in 'saveMessage'\n", filePath);
		exit(EXIT_FAILURE);
	}
	fclose(fh);
}
