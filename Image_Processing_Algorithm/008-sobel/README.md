# Enhanced Image Accentuation with Gaussian Filtering and Sobel Operators

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




