#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"
#include <algorithm>
#include <vector>

using namespace std;
using namespace cv;

int factor = 5;
Mat flowers = imread("flower.jpg", IMREAD_GRAYSCALE);
Mat average_result(flowers.rows, flowers.cols, CV_8UC1, Scalar(0));
Mat meidan_result(flowers.rows, flowers.cols, CV_8UC1, Scalar(0));
Mat border_result(flowers.rows, flowers.cols, CV_8UC1, Scalar(0));

int average(int r, int c) {
	int value = 0;
	for (int i = r - int(factor / 2); i < (r + int(factor / 2) + 1); i++) {
		for (int j = c - int(factor / 2); j < (c + int(factor / 2)+1); j++) {
			if (i < 0 or i >= flowers.rows or j < 0 or j >= flowers.cols) {
				continue;
			}
			else {
				value += int(flowers.at<uchar>(i, j));
			}
		}
	}
	int res = 0;
	res = int(value / (factor*factor));
	return res;
}

int median(int r, int c) {
	int value = 0;
	vector<int> v;
	int res = 0;
	int index = 0;
	for (int i = r - int(factor / 2); i < (r + int(factor / 2) + 1); i++) {
		for (int j = c - int(factor / 2); j < (c + int(factor / 2) + 1); j++) {
			if (i < 0 or i >= flowers.rows or j < 0 or j >= flowers.cols) {
				continue;
			}
			else {
				v.push_back(flowers.at<uchar>(i, j));
			}
		}
	}
	sort(v.begin(), v.end());
	index = (int)(v.size() / 2);
	res = v[index];
	return res;
}

int main() {
	//----------------------------------------------------- Average filter
	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			average_result.at<uchar>(i, j) = (uchar)(average(i, j));
		}
	}

	//----------------------------------------------------- Border
	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			border_result.at<uchar>(i, j) = abs(flowers.at<uchar>(i, j) - average_result.at<uchar>(i, j));
		}
	}

	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			if (border_result.at<uchar>(i, j) >= 25) {
				border_result.at<uchar>(i, j) = 255;
			}
			else {
				border_result.at<uchar>(i, j) = 0;
			}
		}
	}
	
	//----------------------------------------------------- Median
	for (int i = 0; i < flowers.rows; i++) {
		for (int j = 0; j < flowers.cols; j++) {
			meidan_result.at<uchar>(i, j) = (uchar)(median(i, j));
		}
	}

	namedWindow("Original", WINDOW_AUTOSIZE);
	imshow("Original", flowers);

	namedWindow("Average", WINDOW_AUTOSIZE);
	imshow("Average", average_result);

	namedWindow("Border", WINDOW_AUTOSIZE);
	imshow("Border", border_result);

	namedWindow("Median", WINDOW_AUTOSIZE);
	imshow("Median", meidan_result);
	waitKey(0);
}