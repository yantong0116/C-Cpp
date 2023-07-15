# Gaussian noise

## Concept
1. Average Blur:
   - Formula: `average = sum / (innerMatrixIndex * innerMatrixIndex)`
   - Description: Calculates the average value of pixel intensities within the kernel.

2. Absolute Difference:
   - Formula: `abs_diff = abs(image.at<uchar>(i, j) - blur.at<uchar>(i, j))`
   - Description: Calculates the absolute difference between corresponding pixels in the `image` and `blur` matrices.

3. Thresholding:
   - Formula: If `difference.at<uchar>(i, j) <= limit`, then `difference.at<uchar>(i, j) = 0`. Otherwise, `difference.at<uchar>(i, j) = 255`.
   - Description: Applies a threshold to the `difference` matrix, setting pixels below the threshold to 0 and above to 255.

4. Median Filter:
   - Formula: `median = myVector.at(myVector.size() / 2)`
   - Description: Calculates the median value of pixel intensities within the kernel.

5. Gaussian Blur:
   - Formula: `average2 = sum2 / 16`
   - Description: Calculates the weighted average value of pixel intensities within the Gaussian kernel.

6. Gaussian Blur 5x5:
   - Formula: `average3 = sum3 / 273`
   - Description: Calculates the weighted average value of pixel intensities within the larger Gaussian kernel.

7. Gaussian Filter (25x25):
   - Formulas:
     - `up = (cX * cX) + (cY * cY)`
     - `exp1 = exp(-(up) / (down))`
     - `gauss[i][j] = constant * exp1`
     - `average4 = sum4 / sumaFiltro`
   - Description: Calculates the Gaussian weights for the 25x25 kernel and applies Gaussian filtering on the image.

## Execute
```
g++ gauss.cpp -o gauss -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./gauss
```

## Output

### Original
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/eca4d967-9a00-4f30-a1e9-e43050a542b4)

| Pepper noise | Median filter |
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/3d7e45bd-6e45-4ad9-9309-838915e952a8) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/bcde6111-c846-408c-93c1-74879ec008ba) |

| Blur | Difference |
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/485b02d8-0332-4b13-8332-be94da371102) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dd33b0d7-197f-455c-a88f-34a3e5699f33) |
| Gaussian | Gaussian2 | 
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/2023b7c0-7ca7-4d90-ad3d-24068da59256) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dd8bea23-a310-47f5-81c3-79a1679d50a3) |
| Gaussian3 | |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/19d145f9-6941-441f-be8c-9ab1cfd56f8c) | |
