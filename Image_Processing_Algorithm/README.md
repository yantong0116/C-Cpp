# Image Processing Algorithms in C++

This repo contains a handful of the most famous algorithms for image processing, coded from scratch just using math and several data structures such as vectors and queues.

## Setup Environment
- Ubuntu 18.04.6 LTS
- Install OpenCV for C++
- http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/
- sudo apt install libcanberra-gtk-module libcanberra-gtk3-module

## Content
1. Threshold
2. Mask
3. Gradient
4. Addition
5. Equalization
6. Filter
7. Gaussian
8. Sobel
9. Sobel, Prewitt, Roberts
10. Regions
11. Dilation, Erosion

## Execute Example
```
g++ main.cpp -o main -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./main
```

## Reference
- https://github.com/the-other-mariana/image-processing-algorithms
- https://github.com/yantong0116/Qt-5-and-OpenCV-4-Computer-Vision-Projects/
