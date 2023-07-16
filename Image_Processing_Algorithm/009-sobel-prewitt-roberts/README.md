# Enhancement and Sobel Prewitt Roberts

## Concept
### Enhancement
1. Gaussian Filter Calculation

<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/b6126583-33e0-45ff-a430-ed45b57064c0"/>
</p>

2. Sharpness Difference Calculation

<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/fe0349ff-04e1-4694-9063-7f61099541ca"/>
</p>

3. Enhanced Image Calculation
<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/d81ffeb7-ae30-44af-93fc-8a72c8aa97ed"/>
</p>

### Sobel Prewitt Roberts
1. Sobel Operator
<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/cd103deb-96e6-4476-af5d-53bcb59c5f96"/>
</p>

<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/f2b5e061-303a-497b-9011-2a08a565f6df"/>
</p>

2. Prewitt Operator
<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/04e41134-2d88-4ee6-80a7-c27340a65c0b"/>
</p>

<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/d63ffa9d-0728-41c8-87d3-4f30421928c2"/>
</p>

3. Roberts Operator
<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/1c0426c7-12e1-44b4-9e70-941feeffa318"/>
</p>

<p align="center">
<img src="https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/daf301bb-0d1d-4d15-854d-5dcb366236b9"/>
</p>

## Execute 1
```
g++ enhancement.cpp -o enhancement -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./enhancement
```

## Execute 2
```
g++ sobel-prewitt-roberts.cpp -o sobel-prewitt-roberts -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./sobel-prewitt-roberts
```


### Original Image
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/33b28bcc-de29-4918-be2f-e9176aed63db)

### Output 1
| Gaussian Filtered Image | Sharpness Difference | Enhanced Image |
| :---: | :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/c14b8667-b93e-4922-8909-2883f699fa4c) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/8787e0bd-2481-4994-90dd-936710e0864e) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/b1e85932-ca32-4689-916c-abaa7ee3bd4b) |

### Output 2
| Sobel Horizontal | Sobel Vertical | Sobel Gradient Magnitude | 
| :---: | :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/26019d75-e006-45d5-8c11-85c33cd101cd) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/8235c55b-a34d-452b-9f35-fd9b0cae7d9a) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/7638f818-7b64-408f-a3b7-ad9b94a05865) |
| Prewitt Horizontal | Prewitt Vertical | Prewitt Gradient Magnitude | 
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dfbb2f5f-fb6b-4003-baa2-6f5db8141c56) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/dfc26a62-aa07-4cb6-a1ce-30e8b23a1115) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/af563833-6500-4e3f-922a-b299c3d9a770) |
| Roberts Horizontal | Roberts Vertical | Roberts Gradient Magnitude |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/0765accb-3087-4c94-a0eb-26b371a6f49c) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/49ec83a4-e391-4c9a-9454-12209b9af1bc) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/380c157d-9abb-44a0-9356-01d225940662) |







