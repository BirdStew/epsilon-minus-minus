/*
 * algor.c
 *
 * author:		Taylor Baldwin
 * date:		02/17/2011
 * description:	testing different parity check equations
 * 				to determine 'best' based on hamming distance
 */

#include "algor.h"
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>

#define N 4
#define K 8


#ifndef MAIN_
#define MAIN_
int main(int argc, char **argv){

	srand(clock());

	/* asdf * /
	matrix *ident = newMatrix(N,N);
	matrix *pce = newMatrix(N,K-N);
	matrix *gen;
	/ *setIdentity(ident); */

	char words[3][8] = {
			{1, 1, 1, 1, 1, 1, 1, 1},
			{0, 0, 1, 1, 0, 0, 1, 1},
			{0, 0, 0, 0, 0, 0, 0, 0}
		};

	int test = getMinDistance(words,3);
	if(test != 4){ printf("fuck you min distance shithead.  Got: %d\n",test); }

	scanf("%d",&test);
	return EXIT_SUCCESS;
}
#endif /* MAIN_ */

void chooseRandom(matrix *A,int asdf){

}
void chooseEven(matrix *A,int asdf){

}
void chooseMajority(matrix *A,int asdf){

}
int getMinDistance(matrix *B){
	int i,j,k,min=length,temp;
	for(i=0;i<B->rows;i++){
		for(j=i+1;j<(B->cols)-1;j++){
			temp = 0;
			for(k = 0; k < B->cols; k++){
				if(B->data[i*(b->cols)+k]==B->)
			}
		}
		min = (temp < min)? temp : min;
	}
	return min;
}
