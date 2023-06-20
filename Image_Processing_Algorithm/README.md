### Setup Environment
- http://techawarey.com/programming/install-opencv-c-c-in-ubuntu-18-04-lts-step-by-step-guide/
- sudo apt install libcanberra-gtk-module libcanberra-gtk3-module

### Execute
- g++ main.cpp -o main -std=c++11 `pkg-config --cflags --libs opencv`
- ./main
