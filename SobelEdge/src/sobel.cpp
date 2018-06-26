/*
 * sobel.cpp
 *
 *  Created on: May 26, 2018
 *      Author: spratik
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>
#include <fstream>
using namespace std;
using namespace cv;

int kernalX[3][3] = {1,0,-1,
					2,0,-2,
					1,0,-1};

int kernalY[3][3] = {1,2,1,
					0,0,0,
					-1,-2,-1};


void writePGM(cv :: Mat image, int *outputBuffer)
{
	ofstream pgmFile("Soble.pgm");
	if(!pgmFile.is_open()){
		cout << "File opening Issue" << endl;
		return;
	}

	uchar *data = image.data;

	pgmFile << "P2" << endl;
	pgmFile << image.cols << " " << image.rows  << endl;
	pgmFile << "255" << endl;



	for(int i = 0; i < image.cols * image.rows; i++){

		if(i % image.cols == 0){
			pgmFile << endl;
			pgmFile << outputBuffer[i] << " ";
		}else{
			pgmFile << outputBuffer[i] << " ";
		}
	}

	pgmFile.close();

}

void getSobelEdge(Mat &image)
{

	uchar *data = image.data;
	int pixelBuffer[image.rows * image.cols];
	int outputBuffer[image.rows * image.cols];

	for(int i = 0; i < image.rows * image.cols; i++){
		pixelBuffer[i] =  (int)data[i];
		outputBuffer[i] =  (int)data[i];
	}

//	cout << image.rows * image.cols << endl;
//	cout <<image.cols << endl;
//	cout << image.rows << endl;



	for(int r = 1; r < image.rows - 2; r++){

		for(int c = 1; c < image.cols - 2; c++){

			int TL = c + r *3;
			int TP = (c + 1) + r *image.cols;
			int TR = (c + 2) + r *image.cols;

			int ML =  c 	 + (r+1) *image.cols;
			int MD = (c + 1) + (r+1) *image.cols;
			int MR = (c + 2) + (r+1) *image.cols;

			int BL =  c 	 + (r+2) *image.cols;
			int BD = (c + 1) + (r+2) *image.cols;
			int BR = (c + 2) + (r+2) *image.cols;

//			cout << "(" << TL << "," << TP << "," << TR << endl;
//			cout << "(" << ML << "," << MD << "," << MR << endl;
//			cout << "(" << BL << "," << BD << "," << BR << endl;

			int MagX = pixelBuffer[TL] * kernalX[0][0] + pixelBuffer[TP] * kernalX[0][1] + pixelBuffer[TR] * kernalX[0][2] +
					   pixelBuffer[ML] * kernalX[1][0] + pixelBuffer[MD] * kernalX[1][1] + pixelBuffer[MR] * kernalX[1][2] +
				       pixelBuffer[BL] * kernalX[2][0] + pixelBuffer[BD] * kernalX[2][1] + pixelBuffer[BR] * kernalX[2][2];

			int MagY = pixelBuffer[TL] * kernalY[0][0] + pixelBuffer[TP] * kernalY[0][1] + pixelBuffer[TR] * kernalY[0][2] +
					   pixelBuffer[ML] * kernalY[1][0] + pixelBuffer[MD] * kernalY[1][1] + pixelBuffer[MR] * kernalY[1][2] +
					   pixelBuffer[BL] * kernalY[2][0] + pixelBuffer[BD] * kernalY[2][1] + pixelBuffer[BR] * kernalY[2][2];


			//cout << MagX << " " << MagY << endl;

			int squreRoot = sqrt( (pow(MagX,2) + pow(MagY,2)));

			//cout << squreRoot << endl;

			if(squreRoot >= 255){
				outputBuffer[MD] = 255;
			}else{
				outputBuffer[MD] = squreRoot;
			}
		}

	}

	writePGM(image,outputBuffer);
}


int main(int argc, char **argv) {

	if(argc != 2){
		cout << "Usgae : Sobel Input_image" << endl;
		return 0;
	}

	Mat image = imread(argv[1],0);
	if(!image.data){
		cout << "Image read Error" << endl;
		return 0;
	}
	getSobelEdge(image);



}
