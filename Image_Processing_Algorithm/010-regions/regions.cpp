#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>
#include <cmath>
#include "opencv2/video.hpp"
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;
using namespace cv;

int numRegions = 0;
queue<Point2d> pointQueue;
Mat binaryImage = imread("lenna.png", IMREAD_GRAYSCALE);
Mat region(binaryImage.rows, binaryImage.cols, CV_8UC1, Scalar(0));
int color = 200;

void find4ConnectedNeighbors(int row, int col) {
    Point2d point;
    if (binaryImage.at<uchar>(row - 1, col) == 255) {
        point.x = row - 1;
        point.y = col;
        pointQueue.push(point);
    }
    if (binaryImage.at<uchar>(row, col + 1) == 255) {
        point.x = row;
        point.y = col + 1;
        pointQueue.push(point);
    }
    if (binaryImage.at<uchar>(row + 1, col) == 255) {
        point.x = row + 1;
        point.y = col;
        pointQueue.push(point);
    }
    if (binaryImage.at<uchar>(row, col - 1) == 255) {
        point.x = row;
        point.y = col - 1;
        pointQueue.push(point);
    }
}

int main() {
    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            if (binaryImage.at<uchar>(i, j) >= 127) {
                binaryImage.at<uchar>(i, j) = 255;
            } else {
                binaryImage.at<uchar>(i, j) = 0;
            }
        }
    }

    for (int i = 0; i < binaryImage.rows; i++) {
        for (int j = 0; j < binaryImage.cols; j++) {
            int pixelValue = int(binaryImage.at<uchar>(i, j));
            if (pixelValue == 255 && (region.at<uchar>(i, j) == 0)) {
                region.at<uchar>(i, j) = color;
                find4ConnectedNeighbors(i, j);
                while (!pointQueue.empty()) {
                    Point2d item;
                    item = pointQueue.front();
                    pointQueue.pop();
                    if ((region.at<uchar>(item.x, item.y) == 0) && binaryImage.at<uchar>(item.x, item.y) == 255) {
                        region.at<uchar>(item.x, item.y) = color;
                        find4ConnectedNeighbors(item.x, item.y);
                    }
                }
                numRegions++;
                color += 50;
            }
        }
    }

    cout << "Number of regions: " << numRegions << endl;

    namedWindow("Binary Image", WINDOW_AUTOSIZE);
    imshow("Binary Image", binaryImage);

    namedWindow("Region", WINDOW_AUTOSIZE);
    imshow("Region", region);
    waitKey(0);
}
