# Lab 2 : Booting

## Introduction
Booting is the process to set up the environment to run various user programs after a computer reset. It includes a kernel loaded by bootloader, subsystems initialization, device-driver matching, and loading the init user program to bring up the remaining services in userspace.

In Lab 2, you’ll learn one of the methods to load your kernel and user programs. Also, you’ll learn how to match a device to a driver on rpi3. The initialization of the remaining subsystems will be introduced at later labs.

## Goals of this lab
- Implement a bootloader that loads kernel images through UART.
- Implement a simple allocator.
- Understand what’s initial ramdisk.
- Understand what’s devicetree.

## Background
### How a Kernel is Loaded on Rpi3
There are 4 steps before your kernel starts its execution.
1. GPU executes the first stage bootloader from ROM on the SoC.
2. The first stage bootloader recognizes the FAT16/32 file system and loads the second stage bootloader bootcode.bin from SD card to L2 cache.
3. bootcode.bin initializes SDRAM and loads start.elf
4. start.elf reads the configuration to load a kernel and other data to memory then wakes up CPUs to start execution.

The kernel loaded at step 4 can also be another bootloader with more powerful functionalities such as network booting, or ELF loading.

In Lab 2, you’ll implement a bootloader that loads the actual kernel through UART, and it’s loaded by the previous stage bootloader.

## Exercises
### Basic Exercise 1 - UART Bootloader
In Lab 1, you might experience the process of moving the SD card between your host and rpi3 very often during debugging. You can eliminate this by introducing another bootloader to load the kernel under debugging.

To send binary through UART, you should devise a protocol to read raw data. It rarely drops data during transmission, so you can keep the protocol simple.

You can effectively write data from the host to rpi3 by serial device’s device file in Linux.

```
with open('/dev/ttyUSB0', "wb", buffering = 0) as tty:
  tty.write(...)
```
> __Note__
> You can use <font color="red">qemu-system-aarch64 -serial null -serial pty</font> to create a pseudo TTY device and test your bootloader through it.

#### Config Kernel Loading Setting

You may still want to load your actual kernel image at 0x80000, but it then overlaps with your bootloader. You can first specify the start address to another by **re-writing the linker script**. Then, add **config.txt** file to your SD card’s boot partition to specify the loading address by **kernel_address=**.

To further make your bootloader less ambiguous with the actual kernel, you can add the loading image name by **kernel=** and **arm_64bit=1**

```
kernel_address=0x60000
kernel=bootloader.img
arm_64bit=1
```

> **Todo**
> Implement a UART bootloader that loads kernel images through UART.

> **Important**
> UART is a low-speed interface. It’s okay to send your kernel image because it’s quite small. Don’t use it to send large binary files.

### Basic Exercise 2 - Initial Ramdisk

After a kernel is initialized, it mounts a root filesystem and runs an init user program. The init program can be a script or executable binary to bring up other services or load other drivers later on.

However, you haven’t implemented any filesystem and storage driver code yet, so you can’t load anything from the SD card using your kernel. Another approach is loading user programs through the initial ramdisk.

An initial ramdisk is a file loaded by a bootloader or embedded in a kernel. It’s usually an archive that can be extracted to build a root filesystem.

#### New ASCII Format Cpio Archive

Cpio is a very simple archive format to pack directories and files. Each directory and file is recorded as **a header followed by its pathname and content**.

In Lab 2, you are going to use the New ASCII Format Cpio format to create a cpio archive. You can first create a **rootfs** directory and put all files you need inside it. Then, use the following commands to archive it.

```
cd rootfs
find . | cpio -o -H newc > ../initramfs.cpio
cd ..
```

Freebsd’s man page has a detailed definition of how New ASCII Format Cpio Archive is structured. You should read it and implement a parser to read files in the archive.

#### Loading Cpio Archive

**QEMU**
Add the argument **-initrd <cpio archive>** to QEMU. QEMU loads the cpio archive file to 0x8000000 by default.

**Rpi3**

Move the cpio archive into SD card. Then specify the name and loading address in **config.txt**.

```
initramfs initramfs.cpio 0x20000000
```

> **Todo**
> Parse New ASCII Format Cpio archive, and read file’s content given file’s pathname.

![image](https://github.com/yantong0116/OS/assets/51469882/bf9e8bda-e679-4850-a4af-d4cd19b60fac)

> **Important**
> In Lab 2, you only need to **put some plain text files inside your archive** to test the functionality. In the later labs, you will also put script files and executables inside to automate the testing.

### Basic Exercise 3 - Simple Allocator

Kernel needs an allocator in the progress of subsystem initialization. However, the dynamic allocator is also a subsystem that need to be initialized. So we need a simple allocator in the early stage of booting.

> **Todo**
> Implement a alloc function that returns a pointer points to a continuous space for requested size.

> **Hint**
> Your allocator don’t need to support free function.

The folloing code is a breif example:
```
void* simple_malloc(size_t size) {
  ...
}

int main() {
  char* string = simple_alloc(8);
}
```

### Advanced Exercise 1 - Bootloader Self Relocation


### Advanced Exercise 2 - Devicetree


## Implement
### Execute lab2
#### Generate initramfs.cpio
```console
cd create_fs
bash create_cpio.sh
```

#### Copy create_fs/initramfs.cpio to kernel/initramfs.cpio
#### Generate kernel/build
```console
cd kernel
make
```

```console
make run
```


