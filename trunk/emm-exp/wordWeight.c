/*
 * wordWeight.c
 *
 *  Created on: Feb 25, 2011
 *      Author: Taylor Baldwin
 */

#include<stdlib.h>
#include<stdio.h>
#include<math.h>

int main (int argc, char **argv){
	unsigned long length = 40;
	unsigned long iter;
	unsigned long i,j;
	unsigned long *weight;
	unsigned long *words;
	unsigned long mask;
	unsigned long bits;

	weight=(unsigned long*)malloc(pow(2,length)*sizeof(unsigned long));
	words=(unsigned long*)malloc(pow(2,length)*sizeof(unsigned l));

	for(iter = 0; iter < pow(2,length); iter++){
		bits = 0;
		for(mask = 0x01; mask <= pow(2,length); mask<<=1){
			if(mask & iter){ bits++; }
		}
		weight[iter]=bits;
	}

	i = 0;
	for(j = 0; j<=length; j++){
		for(iter = 0; iter < pow(2,length); iter++){
			if(j == weight[iter]){
				words[i]=iter;
				i++;
			}
		}
	}

	for(iter = 0; iter < pow(2,length); iter++){
		printf("%x\t",words[iter]);
		if(!(iter%10)){ printf("\n"); }
	}
	return EXIT_SUCCESS;
}

