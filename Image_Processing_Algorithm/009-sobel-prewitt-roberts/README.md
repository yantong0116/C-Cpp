# Sobel Prewitt Roberts

## Concept

## Execute
```
g++ sobel-prewitt-roberts.cpp -o sobel-prewitt-roberts -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./sobel-prewitt-roberts
```

## Image
### Original Image
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/33b28bcc-de29-4918-be2f-e9176aed63db)

### Output
| Sobel Horizontal | Sobel Vertical | Sobel Gradient Magnitude | 
| :---: | :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/26019d75-e006-45d5-8c11-85c33cd101cd) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/8235c55b-a34d-452b-9f35-fd9b0cae7d9a) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/7638f818-7b64-408f-a3b7-ad9b94a05865) |
| Prewitt Horizontal | Prewitt Vertical | Prewitt Gradient Magnitude | 
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dfbb2f5f-fb6b-4003-baa2-6f5db8141c56) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dfc26a62-aa07-4cb6-a1ce-30e8b23a1115) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/af563833-6500-4e3f-922a-b299c3d9a770) |
| Roberts Horizontal | Roberts Vertical | Roberts Gradient Magnitude |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/0765accb-3087-4c94-a0eb-26b371a6f49c) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/49ec83a4-e391-4c9a-9454-12209b9af1bc) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/380c157d-9abb-44a0-9356-01d225940662) |







