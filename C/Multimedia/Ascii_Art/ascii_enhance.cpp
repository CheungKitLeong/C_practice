/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2021

Assignment 01 Enhancement part

ascii_enhance.cpp

*/

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"		//	Simple .bmp library
#include <string.h>
#include <stdlib.h>

#define MAX_SHADES 16

char shades[MAX_SHADES] = {'@','%','W','#','m','*','p','x','+','=','-','l',':','^','.',' '};

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }


int main(int argc, char** argv)
{
	Bitmap image_data(argv[2]);

	if(image_data.getData() == NULL)
	{
		printf("unable to load bmp image!\n");
		return -1;
	}
	int w = image_data.getWidth();
	int h = image_data.getHeight();
	if (argv[1][0]=='p' || argv[1][0]=='s' || argv[1][0]=='i'){
		// grayscale ascii art
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
		// Resize image
		if (argc > 3)
		{
			int lw = atoi(strtok(argv[3],","));
			int lh = atoi(strtok(NULL, " "));
			if (lw <= 0){
				lw = w;
			}
			if(lh <= 0){
				lh = (int)(lw * ((float)h/(float)w));
			}
			float winc = (float)w/(float)lw;
			float hinc = (float)h/(float)lh;
			unsigned char** reluma = (unsigned char **)malloc(lh* sizeof(unsigned char*));
			float sw;
			float sh = 0; 
			int starth, endh, startw, endw;
			int sum;
			int bcount;
			for(int i=0; i<lh; i++){
				reluma[i] = (unsigned char *)malloc(lw* sizeof(unsigned char));
				sw = 0;
				for(int j = 0; j<lw; j++){
					bcount = 0;
					sum = 0;
					// Resize the height
					if (hinc<1){
						// Enlargement
						starth = (int)(sh+hinc);
						if ((sh*2+hinc)<(float)(starth*2)){
							starth -= 1;
						} 
						endh = starth + 1;
					}else{
						// Contraction
						starth = (int)(sh+0.5);
						endh = (int)(sh+hinc+0.5);
					}
					for (int ii = starth; ii < endh; ii++){
						// Resize the width
						if (winc<1){
							// Enlargement
							startw = (int)(sw+winc);
							if ((sw*2+winc)<(float)(startw*2)){
								startw -= 1;
							} 
							endw = startw + 1;
						}else{
							// Contration
							startw = (int)(sw+0.5);
							endw = (int)(sw+winc+0.5);
						}
						for (int jj = startw; jj < endw; jj++){
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
		// Quantization
		for (int i = 0; i < h; i++){
			for(int j=0; j<w; j++){
				luma[i][j] = (unsigned char)(luma[i][j] / 16);
			}
		}
		// Print
		if(argv[1][0] == 's'){
			for(int i=0; i<h; i++){
				for (int j=0; j<w; j++){
					printf("%c ",shades[15-luma[i][j]]);
				}
				printf("\n");
			}
		}
		//Save txt
		else if (argv[1][0] == 'p')
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
					fprintf(fptr, "%c ", shades[luma[i][j]]);
				}
				fprintf(fptr,"\n");
			}
			fclose(fptr);	
		}
		//	ascii art image
		else if (argv[1][0]=='i'){
			if (argc < 5){
				printf("Cannot find the output file!\n");
				return -1;
			}
			Bitmap shadeimgs[8];
			char path[13] = "shades/0.bmp";
			for(int i=0;i<8;i++){
				path[7] = i+48;
				shadeimgs[i].create(path);
			}
			Bitmap output(w*8, h*8);
			for(int i=0;i<h;i++){
				for(int j=0;j<w;j++){
					luma[i][j] = luma[i][j] / 2;
					for(int ii=0; ii<8;ii++){
						for(int jj=0;jj<8;jj++){
							shadeimgs[luma[i][j]].getColor(jj, ii, r, g, b);
							output.setColor(j*8+jj, i*8+ii, r, g, b);
						}
					}
				}
			}
			output.save(argv[4]);
		}
		//  free memory
		for(int i=0; i<h; i++){
			free(luma[i]);
		}
		free (luma);
	}
	//colored ascii art
	else if  (argv[1][0]== 'c' || argv[1][0]=='b'){
		//Resize (modify by above part)
		if (argc < 5){
			printf("Cannot find the output file!\n");
			return -1;
		}
		Bitmap output;
		if(argv[1][0]=='b'){
			output.create(w,h);
		}
		int lw = atoi(strtok(argv[3],","));
		int lh = atoi(strtok(NULL, " "));
		if (lw == 0){
			lw = w;
		}
		if(lh == 0){
			lh = (int)(lw * ((float)h/(float)w));
		}
		unsigned char **r_val = (unsigned char **)malloc(lh * sizeof(unsigned char*));
		unsigned char **g_val = (unsigned char **)malloc(lh * sizeof(unsigned char*));
		unsigned char **b_val = (unsigned char **)malloc(lh * sizeof(unsigned char*));
		float winc = (float)w/(float)lw;
		float hinc = (float)h/(float)lh;
		float sw;
		float sh = 0; 
		int starth, endh, startw, endw;
		int sumr, sumg, sumb;
		int bcount;
		for(int i=0; i<lh; i++){
			r_val[i] = (unsigned char *)malloc(lw * sizeof(unsigned char));
			g_val[i] = (unsigned char *)malloc(lw * sizeof(unsigned char));
			b_val[i] = (unsigned char *)malloc(lw * sizeof(unsigned char));
			sw = 0;
			for(int j = 0; j<lw; j++){
				bcount = 0;
				sumr = 0;
				sumg = 0;
				sumb = 0;
				// Resize the height
				if (hinc<1){
					// Enlargement
					starth = (int)(sh+hinc);
					if ((sh*2+hinc)<(float)(starth*2)){
						starth -= 1;
					} 
					endh = starth + 1;
				}else{
					// Contraction
					starth = (int)(sh+0.5);
					endh = (int)(sh+hinc+0.5);
				}
				for (int ii = starth; ii < endh; ii++){
					// Resize the width
					if (winc<1){
						// Enlargement
						startw = (int)(sw+winc);
						if ((sw*2+winc)<(float)(startw*2)){
							startw -= 1;
						} 
						endw = startw + 1;
					}else{
						// Contration
						startw = (int)(sw+0.5);
						endw = (int)(sw+winc+0.5);
					}
					for (int jj = startw; jj < endw; jj++){
						image_data.getColor(jj, ii, r_val[i][j], g_val[i][j], b_val[i][j]);
						sumr += r_val[i][j];
						sumg += g_val[i][j];
						sumb += b_val[i][j];
						bcount++;
					}
				}
				r_val[i][j] = (unsigned char)(((float)sumr / (float)bcount) +0.5);
				g_val[i][j] = (unsigned char)(((float)sumg / (float)bcount) +0.5);
				b_val[i][j] = (unsigned char)(((float)sumb / (float)bcount) +0.5);
				// Make blurred image
				if(argv[1][0]=='b'){
					for (int ii = starth; ii < endh; ii++){
						for(int jj = startw; jj < endw; jj++){
							output.setColor(jj,ii,r_val[i][j],g_val[i][j],b_val[i][j]);
						}
					}
				}
				//
				sw += winc;
			}
			sh += hinc;
		}
		// Save blurred image
		if(argv[1][0]=='b'){
			output.save(argv[4]);
		}
		// Write HTML
		else if(argv[1][0]=='c'){
			FILE* fptr = fopen(argv[4],"w");
			if (fptr == NULL){
				printf("Unable to create file.\n");
				return -1;
			}
			fprintf(fptr,"<!DOCTYPE html><html><head><title>%s</title></head><body><p style=\"font-family:'Lucida Console'\">\n", argv[2]);
			int mean;
			for(int i =0; i<lh; i++){
				for(int j = 0; j<lw; j++){
					mean = r_val[i][j]+g_val[i][j]+b_val[i][j];
					mean = (int)((float)mean / 48);
					if(shades[mean]==' '){
						fprintf(fptr, "&nbsp;&thinsp;" , r_val[i][j], g_val[i][j], b_val[i][j]);
					}else{
						fprintf(fptr, "<span style=color:#%02X%02X%02X>%c</span>&thinsp;" , r_val[i][j], g_val[i][j], b_val[i][j], shades[mean]);
					}
				}
				fprintf(fptr, "<br>\n");
			}
			fprintf(fptr,"</p></body></html>");
			fclose(fptr);
		}
		// free memory
		
		for(int i=0; i<lh; i++){
			free(r_val[i]);
			free(g_val[i]);
			free(b_val[i]);
		}
		free(r_val);
		free(g_val);
		free(b_val);
		
	}else{
		printf("Task undefined!\n");
		return -1;
	}	
	return 0;
} 
