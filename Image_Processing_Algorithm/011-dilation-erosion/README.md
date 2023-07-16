# Dilation and Erosion
## Concept
### Dilation
- Original Image
0 0 0 0 0 0 0
0 1 1 1 0 0 0
0 1 1 1 0 0 0

- Structuring Element
0 1 0
1 1 1
0 1 0

- Dilation Result
0 1 1 1 0 0 0
1 1 1 1 1 1 0
1 1 1 1 1 1 0

### Erosion
- Original Image
1 1 1 0 0 0 0
1 1 1 0 0 0 0
1 1 1 0 0 0 0

- Structuring Element
0 1 0
1 1 1
0 1 0

- Erosion Result
0 0 0 0 0 0 0
0 0 0 0 0 0 0
0 0 0 0 0 0 0

## Execute
```
g++ dilation-erosion.cpp -o dilation-erosion -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./dilation-erosion
```

## Output
| Input | Dilation | Erosion
| :---: | :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/1db8cb94-76d2-424d-b16f-17274b3435ed) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/f417a837-4546-4afe-a989-8a92f688f741) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/3e645f52-6860-4601-9237-a14aee419510) |



