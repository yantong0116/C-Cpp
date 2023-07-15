#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include<math.h>

using namespace std;
using namespace cv;

int main()
{
	// Original images
	const char* lenna = "lenna.jpg";
	Mat image = imread(lenna, IMREAD_GRAYSCALE);
	const char* pepper = "peppernoise.png";
	Mat pepperNoise = imread(pepper, IMREAD_GRAYSCALE);

	// Result
	Mat blur(image.rows, image.cols, CV_8UC1);
	Mat difference(image.rows, image.cols, CV_8UC1);
	Mat median_filter(pepperNoise.rows, pepperNoise.cols, CV_8UC1);
	Mat gaussian(image.rows, image.cols, CV_8UC1);
	Mat gaussian2(image.rows, image.cols, CV_8UC1);
	Mat gaussian3(image.rows, image.cols, CV_8UC1);

	int innerMatrixIndex = 3;
	int tamCampana = 5;
	int gaussMatrix[3][3] = { { 1,2,1 },{ 2,4,2 },{ 1,2,1 } };
	int gaussMatrix2[5][5] = { { 1,4,7,4,1 },{ 4,16,26,16,4 },{ 7,26,41,26,7 },{ 4,16,26,16,4 },{ 1,4,7,4,1 } };
	float sum, average;
	float gauss[25][25] = { 0 };

	// Average blur
    // This code performs an average blur operation on the image matrix using a 3x3 kernel. 
    // It calculates the average value of the pixel intensities within the kernel and assigns the result to the corresponding pixel in the blur matrix.
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			sum = 0;
			average = 0;
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					// Add
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						sum += image.at<uchar>(i + a, j + b);
					}
				}
			}
			// Average
			average = float(sum / float(innerMatrixIndex*innerMatrixIndex));
			// Assignment
			blur.at<uchar>(i, j) = average;
		}
	}

	// Subtraction of images
    // This code calculates the absolute difference between the corresponding pixels of the image and blur matrices and stores the result in the difference matrix.
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			difference.at<uchar>(i, j) = abs(image.at<uchar>(i, j) - blur.at<uchar>(i, j));
		}
	}
    int limit = 10; // Threshold
	for (int i = 0; i < difference.rows; i++)
	{
		for (int j = 0; j < difference.cols; j++)
		{
			if (difference.at<uchar>(i, j) <= limit)
			{
				difference.at<uchar>(i, j) = 0;
			}
			else {
				difference.at<uchar>(i, j) = 255;
			}
		}
		cout << endl;
	}

	// Median filter
	float median = 0;
	vector<float> myVector;
	for (int i = 0; i < pepperNoise.rows; i++)
	{
		for (int j = 0; j < pepperNoise.cols; j++)
		{
			median = 0;
			myVector.clear();
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					// Add them to vector
					if ((i + a) >= 0 &&
						(i + a) < pepperNoise.rows &&
						(j + b) >= 0 &&
						(j + b) < pepperNoise.cols)
					{
						myVector.push_back(pepperNoise.at<uchar>(i + a, j + b));
					}
				}
			}
			sort(myVector.begin(), myVector.end());
			median = myVector.at(myVector.size() / 2);
			// Assignment
			median_filter.at<uchar>(i, j) = median;
		}
	}

	// Gaussian blur
	int sum2 = 0;
	int average2 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			sum2 = 0;
			average2 = 0;

			int y = 0;
			for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
			{
				int x = 0;
				for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
				{
					// Addition
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						sum2 += int(image.at<uchar>(i + a, j + b)*gaussMatrix[y][x]);
					}
					x++;
				}
				y++;
			}

			// Average
			average2 = int(sum2 / 16);
			// Assignment
			gaussian.at<uchar>(i, j) = average2;
		}
	}

	// Gaussian blur 5x5
	int sum3 = 0;
	int average3 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			sum3 = 0;
			average3 = 0;
			int y1 = 0;
			for (int a = -(tamCampana / 2); a <= tamCampana / 2; a++)
			{
				int x1 = 0;
				for (int b = -(tamCampana / 2); b <= tamCampana / 2; b++)
				{
					// Addition
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						sum3 += int(image.at<uchar>(i + a, j + b)*gaussMatrix2[y1][x1]);
					}
					x1++;
				}
				y1++;
			}
			// Average
			average3 = int(sum3 / 273);
			// Assignment
			gaussian2.at<uchar>(i, j) = average3;
		}
	}

	int x0 = 25 / 2;
	int y0 = 25 / 2;
	int sigma = 3;
	float pi = 3.1416;
	float sumaFiltro = 0;
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 25; j++) {
			int cX = i - x0;
			int cY = y0 - j;
			float up = (cX*cX) + (cY*cY);
			float down = 2 * (sigma * sigma);
			float exp1 = exp(-(up) / (down));
			float constant = 1.0 / (sigma*sigma * 2 * pi);
			gauss[i][j] = constant* exp1;
			sumaFiltro += constant * exp1;
		}
	}
	
	float sum4 = 0;
	int average4 = 0;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			sum4 = 0;
			average4 = 0;
			int y4 = 0;
			for (int a = -(25 / 2); a <= 25 / 2; a++)
			{
				int x4 = 0;
				for (int b = -(25 / 2); b <= 25 / 2; b++)
				{
					// Addition
					if ((i + a) >= 0 &&
						(i + a) < image.rows &&
						(j + b) >= 0 &&
						(j + b) < image.cols)
					{
						sum4 += int(image.at<uchar>(i + a, j + b))*gauss[y4][x4];
					}
					x4++;
				}
				y4++;
			}
			// Average
			average4 = int(sum4 / sumaFiltro);
			// Assignment
			gaussian3.at<uchar>(i, j) = average4;
		}
	}

	namedWindow("Original image", WINDOW_AUTOSIZE);
	imshow("Original image", image);
	namedWindow("Blur", WINDOW_AUTOSIZE);
	imshow("Blur", blur);
	namedWindow("Difference", WINDOW_AUTOSIZE);
	imshow("Difference", difference);
	namedWindow("Median filter", WINDOW_AUTOSIZE);
	imshow("Median filter", median_filter);
	namedWindow("Pepper noise", WINDOW_AUTOSIZE);
	imshow("Pepper noise", pepperNoise);

	namedWindow("gaussian", WINDOW_AUTOSIZE);
	imshow("gaussian", gaussian);
	namedWindow("gaussian2", WINDOW_AUTOSIZE);
	imshow("gaussian2", gaussian2);
	namedWindow("gaussian3", WINDOW_AUTOSIZE);
	imshow("gaussian3", gaussian3);
	waitKey(0);
}