#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <queue>

using namespace std;
using namespace cv;

int calculateB(int row, int col, Mat binary) {
    int sum = 0;
    sum = (binary.at<uchar>(row - 1, col) > 0) + (binary.at<uchar>(row - 1, col + 1) > 0) +
           (binary.at<uchar>(row, col + 1) > 0) + (binary.at<uchar>(row + 1, col + 1) > 0) +
           (binary.at<uchar>(row + 1, col) > 0) + (binary.at<uchar>(row + 1, col - 1) > 0) +
           (binary.at<uchar>(row, col - 1) > 0) +
           (binary.at<uchar>(row - 1, col - 1) > 0);
    return sum;
}

int calculateA(int row, int col, Mat binary) {
    int repetitions = 0;
    string pattern = "";

    (binary.at<uchar>(row - 1, col) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row - 1, col + 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row, col + 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row + 1, col + 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row + 1, col) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row + 1, col - 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row, col - 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row - 1, col - 1) == 0) ? pattern += "0" : pattern += "1";
    (binary.at<uchar>(row - 1, col) == 0) ? pattern += "0" : pattern += "1";

    for (int i = 0; i < 9; i++) {
        if (i == 8) {
            continue;
        }
        if (pattern[i] == '0' && pattern[i + 1] == '1') {
            repetitions++;
        }
    }
    return repetitions;
}

int main() {
    Mat binaryImage = imread("lenna.png", IMREAD_GRAYSCALE);
    Mat thinnedImage = binaryImage.clone();

    // Threshold the image
    threshold(binaryImage, binaryImage, 127, 255, THRESH_BINARY);

    while (true) {
        queue<Point2d> pointsToBeRemoved;

        for (int i = 1; i < binaryImage.rows - 1; i++) {
            for (int j = 1; j < binaryImage.cols - 1; j++) {
                if (thinnedImage.at<uchar>(i, j) > 0) {
                    int b = calculateB(i, j, thinnedImage);
                    int a = calculateA(i, j, thinnedImage);

                    int p3_1 = (thinnedImage.at<uchar>(i - 1, j) > 0) *
                               (thinnedImage.at<uchar>(i, j + 1) > 0) *
                               (thinnedImage.at<uchar>(i + 1, j) > 0);
                    int p4_1 = (thinnedImage.at<uchar>(i, j + 1) > 0) *
                               (thinnedImage.at<uchar>(i + 1, j) > 0) *
                               (thinnedImage.at<uchar>(i, j - 1) > 0);

                    int p3_2 = (thinnedImage.at<uchar>(i - 1, j) > 0) *
                               (thinnedImage.at<uchar>(i, j + 1) > 0) *
                               (thinnedImage.at<uchar>(i, j - 1) > 0);
                    int p4_2 = (thinnedImage.at<uchar>(i - 1, j) > 0) *
                               (thinnedImage.at<uchar>(i + 1, j) > 0) *
                               (thinnedImage.at<uchar>(i, j - 1) > 0);

                    if (((b >= 2 && b <= 6) && (a == 1) && (p3_1 == 0) && (p4_1 == 0)) ||
                        ((b >= 2 && b <= 6) && (a == 1) && (p3_2 == 0) && (p4_2 == 0))) {
                        Point2d point;
                        point.x = i;
                        point.y = j;
                        pointsToBeRemoved.push(point);
                    }
                }
            }
        }

        if (pointsToBeRemoved.empty()) {
            break;
        }

        while (!pointsToBeRemoved.empty()) {
            Point2d point = pointsToBeRemoved.front();
            pointsToBeRemoved.pop();
            thinnedImage.at<uchar>(point.x, point.y) = 0;
        }

        namedWindow("Thinned", WINDOW_AUTOSIZE);
        imshow("Thinned", thinnedImage);
        waitKey(30);
    }

    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", binaryImage);

    waitKey(0);
}
