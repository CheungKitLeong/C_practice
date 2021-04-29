#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

#define IMG_H 256
#define IMG_W 256
#define BLOCKSIZE 8

using namespace std;
#define PI 3.14159265358979

int QuantizationMatrix[8][8] = {
	{3, 5, 7, 9, 11, 13, 15, 17}, 
	{5, 7, 9, 11, 13, 15, 17, 19},
	{7, 9, 11, 13, 15, 17, 19, 21},
	{9, 11,	13,	15,	17,	19,	21,	23},
	{11, 13, 15, 17, 19, 21, 23, 25},
	{13, 15, 17, 19, 21, 23, 25, 27},
	{15, 17, 19, 21, 23, 25, 27, 29},
	{17, 19, 21, 23, 25, 27, 29, 31}
	};
/*
int QuantizationMatrix[8][8] = {
	{1, 2, 3, 4, 5, 6, 7, 8}, 
	{2, 3, 4, 5, 6, 7, 8, 9},
	{3, 4, 5, 6, 7, 8, 9, 10},
	{4, 5, 6, 7, 8, 9, 10, 11},
	{5, 6, 7, 8, 9, 10, 11, 12},
	{6, 7, 8, 9, 10, 11, 12, 13},
	{7, 8, 9, 10, 11, 12, 13, 14},
	{8, 9, 10, 11, 12, 13, 14, 15}
	};
*/

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Arguments prompt: dct.exe <img_path> <apply_idct>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_idct = stoi(argv[2]);

	//! read input image
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;

	// Error message
	if(rows != IMG_H || cols != IMG_W){
		cout << "Input size must be ("<<IMG_H<<", "<<IMG_W<<")." << endl;
		exit(1);
	}

	//! preprocess by shifting pixel values by 128
	//TODO
	int imgpix[IMG_H][IMG_W];
	for(int i = 0; i < rows; i++){
		for(int j = 0; j< cols; j++){
			unsigned char pixel;
			s_img.getPixel(j, i, pixel);
			imgpix[i][j] = (int)pixel - 128;
		}
	}

	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	// The quantized coefficients should be stored into 'coeffArray'
	double coeffArray[256][256]={0};
	int blockRow = rows / 8, blockCol = cols / 8;
	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			int xpos = j*8, ypos = i*8;
			//! apply DCT on block_ij (basic requirement)
			//TODO
			double fr[BLOCKSIZE][BLOCKSIZE] = {0};
			for(int v =0; v<8; v++){
				for(int u=0; u<8; u++){
					for(int x=0; x<8; x++){
						fr[v][u] += cos((2*x+1)*u*PI/16) * (double)imgpix[ypos+v][xpos+x];
					}
					if(u){
						fr[v][u] *= 0.5;
					}else{
						fr[v][u] *= 0.5 / sqrt(2);
					}
				}
			}
			for(int v =0; v<8; v++){
				for(int u=0; u<8; u++){
					for(int y=0; y<8; y++){
						coeffArray[v+ypos][u+xpos] += cos((2*y+1)*v*PI/16) * fr[y][u];
					}
					if(v){
						coeffArray[v+ypos][u+xpos] *= 0.5;
					}else{
						coeffArray[v+ypos][u+xpos] *= 0.5 / sqrt(2);
					}
				}
			}			
			//! quantize the frequency coefficient of this block (basic requirement)
			//TODO
			for(int v =0; v<8; v++){
				for(int u=0; u<8; u++){
					coeffArray[v+ypos][u+xpos] = (int)(coeffArray[v+ypos][u+xpos] / QuantizationMatrix[v][u] + 0.5);
				}
			}			
		}
	}
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[r][c]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
	cout << "Quantized coefficients saved!" << endl;

	if (need_idct)
	{
		Bitmap reconstructedImg(cols, rows);
		//! apply IDCT on the quantized coefficients (enhancement part)
		for (int i = 0; i < blockRow; i++)
		{
			for (int j = 0; j < blockCol; j++)
			{
				int xpos = j*8, ypos = i*8;
				//! apply de-quantization on block_ij 
				//TODO
				for(int v =0; v<8; v++){
					for(int u=0; u<8; u++){
						coeffArray[v+ypos][u+xpos] *= QuantizationMatrix[v][u];
					}
				}	
				//! apply IDCT on this block
				//TODO
				double fr[BLOCKSIZE][BLOCKSIZE] = {0};
				double reimgpix[BLOCKSIZE][BLOCKSIZE] = {0};
				for(int y =0; y<8; y++){
					for(int x=0; x<8; x++){
						fr[y][x] = coeffArray[ypos][xpos+x] / sqrt(2);
						for(int v=1; v<8; v++){
							fr[y][x] += cos((2*y+1)*v*PI/16) * coeffArray[ypos+v][xpos+x];
						}
						fr[y][x] *= 0.5;
					}
				}	
				for(int y =0; y<8; y++){
					for(int x=0; x<8; x++){
						reimgpix[y][x] = fr[y][0] / sqrt(2);
						for(int u=1; u<8; u++){
							reimgpix[y][x] += cos((2*x+1)*u*PI/16) * fr[y][u];
						}
					reimgpix[y][x] *= 0.5;
					//! shiftting back the pixel value range to 0~255
					//TODO
					reimgpix[y][x] = (int)(reimgpix[y][x]+128 +0.5);
					reconstructedImg.setPixel(xpos+x, ypos+y, (unsigned char) reimgpix[y][x]);
					}
				}
			}
		}
		string savePath = "reconstructedImg.bmp";
		reconstructedImg.save(savePath.c_str());
		cout << "reconstructed image saved!" << endl;
	}

	return 0;
}