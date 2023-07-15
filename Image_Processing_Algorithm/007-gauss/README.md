# Gaussian noise

## Concept
1. Average Blur:
   - Formula: `average = sum / (kernelSize * kernelSize)`
   - Description: Calculates the average value of pixel intensities within a square kernel.

2. Difference of Images:
   - Formula: `difference = abs(lennaImage - averageBlur)`
   - Description: Computes the absolute difference between the pixel intensities of the original image and the average-blurred image.

3. Thresholding:
   - Formula: `if (difference <= threshold) difference = 0; else difference = 255;`
   - Description: Applies a threshold to the difference image, setting pixel values below the threshold to 0 (black) and above the threshold to 255 (white).

4. Median Filter:
   - Formula: `medianFilter = median(pixels)`
   - Description: Applies a median filter to the pepper noise image. The filter selects the median pixel value from a neighborhood of pixels within a kernel.

5. Gaussian Blur (3x3):
   - Formula: average = sum / 16
   - Description: Applies a Gaussian blur to the original image using a 3x3 Gaussian kernel. The kernel weights each pixel intensity with a predefined Gaussian weight.

6. Gaussian Blur (5x5):
   - Formula: `average = sum / 273`
   - Description: Applies a Gaussian blur to the original image using a 5x5 Gaussian kernel. The kernel weights each pixel intensity with a predefined Gaussian weight.

7. Custom Gaussian Blur (25x25):
   - Formulas:
     - Calculate Gaussian kernel weights:
       - `exponent = -(deltaX^2 + deltaY^2) / (2 * sigma^2)`
       - `constant = 1 / (sigma^2 * 2 * pi)`
       - `gaussianKernel[i][j] = constant * exp(exponent)`
     - Calculate average:
       - `average = sum / sumFilter`
   - Description: Applies a custom Gaussian blur to the original image using a 25x25 Gaussian kernel. The kernel weights each pixel intensity based on the distance from the center of the kernel, following a Gaussian distribution.


## Execute
```
g++ gauss.cpp -o gauss -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./gauss
```

## Image

### Original Image
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/9b768f8e-7a0b-4cdd-b21d-4448d4eadd00)

### Process
| Pepper Noise | Median Filter |
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/a70a93a9-fa66-4606-84b3-75cffe4c38ee) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/f342dba8-bb09-45aa-981a-f504fbb23fe4) | 

### Output
| Average Blur | Difference |
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/4359eca9-5335-48e9-b601-e9719d7b5d55) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/143eed37-f269-40cb-a4e4-4c75f15a902c) |
| **Gaussian Blur 3x3** | **Gaussian Blur 5x5** |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/67eb9656-6f15-4385-86ac-0c76b20d72b8) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/8b40878d-f8f0-470a-bfdb-aee25658329d) | 
| **Custom Gaussian Blur** | |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/fdc7dd67-d752-4a65-8d5f-0e4417d474e6) | |


