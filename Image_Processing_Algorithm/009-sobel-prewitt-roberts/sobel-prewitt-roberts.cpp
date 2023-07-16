#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

void applyFilter(const Mat& inputImage, Mat& outputImage, const Mat& kernel)
{
    int kernelSize = kernel.rows;
    int innerMatrixIndex = kernelSize;
    int sum = 0;

    for (int i = 0; i < inputImage.rows; i++)
    {
        for (int j = 0; j < inputImage.cols; j++)
        {
            sum = 0;

            int y = 0;
            for (int a = -(innerMatrixIndex / 2); a <= innerMatrixIndex / 2; a++)
            {
                int x = 0;
                for (int b = -(innerMatrixIndex / 2); b <= innerMatrixIndex / 2; b++)
                {
                    if ((i + a) >= 0 && (i + a) < inputImage.rows && (j + b) >= 0 && (j + b) < inputImage.cols)
                    {
                        sum += inputImage.at<uchar>(i + a, j + b) * kernel.at<int>(y, x);
                    }
                    x++;
                }
                y++;
            }

            outputImage.at<float>(i, j) = abs(sum);
        }
    }
}

void normalizeImage(const Mat& inputImage, Mat& outputImage)
{
    double minValue, maxValue;
    minMaxLoc(inputImage, &minValue, &maxValue);

    for (int i = 0; i < inputImage.rows; i++)
    {
        for (int j = 0; j < inputImage.cols; j++)
        {
            float normalizedValue = (255 * (inputImage.at<float>(i, j) - minValue)) / (maxValue - minValue);
            outputImage.at<uchar>(i, j) = abs(normalizedValue);
        }
    }
}

void calculateGradientMagnitude(const Mat& sobelHorizontal, const Mat& sobelVertical, Mat& gradientMagnitude)
{
    for (int i = 0; i < sobelHorizontal.rows; i++)
    {
        for (int j = 0; j < sobelHorizontal.cols; j++)
        {
            float xValue = pow(sobelHorizontal.at<float>(i, j), 2);
            float yValue = pow(sobelVertical.at<float>(i, j), 2);
            gradientMagnitude.at<float>(i, j) = sqrt(xValue + yValue);
        }
    }
}

int main()
{
    // Load the original image
    const char* lennaPath = "lenna.jpg";
    Mat image = imread(lennaPath, IMREAD_GRAYSCALE);

    // Result matrices for Sobel operator
    Mat sobelHorizontal(image.rows, image.cols, CV_32FC1);
    Mat sobelVertical(image.rows, image.cols, CV_32FC1);
    Mat gradientMagnitude(image.rows, image.cols, CV_32FC1);

    // Sobel kernel matrices
    Mat sobelHorizontalKernel = (Mat_<int>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat sobelVerticalKernel = (Mat_<int>(3, 3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);

    // Apply Sobel operator for horizontal and vertical gradients
    applyFilter(image, sobelHorizontal, sobelHorizontalKernel);
    applyFilter(image, sobelVertical, sobelVerticalKernel);

    // Calculate gradient magnitude for Sobel operator
    calculateGradientMagnitude(sobelHorizontal, sobelVertical, gradientMagnitude);

    // Normalize the Sobel operator images
    Mat normalizedSobelHorizontal(image.rows, image.cols, CV_8UC1);
    Mat normalizedSobelVertical(image.rows, image.cols, CV_8UC1);
    Mat normalizedGradientMagnitude(image.rows, image.cols, CV_8UC1);

    normalizeImage(sobelHorizontal, normalizedSobelHorizontal);
    normalizeImage(sobelVertical, normalizedSobelVertical);
    normalizeImage(gradientMagnitude, normalizedGradientMagnitude);

    // Result matrices for Prewitt operator
    Mat prewittHorizontal(image.rows, image.cols, CV_32FC1);
    Mat prewittVertical(image.rows, image.cols, CV_32FC1);
    Mat prewittGradientMagnitude(image.rows, image.cols, CV_32FC1);

    // Prewitt kernel matrices
    Mat prewittHorizontalKernel = (Mat_<int>(3, 3) << -1, 0, 1, -1, 0, 1, -1, 0, 1);
    Mat prewittVerticalKernel = (Mat_<int>(3, 3) << -1, -1, -1, 0, 0, 0, 1, 1, 1);

    // Apply Prewitt operator for horizontal and vertical gradients
    applyFilter(image, prewittHorizontal, prewittHorizontalKernel);
    applyFilter(image, prewittVertical, prewittVerticalKernel);

    // Calculate gradient magnitude for Prewitt operator
    calculateGradientMagnitude(prewittHorizontal, prewittVertical, prewittGradientMagnitude);

    // Normalize the Prewitt operator images
    Mat normalizedPrewittHorizontal(image.rows, image.cols, CV_8UC1);
    Mat normalizedPrewittVertical(image.rows, image.cols, CV_8UC1);
    Mat normalizedPrewittGradientMagnitude(image.rows, image.cols, CV_8UC1);

    normalizeImage(prewittHorizontal, normalizedPrewittHorizontal);
    normalizeImage(prewittVertical, normalizedPrewittVertical);
    normalizeImage(prewittGradientMagnitude, normalizedPrewittGradientMagnitude);

    // Result matrices for Roberts operator
    Mat robertsHorizontal(image.rows, image.cols, CV_32FC1);
    Mat robertsVertical(image.rows, image.cols, CV_32FC1);
    Mat robertsGradientMagnitude(image.rows, image.cols, CV_32FC1);

    // Apply Roberts operator for horizontal and vertical gradients
    for (int i = 1; i < image.rows - 1; i++)
    {
        for (int j = 1; j < image.cols - 1; j++)
        {
            float resH = -1 * image.at<uchar>(i, j) + image.at<uchar>(i + 1, j + 1);
            float resV = -1 * image.at<uchar>(i, j + 1) + image.at<uchar>(i + 1, j);
            robertsHorizontal.at<float>(i, j) = abs(resH);
            robertsVertical.at<float>(i, j) = abs(resV);
        }
    }

    // Calculate gradient magnitude for Roberts operator
    calculateGradientMagnitude(robertsHorizontal, robertsVertical, robertsGradientMagnitude);

    // Normalize the Roberts operator images
    Mat normalizedRobertsHorizontal(image.rows, image.cols, CV_8UC1);
    Mat normalizedRobertsVertical(image.rows, image.cols, CV_8UC1);
    Mat normalizedRobertsGradientMagnitude(image.rows, image.cols, CV_8UC1);

    normalizeImage(robertsHorizontal, normalizedRobertsHorizontal);
    normalizeImage(robertsVertical, normalizedRobertsVertical);
    normalizeImage(robertsGradientMagnitude, normalizedRobertsGradientMagnitude);

    // Display the images
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", image);

    namedWindow("Sobel Horizontal", WINDOW_AUTOSIZE);
    imshow("Sobel Horizontal", normalizedSobelHorizontal);

    namedWindow("Sobel Vertical", WINDOW_AUTOSIZE);
    imshow("Sobel Vertical", normalizedSobelVertical);

    namedWindow("Sobel Gradient Magnitude", WINDOW_AUTOSIZE);
    imshow("Sobel Gradient Magnitude", normalizedGradientMagnitude);

    namedWindow("Prewitt Horizontal", WINDOW_AUTOSIZE);
    imshow("Prewitt Horizontal", normalizedPrewittHorizontal);

    namedWindow("Prewitt Vertical", WINDOW_AUTOSIZE);
    imshow("Prewitt Vertical", normalizedPrewittVertical);

    namedWindow("Prewitt Gradient Magnitude", WINDOW_AUTOSIZE);
    imshow("Prewitt Gradient Magnitude", normalizedPrewittGradientMagnitude);

    namedWindow("Roberts Horizontal", WINDOW_AUTOSIZE);
    imshow("Roberts Horizontal", normalizedRobertsHorizontal);

    namedWindow("Roberts Vertical", WINDOW_AUTOSIZE);
    imshow("Roberts Vertical", normalizedRobertsVertical);

    namedWindow("Roberts Gradient Magnitude", WINDOW_AUTOSIZE);
    imshow("Roberts Gradient Magnitude", normalizedRobertsGradientMagnitude);

    waitKey(0);

    return 0;
}
