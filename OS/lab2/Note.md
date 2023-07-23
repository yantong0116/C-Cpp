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



