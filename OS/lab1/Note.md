### GDB
#### Introduction
除錯器 (debugger)，可以在一個精準受控的環境下執行另一個程式。例如: 單步執行程式，跟蹤程式，查看變數內容，記憶體地址，以及程式中每一條指令指行完畢後 CPU 暫存器的變化情況，檢視程式呼叫堆疊等等。

gdb，全名為 gnu debugger，是在 GNU 軟體系統中的標準除錯器，介面為互動式的 shell，許多類 Unix，如 : FreeBSD, Linux 等作業系統中都能夠使用，支援許多語言，包括 C, C++ 等。

### File explanation
1. src / link.ld
2. src / boot.S
3. src / main.c
4. src / uart1.c
5. src / shell.c
6. src / mbox.c


- src / **link.ld**
```
// 一個連結器腳本 (Linker Script)

SECTIONS
{
    . = 0x80000;                                       // 指示將當前地址位置設置為 0x80000
    .text : { *(.text.boot) *(.text) }                 // 此部分包含可執行程式碼
                                                       // 首先放置 .text.boot 部分的內容，然後放置主要的 .text 部分的內容
    .rodata : { *(.rodata) }                           // 此部分包含 read-only data
    .data : { *(.data) }                               // 此部分包含初始化的 data
    .bss : {                                           // 此部分包含未初始化的 data
        . = ALIGN(16);                                 // ALIGN(16) 指示確保 .bss 部分的開始地址對齊到 16-byte
        __bss_start = .;                               // .bss 起始
        *(.bss)                                        //
        __bss_end = .;                                 // .bss 結束
    }
}

__bss_size = (__bss_end - __bss_start) >> 3;           // 使用 __bss_start 和 __bss_end 符號來計算 .bss 部分的大小
                                                       // 並將其存儲在 __bss_size 變量中
```

<img src= https://user-images.githubusercontent.com/51469882/224490626-35d6ab1f-2752-4adc-a249-fb99b732e884.png >

- src / **boot.S**
```Assembly
// 這是一段 ARM64 位元組的組合語言程式碼，用於在 Raspberry Pi 3 上啟動並運行程序

.section ".text.boot"                      // 表示這是一個屬於 .text.boot 段的程式碼
                                           // .text.boot 段是一個特殊的段，其中**包含在執行 main() 函数之前需要執行的初始化代碼**
                                           // 因此，在這個段中的程式碼將會在程式運行之前被執行。

.global _start                             // _start 函数是程序的起點，其代碼的作用是執行必要的初始化，然後調用 main 函数。

_start:                                    // _start 函数
    ldr     x1, =_start                    // _start 首先將 _start 函数的地址載入 x1 寄存器
    mov     sp, x1                         // 然後將 x1 的值設置為 sp (stack pointer)，以確保 sp 指向 _start 函數的末尾
    ldr     x1, =__bss_start               // __bss_start 的地址並將其載入到 x1 中。
                                           // __bss_start 是 BSS 段的起始地址，BSS 段是一個特殊的段，
                                           // 其中包含尚未初始化的全局變量和靜態變量。
                                           // 在這裡，我們需要清空 BSS 段，以確保所有變量都被初始化為零。
    ldr     w2, =__bss_size                // 將 __bss_size 的值其載入到 w2 中
                                           
clear_bss:                                 // __bss_size 是 BSS 段的大小，因此 _start 將使用這個值來迭代清空整個 BSS 段
    cbz     w2, run_main                   // 使用 cbz 指令來檢查 w2 的值是否為零，如果是零則跳轉到 run_main 標記所在的位置
    str     xzr, [x1], #8                  // 否則使用 str 指令將零寫入 x1 所指向的內存地址中
                                           // 然後將 x1 加上 8-byte 以便移動到下一個內存位置
    sub     w2, w2, #1                     // 同時將 w2 減去 1 以便在下一次迭代中減少清空的內存區域
    cbnz    w2, clear_bss                  // 使用 cbnz 指令來檢查 w2 的值是否為零
                                           // 如果不是零 (即還有更多 .bss 區段需要清除) 則跳轉回 clear_bss 標記所在的位置繼續清空 BSS 段

run_main:
    bl      main
proc_hang:                                 // 這是一個死循環，該循環通過等待事件的發生來防止系統在程序結束時崩潰或停止運行
    wfe                                    // 這是一個 ARM 處理器指令，它讓處理器進入低功耗待機模式，直到外部事件發生
    b       proc_hang                      // 這是一個無條件的分支指令，它將程序的控制權永遠轉移到 proc_hang 標籤所在的位置，使程序繼續進入死循環
    
// 這段程式碼的主要目的是在系統啟動時清除未初始化的全局數據區段，然後調用 main 函數。
// 一旦 main 函數返回，程序將進入一個死循環，直到外部事件發生。
```

- src / **main.c**
```
#include "uart1.h"                                  
#include "shell.h"                                  
void main(){                                        
    char input_buffer[CMD_MAX_LEN];                 // 用於存儲用戶端輸入的命令字符串。
                                                    // CMD_MAX_LEN 是一個預先定義的常量，它表示命令的最大長度

    uart_init();                                    // 初始化 UART1 的 protocal
    cli_print_banner();                             // print CLI 的歡迎界面。之後進入一個死循環，每次從用戶端讀取一個命令，然後執行該命令

    while(1){                                       // 進入一個死循環，不斷接收用戶輸入的命令
        cli_cmd_clear(input_buffer, CMD_MAX_LEN);   // 清空 input_buffer 數組
        uart_puts("# ");                            // 將提示符輸出到 UART1 protocal，提示用戶輸入命令
        cli_cmd_read(input_buffer);                 // 從 UART1 串口讀取用戶輸入的命令，並存儲到 input_buffer array 中
        cli_cmd_exec(input_buffer);                 // 解析並執行用戶輸入的命令
    }
}
```

- src / **uart1.c**
```
// 這是一個用於 Raspberry Pi 的嵌入式系統的 UART 库，用於與串行通信接口進行通信。

#include "bcm2837/rpi_gpio.h"
#include "bcm2837/rpi_uart1.h"
#include "uart1.h"

// 用於初始化 UART，其中使用 Broadcom BCM2837 SoC 的寄存器來設置 UART。
// 它啟用 UART，設置數據位大小、波特率、流控制和映射到 GPIO 引腳。
void uart_init()                  
{
    register unsigned int r;

    /* initialize UART */
    *AUX_ENABLES     |= 1;       // 啟用 UART1
    *AUX_MU_CNTL_REG  = 0;       // 禁用 TX/RX

    /* configure UART */
    *AUX_MU_IER_REG   = 0;       // 禁用中斷
    *AUX_MU_LCR_REG   = 3;       // 設定資料大小為 8 位元組
    *AUX_MU_MCR_REG   = 0;       // 禁用流量控制
    *AUX_MU_BAUD_REG  = 270;     // 設定傳輸速率為 115200 baud rate
    *AUX_MU_IIR_REG   = 6;       // 禁用 FIFO

    /* map UART1 to GPIO pins */
    r = *GPFSEL1;
    r &= ~(7<<12);               // 清除 gpio14 的設定
    r |= 2<<12;                  // 設定 gpio14 為 alt5
    r &= ~(7<<15);               // 清除 gpio15 的設定
    r |= 2<<15;                  // 設定 gpio15 為 alt5
    *GPFSEL1 = r;

    /* enable pin 14, 15 - ref: Page 101 */
    *GPPUD = 0;
    r=150; while(r--) { asm volatile("nop"); }     // 短暫延遲
    *GPPUDCLK0 = (1<<14)|(1<<15);
    r=150; while(r--) { asm volatile("nop"); }     // 短暫延遲
    *GPPUDCLK0 = 0;

    *AUX_MU_CNTL_REG = 3;      // 啟用 TX/RX
}

// 用於接收一個字符，並等待接收到字符，然後返回該字符，但在返回之前將換行符轉換為換行符。
char uart_recv() {
    char r;
    while(!(*AUX_MU_LSR_REG & 0x01)){};  // 一個忙等迴圈，它會等待 UART 接收到一個字元。
                                         // AUX_MU_LSR_REG 註冊是 BCM2835 芯片中包含 UART 狀態的註冊。
                                         // & 0x01 操作將除了最低有效位之外的所有位掩蓋，該位指示接收器先進先出（FIFO）緩衝區中是否有數據。
                                         // 迴圈會一直執行，直到此位被設置為 1，表示緩衝區中有數據。
    r = (char)(*AUX_MU_IO_REG);          // 程式碼通過讀取 AUX_MU_IO_REG 註冊來從 UART 接收緩衝區讀取一個字元。
                                         // 該字元被強制轉換為 char 類型並存儲在變數 r 中。
    return r=='\r'?'\n':r;               // 程式碼檢查接收到的字元是否為回車符（\r），如果是，則將其轉換為換行符（\n）並返回。
                                         // 這樣做是為了規範行結束符號，因為不同的平台可能使用不同的行結束符號。
                                         // 如果接收到的字符不是回车符，則原樣返回。
}

// 用於發送一個字符，並等待發送緩衝區為空，然後將字符寫入緩衝區。
void uart_send(unsigned int c) {
    while(!(*AUX_MU_LSR_REG & 0x20)){};  // 檢查 AUX_MU_LSR_REG 寄存器的第 6 個位元（二進位制下的第 5 個位元）是否為 1。
                                         // 這個位元稱為 THRE（Transmitter Holding Register Empty）
                                         // 如果為 1，表示 UART 的發送緩衝區已經空了，可以開始進行新的發送操作
                                         // 如果為 0，則表示 UART 的發送緩衝區還沒有空出來，需要繼續等待
    *AUX_MU_IO_REG = c;                  // 當 THRE 位元為 1 時，表示 UART 的發送緩衝區已經可以開始進行新的發送操作
                                         // 該函數就會使用 *AUX_MU_IO_REG = c 將新的字符寫入 AUX_MU_IO_REG 寄存器，從而發送出去
    // 由於該函數需要等待 UART 完成先前的發送操作，因此可以確保 UART 的發送緩衝區不會過載，從而提高了通訊的可靠性
}

// 用於發送一個字符串，將換行符轉換為回車換行符，並逐個字符發送。
void uart_puts(char *str) {
    while(*str) {                        // 逐個字符地迭代指向 str 的字串。循環會持續到達到字串的結尾，即空字符 ('\0')
        if(*str=='\n')                   // 檢查當前字符是否是換行符 ('\n')。如果是，會使用 uart_send 函數發送回車符 ('\r')
            uart_send('\r');
        uart_send(*str++);               // 將指針 str 指向字串中的下一個字符
    }
}

// 用於將一個 32 位 unsigned 整數轉換成兩個十六進制數位並發送到 UART 介面上。
void uart_2hex(unsigned int d) {
    unsigned int n;
    int c;
    for(c=28;c>=0;c-=4) { 
        n=(d>>c)&0xF;
        n+=n>9?0x37:0x30;          // 判斷 n 是否大於 9。
                                   // 如果是，則表示這是一個大於 9 的字母，需要加上 0x37 得到 ASCII 碼；
                                   // 如果不是，則表示這是一個數字，需要加上 0x30 得到 ASCII 碼。
        uart_send(n);              // 將 ASCII 碼發送到 UART 介面上
    }
}
```

- src / **shell.c**
```
// 這段程式碼包含了一個簡單的命令行界面（CLI），可以在一個 Raspberry Pi 上運行。它定義了一些命令，例如 "hello"、"help"、"info" 和 "reboot"，用於打印消息、查看設備信息以及重新啟動設備。

#include "shell.h"
#include "uart1.h"
#include "mbox.h"
#include "power.h"

// 宣告一個 CLI 指令清單，使用結構體陣列表示
// 這個清單最多可以容納 CLI_MAX_CMD 個指令
struct CLI_CMDS cmd_list[CLI_MAX_CMD]=
{
    {.command="hello", .help="print Hello World!"},                 // 第一個指令 "hello"，說明為 "print Hello World!"
    {.command="help", .help="print all available commands"},        // 第二個指令 "help"，說明為 "print all available commands"
    {.command="info", .help="get device information via mailbox"},  // 第三個指令 "info"，說明為 "get device information via mailbox"
    {.command="reboot", .help="reboot the device"}                  // 第四個指令 "reboot"，說明為 "reboot the device"
};

// 比較命令
int cli_cmd_strcmp(const char* p1, const char* p2)
{
    const unsigned char *s1 = (const unsigned char*) p1;     // 將指向第一個字串的指標轉換為 unsigned char 型別的指標
    const unsigned char *s2 = (const unsigned char*) p2;     // 將指向第二個字串的指標轉換為 unsigned char 型別的指標
    unsigned char c1, c2;                                    // c1, c2 分別代表兩個字串中目前正在比較的字元

    do {
        c1 = (unsigned char) *s1++;                          // 取出 s1 所指的字元，並將 s1 往後移一個位置
        c2 = (unsigned char) *s2++;                          // 取出 s2 所指的字元，並將 s2 往後移一個位置
        if ( c1 == '\0' ) return c1 - c2;                    // 如果 s1 已經比較到字串結尾，則回傳 s1 和 s2 最後一個字元的差值
    } while ( c1 == c2 );                                    // 如果兩個字元相同，繼續比較下一個字元
    return c1 - c2;                                          // 如果兩個字元不同，則回傳它們的差值
} 

// 清除緩衝區
void cli_cmd_clear(char* buffer, int length)
{
    for(int i=0; i<length; i++)
    {
        buffer[i] = '\0';      // 使用一個 for 迴圈來遍歷緩衝區中的所有元素，並將它們設置為空字符 '\0'，以清空緩衝區
    }
};

// 讀取用戶輸入
void cli_cmd_read(char* buffer)
{
    char c='\0';
    int idx = 0;
    while(1)
    {
        // 該函數使用一個 while 迴圈不斷讀取 UART 中的字符，直到讀取到換行符 '\n' 為止，表示字符序列結束。
        // 在讀取字符的過程中，如果字符序列的長度超過了緩衝區的最大長度 CMD_MAX_LEN，則函數會直接退出。
        
        if ( idx >= CMD_MAX_LEN ) break;

        c = uart_recv();
        if ( c == '\n')
        {
            uart_puts("\r\n");
            break;
        }
        
        // 如果讀取到的字符的 ASCII 編碼 介於 16 與 32 之間 或者大於 127，則表示它是一個非可見字符，該字符會被忽略。
        if ( c > 16 && c < 32 ) continue;
        if ( c > 127 ) continue;
        
        // 最後，函數將每個讀取到的可見字符儲存在緩衝區中，並將它們通過 UART 發送回主機端
        buffer[idx++] = c;
        uart_send(c);
    }
}

// 執行命令
void cli_cmd_exec(char* buffer)
{
    if (cli_cmd_strcmp(buffer, "hello") == 0) {
        do_cmd_hello();
    } else if (cli_cmd_strcmp(buffer, "help") == 0) {
        do_cmd_help();
    } else if (cli_cmd_strcmp(buffer, "info") == 0) {
        do_cmd_info();
    } else if (cli_cmd_strcmp(buffer, "reboot") == 0) {
        do_cmd_reboot();
    } else if (*buffer){
        uart_puts(buffer);
        uart_puts(": command not found\r\n");
    }
}

void cli_print_banner()
{
    uart_puts("=======================================\r\n");
    uart_puts("  Welcome to NYCU-OSC 2023 Lab1 Shell  \r\n");
    uart_puts("=======================================\r\n");
}

void do_cmd_help()
{
    for(int i = 0; i < CLI_MAX_CMD; i++)
    {
        uart_puts(cmd_list[i].command);
        uart_puts("\t\t: ");
        uart_puts(cmd_list[i].help);
        uart_puts("\r\n");
    }
}

void do_cmd_hello()
{
    uart_puts("Hello World!\r\n");
}

void do_cmd_info()
{
    // print hw revision
    pt[0] = 8 * 4;
    pt[1] = MBOX_REQUEST_PROCESS;
    pt[2] = MBOX_TAG_GET_BOARD_REVISION;
    pt[3] = 4;
    pt[4] = MBOX_TAG_REQUEST_CODE;
    pt[5] = 0;
    pt[6] = 0;
    pt[7] = MBOX_TAG_LAST_BYTE;

    if (mbox_call(MBOX_TAGS_ARM_TO_VC, (unsigned int)((unsigned long)&pt)) ) {
        uart_puts("Hardware Revision\t: ");
        uart_2hex(pt[6]);
        uart_2hex(pt[5]);
        uart_puts("\r\n");
    }
    // print arm memory
    pt[0] = 8 * 4;
    pt[1] = MBOX_REQUEST_PROCESS;
    pt[2] = MBOX_TAG_GET_ARM_MEMORY;
    pt[3] = 8;
    pt[4] = MBOX_TAG_REQUEST_CODE;
    pt[5] = 0;
    pt[6] = 0;
    pt[7] = MBOX_TAG_LAST_BYTE;

    if (mbox_call(MBOX_TAGS_ARM_TO_VC, (unsigned int)((unsigned long)&pt)) ) {
        uart_puts("ARM Memory Base Address\t: ");
        uart_2hex(pt[5]);
        uart_puts("\r\n");
        uart_puts("ARM Memory Size\t\t: ");
        uart_2hex(pt[6]);
        uart_puts("\r\n");
    }
}

// 這個函數是一種使用內存映射寄存器控制硬件的簡單方式，用於在嵌入式系統中執行系統重新啟動。
void do_cmd_reboot()
{
    uart_puts("Reboot in 5 seconds ...\r\n\r\n");
    volatile unsigned int* rst_addr = (unsigned int*)PM_RSTC;      // 定義了一個名為“rst_addr”的易失性指向無符號整數的指針，其初始化為一個名為“PM_RSTC”的內存映射寄存器的地址
    *rst_addr = PM_PASSWORD | 0x20;                                // 將“rst_addr”寄存器的值設置為密碼值和0x20的位 or。這將觸發系統重置的效果。
    volatile unsigned int* wdg_addr = (unsigned int*)PM_WDOG;      // “wdg_addr”，被定義並初始化為一個名為“PM_WDOG”的內存映射寄存器的地址。
    *wdg_addr = PM_PASSWORD | 5;                                   // 將 “wdg_addr” 暫存器的值設置為密碼值和5的位 or。   
                                                                   // 在定時器上設置了一個計時器，在5秒內如果未清除，將導致系統重置
}
```

- src / **mbox.c**
```
// 將訊息發送到 Raspberry Pi 的 VideoCore (VC) 處理器上的郵箱通道並等待回應。

#include "bcm2837/rpi_mbox.h"
#include "mbox.h"

/* Aligned to 16-byte boundary while we have 28-bits for VC */
// pt 數組聲明為 volatile 並對齊到 16-byte，可能是為了確保正確的記憶體訪問並避免快取行可能存在的問題
volatile unsigned int  __attribute__((aligned(16))) pt[64];

// mbox_channel_type channel：要發送訊息的郵箱通道。
// unsigned int value：要發送的訊息的值。

int mbox_call( mbox_channel_type channel, unsigned int value )
{
    // Add channel to lower 4 bit                             // 將 value 參數的 lower 4 bit 設置為 channel 參數的值來修改 value 參數
    value &= ~(0xF);
    value |= channel;
    while ( (*MBOX_STATUS & BCM_ARM_VC_MS_FULL) != 0 ) {}     // 等待郵箱狀態寄存器變為非滿狀態
    // Write to Register
    *MBOX_WRITE = value;                                      // 將修改後的 value 寫入郵箱寫暫存器
    while(1) {                                                // 進入一個迴圈
        while ( *MBOX_STATUS & BCM_ARM_VC_MS_EMPTY ) {}       // 等待郵箱狀態寄存器變為非空狀態，然後從郵箱讀取寄存器中讀取值。
        // Read from Register
        if (value == *MBOX_READ)                              // 如果讀取值與發送的值匹配
            return pt[1] == MBOX_REQUEST_SUCCEED;             // 檢查 pt[1]（假定該元素包含郵箱回應），如果它表明請求成功，則返回1。
                                                              // 否則，函式繼續等待匹配的回應。
    }
    return 0;                                                 // 如果函式在沒有找到匹配的回應的情況下退出迴圈，則返回0。
}
```
