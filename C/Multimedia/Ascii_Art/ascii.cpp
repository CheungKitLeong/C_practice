/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2021

Assignment 01 Skeleton

ascii.cpp

*/

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"		//	Simple .bmp library
#include <string.h>
#include <stdlib.h>

#define MAX_SHADES 8

char shades[MAX_SHADES] = {'@','#','%','*','|','-','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	//
	//	1. Open BMP file
	//
	Bitmap image_data(argv[2]);

	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}
	if (argv[1][0] != 'p' && argv[1][0]!= 's'){
		printf("Task undefined!\n");
		return -1;
	}
	int w = image_data.getWidth();
	int h = image_data.getHeight();
	unsigned char** luma = (unsigned char **)malloc(h * sizeof(unsigned char*));
	unsigned char r; 
	unsigned char g;
	unsigned char b;
	for(int i=0; i<h; i++){
		luma[i] = (unsigned char *)malloc(w * sizeof(unsigned char));
		for (int j=0; j<w; j++){
			image_data.getColor(j, i, r,g,b);
			luma[i][j] = 0.299*r+0.587*g+0.114*b;
		}
	}
	//
	//  3. Resize image
	//
	if (argc > 3)
	{
		int lw = atoi(strtok(argv[3],","));
		int lh = atoi(strtok(NULL, " "));
		if(lh == 0){
			lh = (int)(lw * ((float)h/(float)w));
		}
		if(lw<=w && lh<=h && lw > 0 && lh > -1){
			float winc = (float)w/(float)lw;
			float hinc = (float)h/(float)lh;
			unsigned char** reluma = (unsigned char **)malloc(lh* sizeof(unsigned char*));
			float sw;
			float sh = 0; 
			int sum;
			int bcount;
			for(int i=0; i<lh; i++){
				reluma[i] = (unsigned char *)malloc(lw* sizeof(unsigned char));
				sw = 0;
				for(int j = 0; j<lw; j++){
					bcount = 0;
					sum = 0;
					for (int ii = (int)(sh+0.5); ii < (int)(sh+hinc+0.5); ii++){
						for (int jj = (int)(sw+0.5); jj < (int)(sw+winc+0.5); jj++){
							sum += luma[ii][jj];
							bcount++;
						}
					}
					reluma[i][j] = (unsigned char)(((float)sum / (float)bcount) +0.5);
					sw += winc;
				}
				sh += hinc;
			}
			w = lw;
			h = lh;
			luma = (unsigned char **)malloc(h * sizeof(unsigned char*));
			for(int i=0; i<h; i++){
				luma[i] = (unsigned char *)malloc(w * sizeof(unsigned char));
        		for (int j=0; j<w; j++){
            		luma[i][j] = reluma[i][j];
        		}
			}
			for(int i=0; i<h; i++){
				free(reluma[i]);
			}
			free (reluma);	
		}
	}

	//
	//	4. Quantization
	//
	//
	// 5. ASCII Mapping and printout
	//
	for (int i = 0; i < h; i++){
		for(int j=0; j<w; j++){
			luma[i][j] = (unsigned char)(luma[i][j] / 32);
		}
	}
	if(argv[1][0] == 's'){
		for(int i=0; i<h; i++){
			for (int j=0; j<w; j++){
				luma[i][j] = shades[7-luma[i][j]];
				printf("%c ",luma[i][j]);
			}
			printf("\n");
		}
	}
	//
	//      6. ASCII art txt file
	//
	if (argv[1][0] == 'p')
	{
		if (argc < 5){
			printf("Cannot find the output file!\n");
			return -1;
		}
		FILE* fptr = fopen(argv[4],"w");
		if (fptr == NULL){
			printf("Unable to create file.\n");
			return -1;
		}
		for(int i=0; i<h; i++){
			for (int j=0; j<w; j++){
				luma[i][j] = shades[luma[i][j]];
				fprintf(fptr, "%c ", luma[i][j]);
			}
			fprintf(fptr,"\n");
		}
		fclose(fptr);	
	}
	
	//  free memory
	for(int i=0; i<h; i++){
		free(luma[i]);
	}
	free (luma);
	return 0;
} 
