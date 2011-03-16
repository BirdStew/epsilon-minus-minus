/*
 * message.c
 *
 *  Created on: Mar 2, 2011
 *      Author: James Bettke
 */

#include "message.h"


/*
 * Creates a new message struct with a data section of “size” bytes.
 * The data section is filled with zeros and offsets are initialized to zero.
 * Although a simple array would have sufficed, the message structure was created
 * to manage markers in a given memory block.  Its main purpose is to house markers
 * for indexing  into an array at the bit level.
 */

Message* newMessage(long size)
{
	Message* msg = (Message*)malloc(sizeof(Message));
	if(!msg)
	{
		fprintf(stderr, "error: malloc failed for 'msg' struct in 'newMessage'\n");
		exit(EXIT_FAILURE);
	}

	char* data = (char*)calloc(size, sizeof(char));
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


/*
 * Receives a pointer to a message pointer and safely frees the message
 * struct from the heap. The passed pointer is then dereferenced and
 * set to null to prevent users from reading unmanaged memory.
 */

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


/*
 * Creates a new message struct and fills the data section with
 * the contents of the file specified by “filePath.”
 */

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


/*
 * Writes the contents of a message struct to an output file
 * specified by 'filePath.”  The current byte and bit offsets
 * of the message do not impact writing.
 */

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


/*
 * Makes an exact copy of a message struct and returns a pointer
 * to the newly created struct. This includes the contents of the
 * data section, length, byte, and bit offsets.
 */

Message* copyMessage(Message* msg)
{
	Message* copy = newMessage(msg->len);
	copy->byteOffset = msg->byteOffset;
	copy->bitOffset = msg->bitOffset;
	memcpy(copy->data, msg->data, msg->len);
	return copy;
}

