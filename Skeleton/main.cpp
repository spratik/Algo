#include <iostream>
#include "zhangsuen.h"
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int size;


void getBinaryImage(int *hist,vector<uchar> &table)
{


	/// intilize the total variable with size
	long long int total = size;

	/// intilize sum to zero
	long long int sum   = 0;

	/// intilize q1 to zero
	long long int q1    = 0;

	/// intilize q2 to zero
	long long int q2    = 0;

	/// intilize SUM to zero
	float SUM           = 0;

	/// intilize u1 to zero
	float u1            = 0;

	/// intilize u2 to zero
	float u2            = 0;

	/// intilize result to zero
	float result        = 0;

	/// intilize var_max to zero
	float var_max       = 0;

	/// intitlize threshold to zero
	int threshold       = 0;



	/// auxiliary value for computing u2
	for(int i = 0; i < 256 ;i++)
		sum = sum + i * hist[i];

	for(int t = 0; t < 256 ; t++)
	{	///update qi(t)
		q1 = q1 + hist[t];
		/// according to the qi(t) ,get the qi(t) value
		q2 = total - q1;

		/// update ui(t)
		SUM  = SUM + t * hist[t];

		u1 = SUM / q1 ;

		u2 = (sum - SUM) / q2;

		/// update the between-class variance
		result =  q1 * q2 * (u1 - u2) * (u1 - u2);

		/// update the threshold
		if(result > var_max)
		{
			threshold = t;
			var_max   = result;
		}


	}
	cout << threshold << endl;

	/// build the segmented image
	for(int i = 0; i < size; i++)
	{

		if((int)table[i] > threshold){
			table[i] = 0;
		}else{
			table[i] = 1;
		}

	}



}
/*
void writePBM(int r, int c, vector<uchar>image)
{
	ofstream pgmFile("BinaryImage.pbm");

	pgmFile << "P1" << endl;
	pgmFile << c << " " << r  << endl;
	pgmFile << "255" << endl;

	for(int i = 0; i < r * c; i++){

		if(i % c == 0){
			pgmFile << endl;
			pgmFile << (int)image[i] << " ";
		}else{
			pgmFile << (int)image[i] << " ";
		}
	}

	pgmFile.close();

}
void writePBM(Mat image)
{
	ofstream pgmFile("BinaryImage.pbm");
	uchar *data = image.data;

	pgmFile << "P1" << endl;
	pgmFile << image.cols << " " << image.rows  << endl;
	pgmFile << "255" << endl;

	for(int i = 0; i < image.cols * image.rows; i++){

		if(i % image.cols == 0){
			pgmFile << endl;
			pgmFile << (int)data[i] << " ";
		}else{
			pgmFile << (int)data[i] << " ";
		}
	}

	pgmFile.close();

}
*/

int *histogram(vector<uchar>table)
{
	static int hist[256];
	for(int i = 0; i < size; i++) {

		hist[(int)table[i]]++;
	}
	return hist;

}

int main(int argc, char **argv)
{
	if(argc != 3){
		cout << "USAGE : EXE INPUT_IMAGE OUTPUT" << endl;
		return 0;
	}

	vector <uchar> table;
	Mat image1=imread(argv[1],0);
	if(!image1.data){
		cout << "Imread error" << endl;
		return 0;
	}
	size = image1.rows * image1.cols;
	uchar *p = image1.data;

	for(int i=0;i< image1.rows * image1.cols ;i++)
	{
		table.push_back(*p++);
	}

	int *hist=histogram(table);
	getBinaryImage(hist,table);
	//writePBM(image1.rows ,image1.cols, table);

	Mat byteImage = Mat(image1.rows,  image1.cols, CV_8UC1, table.data());
	//writePBM(byteImage);

	thin(byteImage,argv[2], true,true,true);

}
