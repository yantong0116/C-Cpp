# Sobel Operators
#### Enhanced Image Accentuation with Gaussian Filtering and Sobel Operators

## Concept

1. Gaussian Kernel Formula:
   - Formula: `gaussianKernel[i][j] = constant * exp(-(xDistance * xDistance + yDistance * yDistance) / (2 * sigma * sigma))`
   - Purpose: Calculates the values of a Gaussian kernel used for Gaussian filtering.

2. Gaussian Filtering Formula:
   - Formula: `sum += originalImage.at<uchar>(row, col) * gaussianKernel[a + xCenter][b + yCenter]`
   - Purpose: Applies Gaussian filtering to an image by convolving the image with the Gaussian kernel.

3. Sharp Changes Calculation Formula:
   - Formula: `sharpChanges.at<float>(i, j) = originalImage.at<uchar>(i, j) - gaussianFiltered3.at<uchar>(i, j)`
   - Purpose: Calculates the sharp changes in pixel intensities by subtracting the Gaussian filtered image from the original image.

4. Accentuated Image Formula:
   - Formula: `accentuatedImage.at<uchar>(i, j) = (pixelValue > 255) ? 255 : static_cast<uchar>(pixelValue)`
   - Purpose: Generates an accentuated image by adding the sharp changes to the original image, clipping the pixel values to the range [0, 255].

5. Sobel Operators Formula:
   - Horizontal Sobel: Calculates the gradient in the x-direction.
   - Vertical Sobel: Calculates the gradient in the y-direction.
   - Formulas: `sobelHorizontal.at<float>(i, j) = sumH / 255.0` and `sobelVertical.at<float>(i, j) = sumV / 255.0`
   - Purpose: Computes the Sobel operators for edge detection by convolving the image with the respective Sobel kernels.

6. Vector Summation Formula:
   - Formula: `rootImage.at<float>(i, j) = sqrt(xValue + yValue)`
   - Purpose: Calculates the magnitude of the gradient vector by taking the square root of the sum of squared gradient values obtained from the horizontal and vertical Sobel operators.

## Execute
```
g++ sobel.cpp -o sobel -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./sobel
```

## Image
### Original Image
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/add5f005-7759-45fd-9304-f98db8d05e63)

### Output
| Gaussian Filtered (Sigma=3) | Sharp Changes |
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/46b7c33c-80d0-4709-8a4b-780853693d4a) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/e1bf79e8-a2a1-4644-9c34-f48eb8517273) |
| **Accentuated Image** | **Sobel Horizontal** |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/2d4246ac-27e0-4a11-87f9-9050d0bfcc53) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/2bcfbfeb-9764-4fa4-ad20-69331740a9c3) |
| **Sobel Vertical** | **Root Image** | 
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/5c75ccd8-aba6-47ee-81df-3b00af738086) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/eda19717-b616-4bce-874f-3404a0659a20) |




