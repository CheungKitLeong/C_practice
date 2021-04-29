#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define Baseline 30.0
#define Focal_Length 100
#define Image_Width 35.0
#define Image_Height 35.0
#define Resolution_Row 512
#define Resolution_Col 512
#define View_Grid_Row 9
#define View_Grid_Col 9

struct Point3d
{
	double x;
	double y;
	double z;
	Point3d(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}
};

struct Point2d
{
	double x;
	double y;
	Point2d(double x_, double y_) :x(x_), y(y_) {}
};

double bilinear(double alpha, double beta, double value1, double value2, double value3, double value4){
	double p1 = (1 - alpha) * value1 + alpha * value2;
	double p2 = (1 - alpha) * value3 + alpha * value4;
	return (1 - beta) * p1 + beta * p2;
}

void resize(int lw, int lh, Bitmap* bmp){
	float winc = (float)bmp->getWidth()/(float)lw;
	float hinc = (float)bmp->getHeight()/(float)lh;
	unsigned char** reR = (unsigned char **)malloc(lh* sizeof(unsigned char*));
	unsigned char** reG = (unsigned char **)malloc(lh* sizeof(unsigned char*));
	unsigned char** reB = (unsigned char **)malloc(lh* sizeof(unsigned char*));
	float sw;
	float sh = 0; 
	int starth, endh, startw, endw;
	int sumr, sumg, sumb;
	int bcount;
	for(int i=0; i<lh; i++){
		reR[i] = (unsigned char *)malloc(lw* sizeof(unsigned char));
		reG[i] = (unsigned char *)malloc(lw* sizeof(unsigned char));
		reB[i] = (unsigned char *)malloc(lw* sizeof(unsigned char));
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
						bmp->getColor(jj, ii, reR[i][j], reG[i][j], reB[i][j]);
						sumr += reR[i][j];
						sumg += reG[i][j];
						sumb += reB[i][j];
						bcount++;
				}
			}
			reR[i][j] = (unsigned char)(((float)sumr / (float)bcount) +0.5);
			reG[i][j] = (unsigned char)(((float)sumg / (float)bcount) +0.5);
			reB[i][j] = (unsigned char)(((float)sumb / (float)bcount) +0.5);
			sw += winc;
		}
		sh += hinc;
	}
	bmp->create(lw, lh);
	for(int i =0; i<lh; i++){
		for(int j=0; j<lw; j++){
			bmp->setColor(j, i, reR[i][j],reG[i][j],reB[i][j]);
		}
	}
	return;
}

int main(int argc, char** argv)
{
	if(argc < 5 || argc > 6)
	{
		cout << "Arguments prompt: viewSynthesis.exe <LF_dir> <X Y Z> OR: viewSynthesis.exe <LF_dir> <X Y Z> <focal_length>" << endl;
		return 0;
	}
	string LFDir = argv[1];
	double Vx = stod(argv[2]), Vy = stod(argv[3]), Vz = stod(argv[4]);
	double targetFocalLen = 100;
	if(argc == 6)
	{
		targetFocalLen = stod(argv[5]);
	}
	

	vector<Bitmap> viewImageList;
	//! loading light field views
	for (int i = 0; i < View_Grid_Col*View_Grid_Row; i++)
	{
		char name[128];
		sprintf(name, "/cam%03d.bmp", i+1);
		string filePath = LFDir + name;
		Bitmap view_i(filePath.c_str());
		viewImageList.push_back(view_i);
	}

	Bitmap targetView(Resolution_Col, Resolution_Row);
	cout << "Synthesizing image from viewpoint: (" << Vx << "," << Vy << "," << Vz << ") with focal length: " << targetFocalLen << endl;
	//! resample pixels of the target view one by one
	for (int r = 0; r < Resolution_Row; r++)
	{
		for (int c = 0; c < Resolution_Col; c++)
		{
			Point3d rayRGB(0, 0, 0);
			//! resample the pixel value of this ray: TODO
			double vpx = Image_Width * ((double)c / Resolution_Col - 0.5 );
			double vpy = Image_Height * ((double)r / Resolution_Row - 0.5 );
			vpx = Vx + Vz * vpx / Focal_Length;
			vpy = Vy + Vz * vpy / Focal_Length;
			// Basic part
			if((double)abs(vpx)<=120 && (double)abs(vpy)<=120){
				double alpha = vpx / Baseline;
				double u = floor(alpha);
				alpha -= u;
				u += 4;
				double beta = -vpy / Baseline;
				double v = floor(beta);
				beta -= v;
				v += 4;
				Color value1 = {0, 0, 0};
				if(v>=0 && v<View_Grid_Row && u>=0 && u<View_Grid_Col){
					viewImageList.at(v * 9 + u).getColor(c, r, value1.R, value1.G, value1.B);
				}
				Color value2 = {0, 0, 0};
				if(v>=0 && v<View_Grid_Row && u+1>=0 && u+1<View_Grid_Col){			
					viewImageList.at(v * 9 + u + 1).getColor(c, r, value2.R, value2.G, value2.B);
				}
				Color value3 = {0, 0, 0};
				if(v+1>=0 && v+1<View_Grid_Row && u>=0 && u<View_Grid_Col){
					viewImageList.at((v + 1) * 9 + u).getColor(c, r, value3.R, value3.G, value3.B);
				}
				Color value4 = {0, 0, 0};
				if(v+1>=0 && v+1<View_Grid_Row && u+1>=0 && u+1<View_Grid_Col){
					viewImageList.at((v + 1) * 9 + u + 1).getColor(c, r, value4.R, value4.G, value4.B);
				}
				rayRGB.x = bilinear(alpha, beta, (double) value1.R, (double) value2.R, (double) value3.R, (double) value4.R);
				rayRGB.y = bilinear(alpha, beta, (double) value1.G, (double) value2.G, (double) value3.G, (double) value4.G);
				rayRGB.z = bilinear(alpha, beta, (double) value1.B, (double) value2.B, (double) value3.B, (double) value4.B);
			}
			//! record the resampled pixel value
			targetView.setColor(c, r, (unsigned char) rayRGB.x, (unsigned char) rayRGB.y, (unsigned char) rayRGB.z);
		}
	}
	string savePath = "newView.bmp";
	// Consider the targetfocalLen
	if (targetFocalLen == Focal_Length){
		targetView.save(savePath.c_str());
	}else{
		int lw = (int) ((double) Resolution_Col * targetFocalLen / Focal_Length);
		int lh = (int) ((double) Resolution_Row * targetFocalLen / Focal_Length);
		Bitmap newtargetView(Resolution_Col, Resolution_Row);
		resize(lw, lh, &targetView);
		if(targetFocalLen < Focal_Length){
			int cc; 
			int rr = 0;
			for (int r = 0; r < (int) Resolution_Row; r++){
				if (r < (Resolution_Row - lh) / 2){
					continue;
				}
				cc = 0;
				for(int c = 0; c < (int) Resolution_Col; c++){
					if (c < (Resolution_Col - lw) / 2){
						continue;
					}
					unsigned char rval, gval, bval;
					targetView.getColor(cc, rr, rval, gval, bval);
					newtargetView.setColor(c, r, rval, gval, bval);
					cc++;
					if(cc == lw){
						break;
					}
				}
				rr++;
				if(rr == lh){
					break;
				}
			}
		}
		else{
			int startc = (lw - Resolution_Col) / 2;
			int startr = (lh - Resolution_Row) / 2;
			for (int r = 0; r < (int) Resolution_Row; r++){
				for(int c = 0; c < (int) Resolution_Col; c++){
					unsigned char rval, gval, bval;
					targetView.getColor(startc + c, startr + r, rval, gval, bval);
					newtargetView.setColor(c, r, rval, gval, bval);
				}
			}
		}
		newtargetView.save(savePath.c_str());
	}
	cout << "Result saved!" << endl;
	return 0;
}