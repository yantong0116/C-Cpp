#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <math.h>

using namespace std;
using namespace cv;

int main()
{
    // Load original images
    const char* lennaPath = "lenna.jpg";
    Mat lennaImage = imread(lennaPath, IMREAD_GRAYSCALE);

    const char* pepperPath = "peppernoise.png";
    Mat pepperNoiseImage = imread(pepperPath, IMREAD_GRAYSCALE);

    // Result matrices
    Mat averageBlur(lennaImage.rows, lennaImage.cols, CV_8UC1);
    Mat difference(lennaImage.rows, lennaImage.cols, CV_8UC1);
    Mat medianFilter(pepperNoiseImage.rows, pepperNoiseImage.cols, CV_8UC1);
    Mat gaussianBlur(lennaImage.rows, lennaImage.cols, CV_8UC1);
    Mat gaussianBlur2(lennaImage.rows, lennaImage.cols, CV_8UC1);
    Mat gaussianBlur3(lennaImage.rows, lennaImage.cols, CV_8UC1);

    int kernelSize = 3;
    int bellSize = 5;
    int gaussianKernel3x3[3][3] = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };
    int gaussianKernel5x5[5][5] = {
        { 1, 4, 7, 4, 1 },
        { 4, 16, 26, 16, 4 },
        { 7, 26, 41, 26, 7 },
        { 4, 16, 26, 16, 4 },
        { 1, 4, 7, 4, 1 }
    };

    // Average blur
    for (int y = 0; y < lennaImage.rows; y++)
    {
        for (int x = 0; x < lennaImage.cols; x++)
        {
            int sum = 0;
            int average = 0;

            for (int a = -(kernelSize / 2); a <= kernelSize / 2; a++)
            {
                for (int b = -(kernelSize / 2); b <= kernelSize / 2; b++)
                {
                    // Add pixel intensities within the kernel
                    if ((y + a) >= 0 && (y + a) < lennaImage.rows && (x + b) >= 0 && (x + b) < lennaImage.cols)
                    {
                        sum += lennaImage.at<uchar>(y + a, x + b);
                    }
                }
            }

            // Calculate average
            average = sum / (kernelSize * kernelSize);

            // Assign average to corresponding pixel in the blur matrix
            averageBlur.at<uchar>(y, x) = average;
        }
    }

    // Difference of images
    absdiff(lennaImage, averageBlur, difference);

    int threshold = 10;
    for (int y = 0; y < difference.rows; y++)
    {
        for (int x = 0; x < difference.cols; x++)
        {
            if (difference.at<uchar>(y, x) <= threshold)
            {
                difference.at<uchar>(y, x) = 0;
            }
            else
            {
                difference.at<uchar>(y, x) = 255;
            }
        }
    }

    // Median filter
    for (int y = 0; y < pepperNoiseImage.rows; y++)
    {
        for (int x = 0; x < pepperNoiseImage.cols; x++)
        {
            vector<uchar> pixels;
            for (int a = -(kernelSize / 2); a <= kernelSize / 2; a++)
            {
                for (int b = -(kernelSize / 2); b <= kernelSize / 2; b++)
                {
                    // Add pixels to vector
                    if ((y + a) >= 0 && (y + a) < pepperNoiseImage.rows && (x + b) >= 0 && (x + b) < pepperNoiseImage.cols)
                    {
                        pixels.push_back(pepperNoiseImage.at<uchar>(y + a, x + b));
                    }
                }
            }

            // Sort the vector
            sort(pixels.begin(), pixels.end());

            // Assign median to corresponding pixel in the median filter matrix
            medianFilter.at<uchar>(y, x) = pixels[pixels.size() / 2];
        }
    }

    // Gaussian blur 3x3
    for (int y = 0; y < lennaImage.rows; y++)
    {
        for (int x = 0; x < lennaImage.cols; x++)
        {
            int sum = 0;
            for (int a = -(kernelSize / 2); a <= kernelSize / 2; a++)
            {
                for (int b = -(kernelSize / 2); b <= kernelSize / 2; b++)
                {
                    // Add weighted pixel intensities within the kernel
                    if ((y + a) >= 0 && (y + a) < lennaImage.rows && (x + b) >= 0 && (x + b) < lennaImage.cols)
                    {
                        sum += lennaImage.at<uchar>(y + a, x + b) * gaussianKernel3x3[a + (kernelSize / 2)][b + (kernelSize / 2)];
                    }
                }
            }

            // Calculate average
            int average = sum / 16;

            // Assign average to corresponding pixel in the Gaussian blur matrix
            gaussianBlur.at<uchar>(y, x) = average;
        }
    }

    // Gaussian blur 5x5
    for (int y = 0; y < lennaImage.rows; y++)
    {
        for (int x = 0; x < lennaImage.cols; x++)
        {
            int sum = 0;
            for (int a = -(bellSize / 2); a <= bellSize / 2; a++)
            {
                for (int b = -(bellSize / 2); b <= bellSize / 2; b++)
                {
                    // Add weighted pixel intensities within the kernel
                    if ((y + a) >= 0 && (y + a) < lennaImage.rows && (x + b) >= 0 && (x + b) < lennaImage.cols)
                    {
                        sum += lennaImage.at<uchar>(y + a, x + b) * gaussianKernel5x5[a + (bellSize / 2)][b + (bellSize / 2)];
                    }
                }
            }

            // Calculate average
            int average = sum / 273;

            // Assign average to corresponding pixel in the Gaussian blur matrix
            gaussianBlur2.at<uchar>(y, x) = average;
        }
    }

    // Gaussian blur using custom kernel
    int kernelSize2 = 25;
    int centerX = kernelSize2 / 2;
    int centerY = kernelSize2 / 2;
    float sigma = 3.0;
    float pi = 3.1416;
    float gaussianKernel[25][25] = { 0 };
    float sumFilter = 0;

    for (int i = 0; i < kernelSize2; i++)
    {
        for (int j = 0; j < kernelSize2; j++)
        {
            int deltaX = i - centerX;
            int deltaY = centerY - j;
            float distanceSquared = (deltaX * deltaX) + (deltaY * deltaY);
            float exponent = -distanceSquared / (2 * sigma * sigma);
            float constant = 1.0 / (sigma * sigma * 2 * pi);
            gaussianKernel[i][j] = constant * exp(exponent);
            sumFilter += gaussianKernel[i][j];
        }
    }

    for (int y = 0; y < lennaImage.rows; y++)
    {
        for (int x = 0; x < lennaImage.cols; x++)
        {
            float sum = 0;
            for (int a = -(kernelSize2 / 2); a <= kernelSize2 / 2; a++)
            {
                for (int b = -(kernelSize2 / 2); b <= kernelSize2 / 2; b++)
                {
                    // Add weighted pixel intensities within the kernel
                    if ((y + a) >= 0 && (y + a) < lennaImage.rows && (x + b) >= 0 && (x + b) < lennaImage.cols)
                    {
                        sum += lennaImage.at<uchar>(y + a, x + b) * gaussianKernel[a + (kernelSize2 / 2)][b + (kernelSize2 / 2)];
                    }
                }
            }

            // Calculate average
            int average = sum / sumFilter;

            // Assign average to corresponding pixel in the Gaussian blur matrix
            gaussianBlur3.at<uchar>(y, x) = average;
        }
    }

    // Display images
    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original image", lennaImage);

    namedWindow("Average Blur", WINDOW_AUTOSIZE);
    imshow("Average Blur", averageBlur);

    namedWindow("Difference", WINDOW_AUTOSIZE);
    imshow("Difference", difference);

    namedWindow("Median Filter", WINDOW_AUTOSIZE);
    imshow("Median Filter", medianFilter);

    namedWindow("Pepper Noise", WINDOW_AUTOSIZE);
    imshow("Pepper Noise", pepperNoiseImage);

    namedWindow("Gaussian Blur 3x3", WINDOW_AUTOSIZE);
    imshow("Gaussian Blur 3x3", gaussianBlur);

    namedWindow("Gaussian Blur 5x5", WINDOW_AUTOSIZE);
    imshow("Gaussian Blur 5x5", gaussianBlur2);

    namedWindow("Custom Gaussian Blur", WINDOW_AUTOSIZE);
    imshow("Custom Gaussian Blur", gaussianBlur3);

    waitKey(0);
}
