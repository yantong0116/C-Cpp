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
- src/lab0.ld
```text
SECTIONS
{
  . = 0x80000;
  .text : { *(.text) }
}
```

### Setup Qemu environment
```console
./install_env.sh
```
## From Source Code to Kernel Image

### From Source Code to Object Files
- src/a.S
```text
SECTIONS
{
  . = 0x80000;
  .text : { *(.text) }
}
```

### Generate kernel.img
```console
cd src
make
```
## Check on QEMU
qemu-system-aarch64 -M raspi3b -kernel kernel8.img -display none -d in_asm
or
```console
cd src
make run
```
