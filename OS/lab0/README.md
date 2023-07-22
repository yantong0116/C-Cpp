## Introduction
In Lab 0, you need to prepare the environment for future development.

## Goals of this lab
- Set up the development environment.
- Understand what’s cross-platform development.
- Test your rpi3.

## Cross-Platform Development
### Cross Compiler
Rpi3 uses ARM Cortex-A53 CPU. To compile your source code to 64-bit ARM machine code, you need a cross compiler if you develop on a non-ARM64 environment.

### Linker
You might not notice the existence of linkers before. It’s because the compiler uses the default linker script for you. (ld --verbose to check the content) In bare-metal programming, you should set the memory layout yourself.

This is an incomplete linker script for you. You should extend it in the following lab.

- src/linker.ld

```text
SECTIONS
{
  . = 0x80000;
  .text : { *(.text) }
}
```
```
該鏈接描述文件的作用是將程序編譯後的可執行代碼放置在內存的地址 0x80000 處。
這裡 “.=0x80000;” 用於設置位置計數器（location counter）為 0x80000，表示程序的起始地址為0x80000。 
然後，使用 “.text” 部分從所有標記為 “.text” 的輸入文件中收集代碼，並將它們放入 “.text” 部分。 “.text” 節通常用於存儲程序的可執行代碼（機器指令）。
```

### Setup Qemu environment
```console
./install_env.sh
```

## From Source Code to Kernel Image

### From Source Code to Object Files
- src / a.S

```assembly
.section ".text"
_start:
  wfe
  b _start
```
```
這段彙編代碼定義了一個簡單的程序。它包含了兩個 ARM 彙編指令，實現了一個無限循環。
.section ".text"：這行指示代碼所在的段是".text"段。 ".text" 段通常用於存儲程序的可執行代碼。
_start:：這行定義了一個標籤叫做"_start"，它是程序的入口點，類似於高級編程語言中的 main 函數。
wfe：這行是一條 ARM 彙編指令，代表 "Wait For Event"，將處理器置於低功耗模式，直到有事件喚醒它。在這段代碼中，沒有特定的事件要等待，所以它可能會一直使處理器進入睡眠狀態。
b _start：這行是一條 ARM 彙編指令，"b"是跳轉指令，用於跳轉到代碼中的特定位置。在這裡，它跳回到 "_start" 標籤，形成一個無限循環。程序將不斷執行 "wfe" 指令，然後跳回到開始處，形成一個無盡循環。
綜合起來，這個簡單的彙編程序將會在運行時不斷執行 "wfe" 指令，進入低功耗模式，然後又跳回到開始處，形成一個永遠不會停止的循環。
```

- To generate a.o
```
aarch64-linux-gnu-gcc -c a.S
```

### From Object Files to ELF
A linker links object files to an ELF file. An **ELF file can be loaded and executed by program loaders**. Program loaders are usually provided by the operating system in a regular development environment. In bare-metal programming, **ELF can be loaded by some bootloaders**.

- To generate kernel.elf
```
aarch64-linux-gnu-ld -T linker.ld -o kernel8.elf a.o
```

### From ELF to Kernel Image
Rpi3’s bootloader can’t load ELF files. Hence, you need to convert the ELF file to a raw binary image. You can use objcopy to convert ELF files to raw binary.

- To generate kernel8.img
```
aarch64-linux-gnu-objcopy -O binary kernel8.elf kernel8.img
```

## Execute

**Check on QEMU**

```
cd src
make
make run
```
