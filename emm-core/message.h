/*
 * message.h
 *
 *  Created on: Mar 2, 2011
 *      Author: James Betke
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Message
{
	long len;
	int byteOffset;
	int bitOffset;
	char* data;
} Message;


Message* newMessage(long size);
void delMessage(Message** msg);
Message* readMessage(char* filePath);
void saveMessage(Message* msg, char* filePath);
Message* copyMessage(Message* msg);

#endif /* MESSAGE_H_ */
