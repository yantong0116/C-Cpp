# Image Mask
This is a small exercise where a binary filter is used as a mask of visibility of an image in another image.

## Input
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/575fd7b8-3240-48fa-9ade-82a4eed03907)

## Execute
```
g++ mask.cpp -o mask -std=c++11 `pkg-config --cflags --libs opencv`
```
```
./mask
```

## Output
The resulting mask is as follows. Note that the mask must be the same as the target image.
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/fe4b378a-1aae-48e3-a64e-79816749b5eb)

Below is the mask value of the image.
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/4070b7ae-3efe-460b-82f6-64f1f503aa60)

The resulting image is the following. Note that the pattern above is in the white areas of the mask.
![image](https://github.com/yantong0116/C-Cpp-Learning/assets/51469882/6a1df023-c8b1-419f-ad7f-c95209b68ff2)
