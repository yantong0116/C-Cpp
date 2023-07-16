# Dilation Erosion Opening Closing with ker 7x7

## Execute
```
g++ dilation-erosion-open-close.cpp -o dilation-erosion-open-close -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./dilation-erosion-open-close
```

## Image
### Input
#### Original image
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/6b075e50-b7fb-4719-9f04-57ff59e63a54)

### Output

| Threshold | Structuring Element |
| :---: | :---: |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/4d2d6026-2353-42e3-aad6-8d19680d00b2) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/6dfa9a82-2dae-4990-ae73-22dc7885778b) |
| Dilation | Erosion |
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/7af413f2-d838-4ea5-9cb8-500c515fda69) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/93c77d95-c002-4e5d-821b-ad278e4b85bc) |
| Opening | Closing | 
| ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/f36db41f-de16-4b95-b758-244a8695ca49) | ![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/20abc6e2-d470-4168-85d4-653ee24ca215) |
