# Lab 1 : Hello World

## Introduction
In Lab 1, you will practice bare metal programming by implementing a simple shell. You need to set up **mini UART**, and let your host and rpi3 can communicate through it.

## Goals of this lab
- Practice bare metal programming.
- Understand how to access rpi3’s peripherals.
- Set up Mini UART.
- Set up Mailbox.

## Exercises
### Basic Exercise 1 - Basic Initialization

When a program is loaded, it requires,
- All it’s data is presented at correct memory address.
- The program counter is set to correct memory address.
- The bss segment are initialized to 0.
- The stack pointer is set to a proper address.

After rpi3 booting, its booloader loads kernel8.img to physical address 0x80000, and start executing the loaded program. **If the linker script is correct, the above two requirements are met**.

However, **both bss segment and stack pointer are not properly initialized**. Hence, you need to initialize them at very beginning. Otherwise, it may lead to undefined behaviors.

> Todo
> Initialize rpi3 after booted by bootloader.

```
link.ld
```

### Basic Exercise 2 - Mini UART

You’ll use UART as a bridge between rpi3 and host computer for all the labs. Rpi3 has 2 different UARTs, mini UART and PL011 UART. In this lab, you need to set up the mini UART.

> Todo
> Follow [UART](https://oscapstone.github.io/labs/hardware/uart.html#uart) to set up mini UART.
> https://cs140e.sergio.bz/docs/BCM2837-ARM-Peripherals.pdf

```
uart1.c
```

### Basic Exercise 3 - Simple Shell

After setting up UART, you should implement a simple shell to let rpi3 interact with the host computer. The shell should be able to execute the following commands.

| Command | Description    |
| :---:   | :---: |
| help    | print all available commands   |
| hello   | print Hello World ! |

> Todo
> There may be some text alignment issue on screen IO, think about \r\n on both input and output.

> Todo
> Implement a simple shell supporting the listed commands.

```
shell.c
```

### Basic Exercise 4 - Mailbox

ARM CPU is able to configure peripherals such as clock rate and framebuffer by calling VideoCoreIV(GPU) routines. Mailboxes are the communication bridge between them.

You can refer to [Mailbox](https://oscapstone.github.io/labs/hardware/mailbox.html#mailbox) for more information.

#### Get the hardware’s information

Get the hardware’s information is the easiest thing could be done by mailbox. Check if you implement mailbox communication correctly by verifying the hardware information’s correctness.

> Todo
> Get the hardware’s information by mailbox and print them, you should at least print board revision and ARM memory base address and size.

```
shell.c
```

## Implement
### Generate build folder & kernel8.img

```console
make
```

### Execute lab1

```console
make run
```

![image](https://user-images.githubusercontent.com/51469882/224073424-37e8510e-a8a0-4274-93b9-3510a744166a.png)
