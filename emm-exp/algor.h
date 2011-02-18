/*
 * algor.h
 *
 * author:		Taylor Baldwin
 * date:		02/17/2011
 * description:	testing different parity check equations
 * 				to determine 'best' based on hamming distance
 */

#ifndef ALGOR_H_
#define ALGOR_H_

#include "../emm-core/matrix.h"

void chooseRandom(matrix*,int);
void chooseEven(matrix*,int);
void chooseMajority(matrix*,int);

int getMinDistance(matrix*);


#endif /* ALGOR_H_ */
