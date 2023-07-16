# Opening and Closing
## Concept
1. Opening : Erosion -> Dilation
2. Closing : Dilation -> Erosion

## Execute
```
g++ open-close.cpp -o open-close -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./open-close
```

## Image
| Original | Opening | Closing |
| :---: | :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/29cb2db3-0c06-42a2-ad6d-9a3933bf1917) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/35d06f55-1a20-483b-b868-2354da143808) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/1d50fbd5-c3d5-4f86-993d-7c3e6010c66c) |
