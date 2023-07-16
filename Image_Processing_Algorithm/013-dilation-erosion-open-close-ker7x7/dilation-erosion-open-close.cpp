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

Mat image = imread("lenna.png", IMREAD_GRAYSCALE);
Mat rDilation(image.rows, image.cols, CV_8UC1, Scalar(0));

int structuringElement[7][7] = {
    { 0, 0, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1, 1, 1 },
    { 0, 0, 1, 1, 1, 0, 0 },
    { 0, 0, 1, 1, 1, 0, 0 }
};

int innerMatrixSize = 7;

bool erosion(int row, int col, Mat binary) {
    int y = 0;
    for (int i = row - int(innerMatrixSize / 2); i <= row + int(innerMatrixSize / 2); i++) {
        int x = 0;
        for (int j = col - int(innerMatrixSize / 2); j <= col + int(innerMatrixSize / 2); j++) {
            if (i < 0 or i >= binary.rows or j < 0 or j >= binary.cols) {
                continue;
            }
            else {
                if (structuringElement[y][x] == 1) {
                    if (binary.at<uchar>(i, j) == 0) {
                        return false;
                    }
                }
            }
            x++;
        }
        y++;
    }
    return true;
}

bool dilation(int row, int col, Mat binary) {
    int y = 0;
    for (int i = row - int(innerMatrixSize / 2); i <= row + int(innerMatrixSize / 2); i++) {
        int x = 0;
        for (int j = col - int(innerMatrixSize / 2); j <= col + int(innerMatrixSize / 2); j++) {
            if (i < 0 or i >= binary.rows or j < 0 or j >= binary.cols) {
                continue;
            }
            else {
                if (structuringElement[y][x] == 1) {
                    if (binary.at<uchar>(i, j) == 255) {
                        return true;
                    }
                }
            }
            x++;
        }
        y++;
    }
    return false;
}

int main() {
    Mat binary = imread("lenna.png", IMREAD_GRAYSCALE);
    Mat rErosion(binary.rows, binary.cols, CV_8UC1, Scalar(0));
    Mat rDilation(binary.rows, binary.cols, CV_8UC1, Scalar(0));
    Mat maskImg(innerMatrixSize, innerMatrixSize, CV_8UC1, Scalar(0));

    // Create the mask image
    for (int i = 0; i < innerMatrixSize; i++) {
        for (int j = 0; j < innerMatrixSize; j++) {
            if (structuringElement[i][j] == 0) {
                maskImg.at<uchar>(i, j) = 0;
            }
            else {
                maskImg.at<uchar>(i, j) = 255;
            }
        }
    }

    // Threshold the binary image
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (binary.at<uchar>(i, j) >= 127) {
                binary.at<uchar>(i, j) = 255;
            }
            else {
                binary.at<uchar>(i, j) = 0;
            }
        }
    }

    //---------------------------------------------- EROSION
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (!erosion(i, j, binary)) {
                rErosion.at<uchar>(i, j) = 0;
            }
            else {
                rErosion.at<uchar>(i, j) = 255;
            }
        }
    }

    //---------------------------------------------- DILATION
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (dilation(i, j, binary)) {
                rDilation.at<uchar>(i, j) = 255;
            }
            else {
                rDilation.at<uchar>(i, j) = 0;
            }
        }
    }

    //---------------------------------------------- CLOSING
    // First perform dilation and then erosion
    Mat binary2 = imread("lenna.png", IMREAD_GRAYSCALE);
    Mat dClosing(binary.rows, binary.cols, CV_8UC1, Scalar(0));
    Mat closing(binary.rows, binary.cols, CV_8UC1, Scalar(0));
    Mat opening(binary.rows, binary.cols, CV_8UC1, Scalar(0));
    Mat eOpening(binary.rows, binary.cols, CV_8UC1, Scalar(0));

    // Threshold the binary2 image
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (binary2.at<uchar>(i, j) >= 127) {
                binary2.at<uchar>(i, j) = 255;
            }
            else {
                binary2.at<uchar>(i, j) = 0;
            }
        }
    }

    // Perform dilation on binary2 and store the result in dClosing
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (dilation(i, j, binary2)) {
                dClosing.at<uchar>(i, j) = 255;
            }
            else {
                dClosing.at<uchar>(i, j) = 0;
            }
        }
    }

    // Perform erosion on dClosing and store the result in closing
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (!erosion(i, j, dClosing)) {
                closing.at<uchar>(i, j) = 0;
            }
            else {
                closing.at<uchar>(i, j) = 255;
            }
        }
    }

    //---------------------------------------- OPENING
    // First perform erosion and then dilation
    // Perform erosion on binary2 and store the result in eOpening
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (!erosion(i, j, binary2)) {
                eOpening.at<uchar>(i, j) = 0;
            }
            else {
                eOpening.at<uchar>(i, j) = 255;
            }
        }
    }

    // Perform dilation on eOpening and store the result in opening
    for (int i = 0; i < binary.rows; i++) {
        for (int j = 0; j < binary.cols; j++) {
            if (dilation(i, j, eOpening)) {
                opening.at<uchar>(i, j) = 255;
            }
            else {
                opening.at<uchar>(i, j) = 0;
            }
        }
    }

    namedWindow("Original image", WINDOW_AUTOSIZE);
    imshow("Original image", image);

    namedWindow("Threshold", WINDOW_AUTOSIZE);
    imshow("Threshold", binary);

    namedWindow("Erosion", WINDOW_AUTOSIZE);
    imshow("Erosion", rErosion);

    namedWindow("Dilation", WINDOW_AUTOSIZE);
    imshow("Dilation", rDilation);

    namedWindow("Closing", WINDOW_AUTOSIZE);
    imshow("Closing", closing);

    namedWindow("Opening", WINDOW_AUTOSIZE);
    imshow("Opening", opening);

    namedWindow("Structuring Element", WINDOW_NORMAL);
    imshow("Structuring Element", maskImg);

    waitKey(0);
}
