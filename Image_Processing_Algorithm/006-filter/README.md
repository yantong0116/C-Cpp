# Image Equalization

## Execute
```
g++ filter.cpp -o filter -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./filter
```

## Output
| Input   | Average Filter|
| :---:   | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/ccceeea9-4a5d-418f-907b-5dc9dab49dec) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/12fa49de-b7ff-455e-aca4-eb9a7c4955bf) |
| **Median Filter** | **Border** |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/348d93bb-e961-4190-a17c-0763ca8ebd65) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/480215cb-9119-4226-aa7f-338791db1d80) |


