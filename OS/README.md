# OS
https://oscapstone.github.io/index.html

This course aims to introduce the **design and implementation of operating system kernels**. You’ll learn both concept and implementation from a series of labs.

This course uses **Raspberry Pi 3 Model B+ (rpi3 for short)** as the hardware platform.

## Execute
QEMU debug

```
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -S -s
```

```
cd /home/yantong/Desktop/yantong/GitHub/C-Cpp/OS/
aarch64-linux-gnu-gdb
```

### Lab Reference
https://github.com/CRTao/osc2023

### Reference
1. [嵌入式作業系統 Embedded Operating Systems](http://ocw.nctu.edu.tw/course_detail.php?bgid=8&gid=0&nid=575)
2. [曹孝櫟老師 - 作業系統設計與實作](https://www.youtube.com/playlist?list=PLj6E8qlqmkFvCxHVYYVYtnPuQb3NeOlUb)
3. https://hackmd.io/@O3vqd41hS8qfFYxGORVmnw/Alvie
4. https://qemu-project.gitlab.io/qemu/system/gdb.html
5. https://hackmd.io/@posutsai/SyNzl72f4?type=view#Bomb-Lab-%E5%AF%A6%E4%BD%9C%E7%B4%80%E9%8C%84
