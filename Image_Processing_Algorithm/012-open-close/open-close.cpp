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

Mat binaryImage = imread("lenna.png", IMREAD_GRAYSCALE);

int structuringElement[3][3] = {{0, 1, 0}, {1, 1, 1}, {0, 1, 0}};
int structuringElementSize = 3;

bool performErosion(int row, int col, Mat binary) {
    int y = 0;
    for (int i = row - int(structuringElementSize / 2); i <= row + int(structuringElementSize / 2); i++) {
        int x = 0;
        for (int j = col - int(structuringElementSize / 2); j <= col + int(structuringElementSize / 2); j++) {
            if (i < 0 || i >= binary.rows || j < 0 || j >= binary.cols) {
                continue;
            } else {
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

bool performDilation(int row, int col, Mat binary) {
    int y = 0;
    for (int i = row - int(structuringElementSize / 2); i <= row + int(structuringElementSize / 2); i++) {
        int x = 0;
        for (int j = col - int(structuringElementSize / 2); j <= col + int(structuringElementSize / 2); j++) {
            if (i < 0 || i >= binary.rows || j < 0 || j >= binary.cols) {
                continue;
            } else {
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

    Mat binaryImage = imread("lenna.png", IMREAD_GRAYSCALE);

    // Thresholding
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (binaryImage.at<uchar>(i, j) >= 127) {
                binaryImage.at<uchar>(i, j) = 255;
            } else {
                binaryImage.at<uchar>(i, j) = 0;
            }
        }
    }

    //---------------------------------------------- CLOSING
    // First perform dilation and then erosion
    Mat binaryImageForClosing = imread("lenna.png", IMREAD_GRAYSCALE);
    Mat dilatedImageClosing(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));
    Mat closing(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));
    Mat opening(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));
    Mat erodedOpening(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));

    // Thresholding on binaryImageForClosing
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (binaryImageForClosing.at<uchar>(i, j) >= 127) {
                binaryImageForClosing.at<uchar>(i, j) = 255;
            } else {
                binaryImageForClosing.at<uchar>(i, j) = 0;
            }
        }
    }

    // Perform dilation on binaryImageForClosing, store the result in dilatedImageClosing
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (performDilation(i, j, binaryImageForClosing)) {
                dilatedImageClosing.at<uchar>(i, j) = 255;
            } else {
                dilatedImageClosing.at<uchar>(i, j) = 0;
            }
        }
    }

    // Perform erosion on dilatedImageClosing, store the result in closing
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (!performErosion(i, j, dilatedImageClosing)) {
                closing.at<uchar>(i, j) = 0;
            } else {
                closing.at<uchar>(i, j) = 255;
            }
        }
    }

    //---------------------------------------- OPENING
    // First perform erosion and then dilation
    // Perform erosion on binaryImageForClosing, store the result in erodedOpening
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (!performErosion(i, j, binaryImageForClosing)) {
                erodedOpening.at<uchar>(i, j) = 0;
            } else {
                erodedOpening.at<uchar>(i, j) = 255;
            }
        }
    }

    // Perform dilation on erodedOpening, store the result in opening
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (performDilation(i, j, erodedOpening)) {
                opening.at<uchar>(i, j) = 255;
            } else {
                opening.at<uchar>(i, j) = 0;
            }
        }
    }

    // Create windows and display images
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", binaryImage);

    namedWindow("Closing", WINDOW_AUTOSIZE);
    imshow("Closing", closing);

    namedWindow("Opening", WINDOW_AUTOSIZE);
    imshow("Opening", opening);

    waitKey(0);
}
