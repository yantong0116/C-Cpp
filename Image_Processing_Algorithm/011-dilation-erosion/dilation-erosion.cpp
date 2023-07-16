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
Mat erosionResult(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));
Mat dilationResult(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));

int structuringElement[3][3] = { {0, 1, 0}, {1, 1, 1}, {0, 1, 0} };
int structuringElementSize = 3;

bool applyErosion(int row, int col) {
    int y = 0;
    for (int i = row - int(structuringElementSize / 2); i <= row + int(structuringElementSize / 2); i++) {
        int x = 0;
        for (int j = col - int(structuringElementSize / 2); j <= col + int(structuringElementSize / 2); j++) {
            if (i < 0 || i >= binaryImage.rows || j < 0 || j >= binaryImage.cols) {
                continue;
            }
            else {
                if (structuringElement[y][x] == 1) {
                    if (binaryImage.at<uchar>(i, j) == 0) {
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

bool applyDilation(int row, int col) {
    int y = 0;
    for (int i = row - int(structuringElementSize / 2); i <= row + int(structuringElementSize / 2); i++) {
        int x = 0;
        for (int j = col - int(structuringElementSize / 2); j <= col + int(structuringElementSize / 2); j++) {
            if (i < 0 || i >= binaryImage.rows || j < 0 || j >= binaryImage.cols) {
                continue;
            }
            else {
                if (structuringElement[y][x] == 1) {
                    if (binaryImage.at<uchar>(i, j) == 255) {
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
    //---------------------------------------------- EROSION
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (!applyErosion(i, j)) {
                erosionResult.at<uchar>(i, j) = 25;
            }
            else {
                erosionResult.at<uchar>(i, j) = 230;
            }
        }
    }

    //---------------------------------------------- DILATION
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (applyDilation(i, j)) {
                dilationResult.at<uchar>(i, j) = 230;
            }
            else {
                dilationResult.at<uchar>(i, j) = 25;
            }
        }
    }

    int differenceErosion = 0;
    int differenceDilation = 0;
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (binaryImage.at<uchar>(i, j) != erosionResult.at<uchar>(i, j)) {
                differenceErosion++;
            }
            if (binaryImage.at<uchar>(i, j) != dilationResult.at<uchar>(i, j)) {
                differenceDilation++;
            }
        }
    }

    cout << "Difference with erosion: " << differenceErosion << endl;
    cout << "Difference with dilation: " << differenceDilation << endl;

    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", binaryImage);

    namedWindow("Erosion", WINDOW_AUTOSIZE);
    imshow("Erosion", erosionResult);

    namedWindow("Dilation", WINDOW_AUTOSIZE);
    imshow("Dilation", dilationResult);

    waitKey(0);
}
