# Gaussian noise

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
