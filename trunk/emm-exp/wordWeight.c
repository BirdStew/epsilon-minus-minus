/*
 * wordWeight.c
 *
 *  Created on: Feb 25, 2011
 *      Author: Taylor Baldwin
 */

#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"matrix.h"

//#define wordLength 12
#define encodeLength 16

Matrix* wordsByWeight(int length);
Matrix* newLowDensityParity(int rows, int cols);

int main (int argc, char **argv){

	//Matrix *asdf = newMatrix(pow(2,encodeLength),encodeLength);
	Matrix *ldpc = newLowDensityParity(8,6);

	//wordsByWeight(asdf,encodeLength);

	printMatrix(ldpc);

	return EXIT_SUCCESS;
}

Matrix* wordsByWeight(int length){
	unsigned int iter;
	unsigned int i,j,k;
	unsigned int *weight;
	unsigned int mask;
	unsigned int bits;
	Matrix *allWords = newMatrix(pow(2,length),length);

	weight=(unsigned int*)malloc((allWords->rows)*sizeof(unsigned int));

	for(iter = 0; iter < allWords->rows; iter++){
		bits = 0;
		for(mask = 0x01; mask <= allWords->rows; mask<<=1){
			if(mask & iter){ bits++; }
		}
		weight[iter]=bits;
	}

	i = 0;
	for(j = 0; j<=length; j++){
		for(iter = 0; iter < allWords->rows; iter++){
			if(j == weight[iter]){
				for(k = 0; k < (allWords->cols); k++){
					allWords->data[(i*(allWords->cols))+k] = ((0x01<<k) & iter)? 1 : 0;
				}
				i++;
			}
		}
	}

	return allWords;
}

Matrix* newLowDensityParity(int rows, int cols){
	int i,j;
	Matrix *temp = newMatrix(rows,cols);
	Matrix *valid = wordsByWeight(rows);

	for(i = 0; i < cols; i++){
		for(j = 0; j < rows; j++){
			temp->data[j*cols+i] = valid->data[(valid->rows-1-i)*rows+j];
		}
	}

	return temp;
}
