#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

int main()
{
    const char* lennaPath = "lenna.jpg";
    Mat originalImage = imread(lennaPath, IMREAD_GRAYSCALE);

    const char* pepperPath = "peppernoise.png";
    Mat pepperNoise = imread(pepperPath, IMREAD_GRAYSCALE);

    Mat blurredImage(originalImage.rows, originalImage.cols, CV_8UC1);
    Mat difference(originalImage.rows, originalImage.cols, CV_8UC1);
    Mat medianFilter(pepperNoise.rows, pepperNoise.cols, CV_8UC1);
    Mat sobelHorizontal(originalImage.rows, originalImage.cols, CV_32FC1);
    Mat sobelVertical(originalImage.rows, originalImage.cols, CV_32FC1);
    Mat rootImage(originalImage.rows, originalImage.cols, CV_32FC1);
    Mat gaussianFiltered2(originalImage.rows, originalImage.cols, CV_8UC1);
    Mat gaussianFiltered3(originalImage.rows, originalImage.cols, CV_8UC1);

    int innerMatrixSize = 3;
    int kernelSize = 5;
    int horizontalKernel[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
    int verticalKernel[3][3] = { { -1, -2, -1 }, { 0, 0, 0 }, { 1, 2, 1 } };

    float gaussianKernel[25][25] = { 0 };
    int gaussianKernelSize = 15;
    int xCenter = gaussianKernelSize / 2;
    int yCenter = gaussianKernelSize / 2;
    int sigma = 3;
    float pi = 3.1416;
    float sumOfGaussianKernel = 0;

    for (int i = 0; i < gaussianKernelSize; i++) {
        for (int j = 0; j < gaussianKernelSize; j++) {
            int xDistance = i - xCenter;
            int yDistance = yCenter - j;
            float exponent = exp(-(xDistance * xDistance + yDistance * yDistance) / (2 * sigma * sigma));
            float constant = 1.0 / (sigma * sigma * 2 * pi);
            gaussianKernel[i][j] = constant * exponent;
            sumOfGaussianKernel += constant * exponent;
        }
    }

    // Applying Gaussian filter
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            float sum = 0;
            for (int a = -(gaussianKernelSize / 2); a <= gaussianKernelSize / 2; a++) {
                for (int b = -(gaussianKernelSize / 2); b <= gaussianKernelSize / 2; b++) {
                    int row = i + a;
                    int col = j + b;
                    if (row >= 0 && row < originalImage.rows && col >= 0 && col < originalImage.cols) {
                        sum += originalImage.at<uchar>(row, col) * gaussianKernel[a + xCenter][b + yCenter];
                    }
                }
            }
            gaussianFiltered3.at<uchar>(i, j) = static_cast<uchar>(sum / sumOfGaussianKernel);
        }
    }

    // Calculating sharp changes (difference)
    Mat sharpChanges(originalImage.rows, originalImage.cols, CV_32FC1, Scalar(0));
    Mat accentuatedImage(originalImage.rows, originalImage.cols, CV_8UC1, Scalar(0));
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            sharpChanges.at<float>(i, j) = originalImage.at<uchar>(i, j) - gaussianFiltered3.at<uchar>(i, j);
        }
    }

    // Accentuating the image
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            int pixelValue = originalImage.at<uchar>(i, j) + sharpChanges.at<float>(i, j);
            accentuatedImage.at<uchar>(i, j) = (pixelValue > 255) ? 255 : static_cast<uchar>(pixelValue);
        }
    }

    // Sobel horizontal
    int sumH = 0;
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            sumH = 0;
            int y = 0;
            for (int a = -(innerMatrixSize / 2); a <= innerMatrixSize / 2; a++) {
                int x = 0;
                for (int b = -(innerMatrixSize / 2); b <= innerMatrixSize / 2; b++) {
                    if (i + a >= 0 && i + a < originalImage.rows && j + b >= 0 && j + b < originalImage.cols) {
                        sumH += originalImage.at<uchar>(i + a, j + b) * horizontalKernel[y][x];
                    }
                    x++;
                }
                y++;
            }
            sobelHorizontal.at<float>(i, j) = sumH / 255.0;
        }
    }

    // Sobel vertical
    int sumV = 0;
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            sumV = 0;
            int y = 0;
            for (int a = -(innerMatrixSize / 2); a <= innerMatrixSize / 2; a++) {
                int x = 0;
                for (int b = -(innerMatrixSize / 2); b <= innerMatrixSize / 2; b++) {
                    if (i + a >= 0 && i + a < originalImage.rows && j + b >= 0 && j + b < originalImage.cols) {
                        sumV += originalImage.at<uchar>(i + a, j + b) * verticalKernel[y][x];
                    }
                    x++;
                }
                y++;
            }
            sobelVertical.at<float>(i, j) = sumV / 255.0;
        }
    }

    // Vector summation
    for (int i = 0; i < originalImage.rows; i++) {
        for (int j = 0; j < originalImage.cols; j++) {
            float xValue = sobelHorizontal.at<float>(i, j) * sobelHorizontal.at<float>(i, j);
            float yValue = sobelVertical.at<float>(i, j) * sobelVertical.at<float>(i, j);
            rootImage.at<float>(i, j) = sqrt(xValue + yValue);
        }
    }

    namedWindow("Original Image", WINDOW_AUTOSIZE);
    imshow("Original Image", originalImage);

    namedWindow("Gaussian Filtered (Sigma=3)", WINDOW_AUTOSIZE);
    imshow("Gaussian Filtered (Sigma=3)", gaussianFiltered3);

    namedWindow("Sharp Changes", WINDOW_AUTOSIZE);
    imshow("Sharp Changes", sharpChanges);

    namedWindow("Accentuated Image", WINDOW_AUTOSIZE);
    imshow("Accentuated Image", accentuatedImage);

    namedWindow("Sobel Horizontal", WINDOW_AUTOSIZE);
    imshow("Sobel Horizontal", sobelHorizontal);

    namedWindow("Sobel Vertical", WINDOW_AUTOSIZE);
    imshow("Sobel Vertical", sobelVertical);

    namedWindow("Root Image", WINDOW_AUTOSIZE);
    imshow("Root Image", rootImage);

    waitKey(0);
}
