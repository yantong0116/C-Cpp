### Make File

- link.ld

```
_heap_stack_size = 1M;  // 定義堆和棧的大小為 1MB

SECTIONS
{
    /* bootloader 0x60000, kernel 0x80000 */
    . = 0x80000;   // 將當前位置（鏈接地址）設置為 0x80000，即內存中的 0x80000 地址處
    _start = .;   // 將標籤 "_start" 設置為當前位置

    // .text 段：包含 ".text.boot" 和 ".text" 部分
    .text : { *(.text.boot) *(.text) }

    // .rodata 段：只包含 ".rodata" 部分
    .rodata : { *(.rodata) }

    // .data 段：只包含 ".data" 部分
    .data : { *(.data) }

    // .bss 段：只包含 ".bss" 部分
    .bss : {
        _bss_top = .;   // 將標籤 "_bss_top" 設置為當前位置
        *(.bss)   // 將所有位於 .bss 段的變量和未初始化的數據放置在此
    }

    // 計算 .bss 段的大小，並將結果保存在 _bss_size 變量中
    _bss_size = SIZEOF(.bss) >> 3;  // 這裡用右移 3 位，相當於除以 8，以字節為單位

    // .heap 段：用於堆（heap）的起始位置
    .heap : {
        . = ALIGN(8);   // 將當前位置對齊到 8 字節邊界
        _heap_top = .;   // 將標籤 "_heap_top" 設置為當前位置
    }

    // 將當前位置向前移動 _heap_stack_size 字節，即將堆和棧的大小預留在內存中
    . = . + _heap_stack_size;

    // .stack 段：用於棧（stack）的起始位置
    .stack : {
        . = ALIGN(8);   // 將當前位置對齊到 8 字節邊界
        _stack_top = .;   // 將標籤 "_stack_top" 設置為當前位置
    }

    _end = .;   // 將標籤 "_end" 設置為當前位置
}
```

- send_image_to_bootloader.py

```python
#!/usr/bin/env python3

from serial import Serial
from pwn import *
import argparse
from sys import platform

if platform == "linux" or platform == "linux2":
    parser = argparse.ArgumentParser(description='NYCU OSDI kernel sender')
    parser.add_argument('--filename', metavar='PATH', default='kernel8.img', type=str, help='path to kernel8.img')
    parser.add_argument('--device', metavar='TTY',default='/dev/ttyUSB0', type=str,  help='path to UART device')
    parser.add_argument('--baud', metavar='Hz',default=115200, type=int,  help='baud rate')
    args = parser.parse_args()

    with open(args.filename,'rb') as fd:
        with Serial(args.device, args.baud) as ser:

            kernel_raw = fd.read()
            length = len(kernel_raw)

            print("Kernel image size : ", hex(length))
            for i in range(8):
                ser.write(p64(length)[i:i+1])
                ser.flush()

            print("Start sending kernel image by uart1...")
            for i in range(length):
                # Use kernel_raw[i: i+1] is byte type. Instead of using kernel_raw[i] it will retrieve int type then cause error
                ser.write(kernel_raw[i: i+1])
                ser.flush()
                if i % 100 == 0:
                    print("{:>6}/{:>6} bytes".format(i, length))
            print("{:>6}/{:>6} bytes".format(length, length))
            print("Transfer finished!")

else:
    parser = argparse.ArgumentParser(description='NYCU OSDI kernel sender')
    parser.add_argument('--filename', metavar='PATH', default='kernel8.img', type=str, help='path to kernel8.img')
    parser.add_argument('--device', metavar='COM',default='COM3', type=str,  help='COM# to UART device')
    parser.add_argument('--baud', metavar='Hz',default=115200, type=int,  help='baud rate')
    args = parser.parse_args()

    with open(args.filename,'rb') as fd:
        with Serial(args.device, args.baud) as ser:

            kernel_raw = fd.read()
            length = len(kernel_raw)

            print("Kernel image size : ", hex(length))
            for i in range(8):
                ser.write(p64(length)[i:i+1])
                ser.flush()

            print("Start sending kernel image by uart1...")
            for i in range(length):
                # Use kernel_raw[i: i+1] is byte type. Instead of using kernel_raw[i] it will retrieve int type then cause error
                ser.write(kernel_raw[i: i+1])
                ser.flush()
                if i % 100 == 0:
                    print("{:>6}/{:>6} bytes".format(i, length))
            print("{:>6}/{:>6} bytes".format(length, length))
            print("Transfer finished!")
```

- uart1.c

```c
#include "bcm2837/rpi_gpio.h"
#include "bcm2837/rpi_uart1.h"
#include "uart1.h"
#include "utils.h"

void uart_init()
{
    register unsigned int r;

    /* initialize UART */
    *AUX_ENABLES     |= 1;       // enable UART1
    *AUX_MU_CNTL_REG  = 0;       // disable TX/RX

    /* configure UART */
    *AUX_MU_IER_REG   = 0;       // disable interrupt
    *AUX_MU_LCR_REG   = 3;       // 8 bit data size
    *AUX_MU_MCR_REG   = 0;       // disable flow control
    *AUX_MU_BAUD_REG  = 270;     // 115200 baud rate
    *AUX_MU_IIR_REG   = 6;       // disable FIFO

    /* map UART1 to GPIO pins */
    r = *GPFSEL1;
    r &= ~(7<<12);               // clean gpio14
    r |= 2<<12;                  // set gpio14 to alt5
    r &= ~(7<<15);               // clean gpio15
    r |= 2<<15;                  // set gpio15 to alt5
    *GPFSEL1 = r;

    /* enable pin 14, 15 - ref: Page 101 */
    *GPPUD = 0;
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }
    *GPPUDCLK0 = 0;

    *AUX_MU_CNTL_REG = 3;      // enable TX/RX
}

char uart_getc() {
    char r;
    while(!(*AUX_MU_LSR_REG & 0x01)){};
    r = (char)(*AUX_MU_IO_REG);
    return r;
}

char uart_recv() {
    char r;
    while(!(*AUX_MU_LSR_REG & 0x01)){};
    r = (char)(*AUX_MU_IO_REG);
    return r=='\r'?'\n':r;
}

// 0x20 是因為 UART Line Status Register (LSR) 的第 5 位是 Transmitter Empty (TEMT) 位，而不是第 4 位。
// UART LSR 寄存器是一個 8 位寄存器，每一位表示 UART 的不同狀態。在 LSR 寄存器中，第 5 位（從右往左數）對應於 Transmitter Empty (TEMT) 位。當該位被設置為 1 時，表示 UART 發送保持寄存器 (THR) 是空的，即 UART 已準備好接收新的數據進行發送。

void uart_send(unsigned int c) {
    while(!(*AUX_MU_LSR_REG & 0x20)){};
    *AUX_MU_IO_REG = c;
}

int uart_puts(char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    char buf[VSPRINT_MAX_BUF_SIZE];

    char *str = (char*)buf;
    int count = vsprintf(str,fmt,args);

    while(*str) {
        if(*str=='\n')
            uart_send('\r');
        uart_send(*str++);
    }
    __builtin_va_end(args);
    return count;
}

void uart_2hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) {
        n=(d>>c)&0xF;
        n+=n>9?0x37:0x30;
        uart_send(n);
    }
}
```

- cpio.c

```c
// 這段代碼是一個用於解析cpio新c格式的文件頭的函數。 
// cpio是一個用於歸檔和備份文件的Unix工具，而cpio新c格式是cpio的一種變種，它使用固定的ASCII格式頭來描述歸檔中的文件和目錄。

#include "cpio.h"
#include "utils.h"

/* Parse an ASCII hex string into an integer. */
static unsigned int parse_hex_str(char *s, unsigned int max_len)
{
    unsigned int r = 0;

    for (unsigned int i = 0; i < max_len; i++) {
        r *= 16;
        if (s[i] >= '0' && s[i] <= '9') {
            r += s[i] - '0';
        }  else if (s[i] >= 'a' && s[i] <= 'f') {
            r += s[i] - 'a' + 10;
        }  else if (s[i] >= 'A' && s[i] <= 'F') {
            r += s[i] - 'A' + 10;
        } else {
            return r;
        }
    }
    return r;
}


/* 將路徑名，數據，下一個標頭寫入相應的參數 */
/* 如果沒有下一個標頭，則next_header_pointer = 0 */
/* 返回-1如果解析錯誤 */
int cpio_newc_parse_header(struct cpio_newc_header *this_header_pointer, char **pathname, unsigned int *filesize, char **data, struct cpio_newc_header **next_header_pointer)
{
    /* Ensure magic header exists. */
    if (strncmp(this_header_pointer->c_magic, CPIO_NEWC_HEADER_MAGIC, sizeof(this_header_pointer->c_magic)) != 0) return -1;

    // transfer big endian 8 byte hex string to unsigned int and store into *filesize
    *filesize = parse_hex_str(this_header_pointer->c_filesize,8);

    // end of header is the pathname
    *pathname = ((char *)this_header_pointer) + sizeof(struct cpio_newc_header); 

    // get file data, file data is just after pathname
    unsigned int pathname_length = parse_hex_str(this_header_pointer->c_namesize,8);
    unsigned int offset = pathname_length + sizeof(struct cpio_newc_header);
    // The file data is padded to a multiple of four bytes
    offset = offset % 4 == 0 ? offset:(offset+4-offset%4);
    *data = (char *)this_header_pointer+offset;

    // get next header pointer
    if(*filesize==0)
    {
        *next_header_pointer = (struct cpio_newc_header*)*data;
    }else{
        offset = *filesize;
        *next_header_pointer = (struct cpio_newc_header*)(*data + (offset%4==0?offset:(offset+4-offset%4)));
    }

    // if filepath is TRAILER!!! means there is no more files.
    if(strncmp(*pathname,"TRAILER!!!", sizeof("TRAILER!!!"))==0)
    {
        *next_header_pointer = 0;
    }

    return 0;
}
```

- heap.c

```c
#include "heap.h"

// 聲明一個名為 `_heap_top` 的全局變量，該變量用於表示堆內存的頂部。
extern char _heap_top;
// 定義一個名為 `htop_ptr` 的靜態變量，並將其初始化為 `_heap_top` 變量的地址。這將用於跟踪堆內存的當前位置。
static char* htop_ptr = &_heap_top;

// 用於在堆內存中分配一塊指定大小的內存空間。它接受一個參數 `size`，表示需要分配的內存大小，返回一個指向分配的內存塊的指針。
void* malloc(unsigned int size) {
    // -> htop_ptr
    // htop_ptr + 0x02:  heap_block size
    // htop_ptr + 0x10 ~ htop_ptr + 0x10 * k:
    //            { heap_block }
    // -> htop_ptr

    // 在堆內存中分配的內存塊將包含一個 16 字節（0x10）的頭部信息
    char* r = htop_ptr + 0x10;
    // 將要分配的內存大小調整為 16 字節對齊，即向上取整到最接近的 16 字節倍數
    size = 0x10 + size - size % 0x10;
    // 在內存塊頭部（偏移量為 -0x8）存儲該內存塊的大小。
    *(unsigned int*)(r - 0x8) = size;
    // 將 `htop_ptr` 更新為指向下一個可用的堆內存地址，即將其移動到剛分配的內存塊之後
    htop_ptr += size;
    // 返回指向分配內存塊的指針
    return r;
}
```


