// Image Enhancement using Gaussian Filter and Sharpness Adjustment

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

void applyGaussianFilter(const Mat& inputImage, Mat& outputImage, int kernelSize, double sigma)
{
    int x0 = kernelSize / 2;
    int y0 = kernelSize / 2;
    float pi = 3.1416;
    float sumFilter = 0;
    float gaussianFilter[25][25] = { 0 };

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            int cX = i - x0;
            int cY = y0 - j;
            float up = (cX * cX) + (cY * cY);
            float down = 2 * (sigma * sigma);
            float exp1 = exp(-(up) / (down));
            float constant = 1.0 / (sigma * sigma * 2 * pi);
            gaussianFilter[i][j] = constant * exp1;
            sumFilter += constant * exp1;
        }
    }

    float sum = 0;
    int average = 0;

    for (int i = 0; i < inputImage.rows; i++) {
        for (int j = 0; j < inputImage.cols; j++) {
            sum = 0;
            average = 0;
            int y = 0;
            for (int a = -(kernelSize / 2); a <= kernelSize / 2; a++) {
                int x = 0;
                for (int b = -(kernelSize / 2); b <= kernelSize / 2; b++) {
                    if ((i + a) >= 0 && (i + a) < inputImage.rows && (j + b) >= 0 && (j + b) < inputImage.cols) {
                        sum += int(inputImage.at<uchar>(i + a, j + b)) * gaussianFilter[y][x];
                    }
                    x++;
                }
                y++;
            }
            average = int(sum / sumFilter);
            outputImage.at<uchar>(i, j) = average;
        }
    }
}

int main()
{
    // Load the original image
    const char* lennaPath = "lenna.jpg";
    Mat originalImage = imread(lennaPath, IMREAD_GRAYSCALE);

    // Apply Gaussian filter
    Mat gaussianImage(originalImage.rows, originalImage.cols, CV_8UC1);
    int kernelSize = 15;
    double sigma = 3;
    applyGaussianFilter(originalImage, gaussianImage, kernelSize, sigma);

    // Calculate sharpness
    Mat sharpnessDiff(originalImage.rows, originalImage.cols, CV_32FC1);
    Mat enhancedImage(originalImage.rows, originalImage.cols, CV_8UC1);
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            sharpnessDiff.at<float>(i, j) = originalImage.at<uchar>(i, j) - gaussianImage.at<uchar>(i, j);
        }
    }
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            if ((originalImage.at<uchar>(i, j) + sharpnessDiff.at<float>(i, j)) > 255) {
                enhancedImage.at<uchar>(i, j) = 255;
            }
            else {
                enhancedImage.at<uchar>(i, j) = originalImage.at<uchar>(i, j) + sharpnessDiff.at<float>(i, j);
            }
        }
    }

    // Display the images
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    imshow("Original Image", originalImage);

    namedWindow("Gaussian Filtered Image", WINDOW_AUTOSIZE);
    imshow("Gaussian Filtered Image", gaussianImage);

    namedWindow("Sharpness Difference", WINDOW_AUTOSIZE);
    imshow("Sharpness Difference", sharpnessDiff);

    namedWindow("Enhanced Image", WINDOW_AUTOSIZE);
    imshow("Enhanced Image", enhancedImage);

    waitKey(0);

    return 0;
}
