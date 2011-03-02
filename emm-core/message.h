/*
 * message.h
 *
 *  Created on: Mar 2, 2011
 *      Author: James Betke
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

typedef struct Message
{
	long len;
	int byteOffset;
	int bitOffset;
	char* data;
} Message;


Message* newMessage(char* filePath);
void delMessage(Message** m);

#endif /* MESSAGE_H_ */
