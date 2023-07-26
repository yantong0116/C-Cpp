## Exercise 1
### UART Bootloader

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/ca5506ce-134f-41e8-898c-82d0202be701)

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/124f2f9b-5e3b-4be6-97c2-3bf6b6b4a2d8)

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/55d41cb6-3609-4c6f-9a46-a31f26ee6fb1)

```
send_image_to_bootloader.py
```

## Exercise 2
### Initial Ramdisk

#### Generate initramfs.cpio

```
cd create_fs
create_cpio.sh
makefile
```

#### In makefile
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial null -serial stdio **-initrd initramfs.cpio** -dtb bcm2710-rpi-3-b-plus.dtb

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/78eb58a5-8ffd-4f93-bbe4-23dd44616d87)

## Exercise 3
### Simple Allocator

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/d6e2acd8-ebaf-44fd-89b9-74c5ce330485)

## Advanced Exercise 1
### Devicetree





