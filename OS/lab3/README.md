# Lab3 : Exception and Interrupt

## Introduction

異常 (Exception) 是導致當前正在執行的程序將CPU讓給相應處理程序的事件。 通過異常機制，操作系統可以

1. 執行過程中出現錯誤時進行適當的處理。

2. 用戶程序可以產生異常來獲取相應操作系統的服務。

3. 外圍設備 (peripheral device) 可以強制當前正在執行的程序放棄 CPU 並執行其處理程序。

## Goals of this lab

- 了解 Armv8-A 中的異常級別。
- 了解什麼是異常處理。
- 了解什麼是中斷。
- 了解 rpi3 的外設如何通過中斷控制器來中斷 CPU。
- 了解如何復用定時器。
- 了解如何並發處理 I/O 設備。

## Background

### Official Reference

異常與 CPU 的設計緊密結合。 我們僅簡單介紹本實驗室所需的組件。 如果想了解詳細內容，請參考 ARM 官方介紹和手冊的D1章（第1496頁）

### Exception Levels

最小權限原則限制了程序可以訪問的資源。 該限制減少了執行過程中可能出現的錯誤和攻擊面，從而提高了系統的安全性和穩定性。 Armv8-A的CPU遵循該原則並實現異常級別，因此操作系統可以運行各種用戶應用程序而不會導致整個系統崩潰。


Armv8-A 有 4 個異常級別 (EL)。 一般來說，所有用戶程序都運行在EL0中，操作系統運行在EL1中。 操作系統可以通過設置系統寄存器並執行異常返回指令來降低異常級別並跳轉到用戶程序。 當用戶程序執行過程中發生異常時，異常級別會提高，CPU會跳轉到異常處理程序。

### Exception Handling

當 CPU 發生異常時，它會執行以下操作。

- 將當前處理器的狀態（PSTATE）保存在 SPSR_ELx 中。 （x 是目標異常級別）
- 將異常返回地址保存在ELR_ELx中。
- 禁用其中斷。 （PSTATE.{D,A,I,F} 設置為 1）。
- 如果異常是同步異常或 SError 中斷，則將該異常的原因保存在 ESR_ELx 中。
- 切換到目標異常級別並從相應的向量地址開始。

異常處理程序完成後，它會發出 eret 從異常中返回。 然後是CPU，

- 從 ELR_ELx 恢復程序計數器。
- 從 SPSR_ELx 恢復 PSTATE。
- 根據SPSR_ELx切換到相應的Exception級別。


#### Vector Table

如上所述，CPU從相應的向量地址開始執行。 地址定義為如下向量表，表的基地址保存在VBAR_ELx中。


表的左邊部分是異常的原因。 在本實驗中，我們僅關注同步和 IRQ 異常。 表的右側部分是異常發生的 EL 與其目標 EL 之間的關係。 在本實驗中，我們僅關注內核接受異常（EL1 -> EL1）和 64 位用戶程序接受異常（EL0 -> EL1）的情況。 另外，我們希望內核和用戶程序使用不同的堆棧（使用 SP_ELx）。 因此，它對應於表中的

> Exception from the currentEL while using SP_ELx

and

> Exception from a lower EL and at least one lower EL is AARCH64.

### Interrupts

除了計算之外，CPU還需要控制I/O設備。 I/O 設備並不總是準備就緒。 因此，CPU 在處理 I/O 設備的數據之前需要檢查設備的狀態和準備情況。

在之前的實驗中，您是通過忙於輪詢設備來完成的。 然而，冗餘輪詢會浪費CPU時間。 此外，CPU 可能不會在 I/O 設備準備就緒後立即處理該設備的數據。 它可能會導致 I/O 設備利用率不足或數據丟失。

中斷作為一種替代方式，允許 I/O 設備在準備就緒時通知 CPU。 然後，CPU 可以立即執行設備的處理程序來處理數據。 此外，中斷會強制當前正在運行的進程放棄CPU的控制權。 因此，任何進程都不能無限期地運行。

#### Interrupt Controllers

Rpi3 有兩級中斷控制器。 第一級控制器將中斷路由到每個CPU內核，因此每個CPU內核都可以有自己的定時器中斷並在彼此之間發送中斷處理器中斷。 詳細信息可以在

https://github.com/Tekki/raspberrypi-documentation/blob/master/hardware/raspberrypi/bcm2836/QA7_rev3.4.pdf

第二級控制器路由來自 UART 和系統定時器等外設的中斷，它們被聚合併作為 GPU IRQ 發送到第一級中斷控制器。 詳細信息可以在

https://cs140e.sergio.bz/docs/BCM2837-ARM-Peripherals.pdf（第109頁）

#### Critical Sections

臨界區是不能同時執行的代碼段。 啟用中斷後，CPU 在訪問某些共享數據時可能會被中斷。 如果中斷處理程序也訪問數據，則數據可能會被損壞。 因此，內核需要保護共享數據。

在基本部分，中斷僅在用戶程序（EL0）中啟用。 因此，你的內核不需要處理它。

在高級部分，您的內核應該啟用中斷。 因此，您應該
- 在關鍵部分暫時禁用 CPU 中斷，以防止中斷處理程序中的並發訪問。
- 限制中斷處理程序調用訪問共享數據的部分 API。

## Basic Exercises

### Basic Exercise 1 - Exception
#### Exception Level Switch
##### EL2 to EL1

Rpi3的CPU默認啟動後運行在EL2，但我們希望內核運行在EL1。 因此，您的內核需要一開始就切換到 EL1。

您可以使用以下代碼從 EL2 切換到 EL1。 它配置 hcr_el2，以便 EL1 在 AARCH64 中運行。 然後它設置spsr_el2和elr_el2，這樣CPU就可以在eret之後以正確的PSTATE返回到目標地址。

```
    ...
    bl from_el2_to_el1
    # the next instruction runs in EL1
    ...
from_el2_to_el1:
    mov x0, (1 << 31) // EL1 uses aarch64
    msr hcr_el2, x0
    mov x0, 0x3c5 // EL1h (SPSel = 1) with interrupt disabled
    msr spsr_el2, x0
    msr elr_el2, lr
    eret // return to EL1
```

> Todo
> 
> Switch from EL2 to EL1.

##### EL1 to EL0

內核初始化後，可以通過eret加載用戶程序並在EL0中執行。 您需要準備一個函數作為用戶程序，並向 shell 添加一個命令，該命令可以

1. 將 spsr_el1 設置為 0x3c0，將 elr_el1 設置為程序的起始地址。
2. 通過設置 sp_el0 將用戶程序的堆棧指針設置到適當的位置。
3. 發出 eret 返回用戶代碼。

> Todo
> 
> 添加一個可以在initramfs中加載用戶程序的命令。 然後，使用eret跳轉到起始地址。

> Hint
> 
> You can use QEMU and GDB to check if you do it correctly.

##### EL0 to EL1

用戶程序可以通過異常返回到 EL1。 但需要先設置異常向量表。 您可以使用以下向量表並將 vbar_el1 設置為其地址。

```
exception_handler:
  ...
.align 11 // vector table should be aligned to 0x800
.global exception_vector_table
exception_vector_table:
  b exception_handler // branch to a handler function.
  .align 7 // entry size is 0x80, .align will pad 0
  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7

  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7

  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7

  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7
  b exception_handler
  .align 7

set_exception_vector_table:
  adr x0, exception_vector_table
  msr vbar_el1, x0
```

> Important
> 
> The vector table’s base address should be aligned to 0x800

##### Exception Handling

設置向量表後，加載以下用戶程序。 用戶程序通過用於系統調用的 svc 指令獲取異常。

系統調用的設計留給下一個實驗室。 現在，您的內核只需要在異常處理程序中打印 spsr_el1、elr_el1 和 esr_el1 的內容。

```
.section ".text"
.global _start
_start:
    mov x0, 0
1:
    add x0, x0, 1
    svc 0
    cmp x0, 5
    blt 1b
1:
    b 1b
```

> Todo
> 
> Set the vector table and implement the exception handler.

##### Context saving

您可能會發現上述用戶程序的行為異常。 這是因為用戶程序和異常處理程序共享相同的通用寄存器組。 您需要在進入內核函數之前保存它們。 否則，它可能會被損壞。

您可以使用以下代碼在進入內核之前保存寄存器並在退出內核之前加載它們。

```
// save general registers to stack
.macro save_all
    sub sp, sp, 32 * 8
    stp x0, x1, [sp ,16 * 0]
    stp x2, x3, [sp ,16 * 1]
    stp x4, x5, [sp ,16 * 2]
    stp x6, x7, [sp ,16 * 3]
    stp x8, x9, [sp ,16 * 4]
    stp x10, x11, [sp ,16 * 5]
    stp x12, x13, [sp ,16 * 6]
    stp x14, x15, [sp ,16 * 7]
    stp x16, x17, [sp ,16 * 8]
    stp x18, x19, [sp ,16 * 9]
    stp x20, x21, [sp ,16 * 10]
    stp x22, x23, [sp ,16 * 11]
    stp x24, x25, [sp ,16 * 12]
    stp x26, x27, [sp ,16 * 13]
    stp x28, x29, [sp ,16 * 14]
    str x30, [sp, 16 * 15]
.endm

// load general registers from stack
.macro load_all
    ldp x0, x1, [sp ,16 * 0]
    ldp x2, x3, [sp ,16 * 1]
    ldp x4, x5, [sp ,16 * 2]
    ldp x6, x7, [sp ,16 * 3]
    ldp x8, x9, [sp ,16 * 4]
    ldp x10, x11, [sp ,16 * 5]
    ldp x12, x13, [sp ,16 * 6]
    ldp x14, x15, [sp ,16 * 7]
    ldp x16, x17, [sp ,16 * 8]
    ldp x18, x19, [sp ,16 * 9]
    ldp x20, x21, [sp ,16 * 10]
    ldp x22, x23, [sp ,16 * 11]
    ldp x24, x25, [sp ,16 * 12]
    ldp x26, x27, [sp ,16 * 13]
    ldp x28, x29, [sp ,16 * 14]
    ldr x30, [sp, 16 * 15]
    add sp, sp, 32 * 8
.endm

exception_handler:
    save_all
    bl exception_entry
    load_all
    eret
```

> Todo
> 
> 在執行異常處理程序之前保存用戶程序的上下文。

## Basic Exercise 2 - Interrupt
### Core Timer Interrupt

Rpi3的每個CPU核心都有其核心定時器。 它可以通過以下系統寄存器進行配置。

> cntpct_el0：定時器的當前計數。
> cntp_cval_el0：比較的計時器計數。 如果cntpct_el0 >= cntp_cval_el0，則中斷CPU核心。
> cntp_tval_el0：（cntp_cval_el0 - cntpct_el0）。 您可以使用它在當前計時器計數後設置過期計時器。

要啟用定時器的中斷，您需要

1. 將 cntp_ctl_el0 設置為 1。
2. 從第一級中斷控制器取消屏蔽定時器中斷。
3. 你應該啟用CPU核心的中斷。

在基礎部分，只需要在EL0中使能中斷即可。 您可以通過在返回 EL0 之前將 spsr_el1 設置為 0 來完成此操作。

您可以使用以下代碼來啟用核心定時器的中斷。

```
#define CORE0_TIMER_IRQ_CTRL 0x40000040

core_timer_enable:
  mov x0, 1
  msr cntp_ctl_el0, x0 // enable
  mrs x0, cntfrq_el0
  msr cntp_tval_el0, x0 // set expired time
  mov x0, 2
  ldr x1, =CORE0_TIMER_IRQ_CTRL
  str w0, [x1] // unmask timer interrupt

core_timer_handler:
  mrs x0, cntfrq_el0
  msr cntp_tval_el0, x0
```

> Todo
> 
> 啟用核心定時器的中斷。 中斷處理程序應打印啟動後的秒數，並將下一次超時設置為 2 秒後。

> Hint
> 
> 您可以通過計時器的計數（cntpct_el0）和計時器的頻率（cntfrq_el0）獲取啟動後的秒數。

## Basic Exercise 3 - Rpi3’s Peripheral Interrupt

在本高級部分中，您需要實現 rpi3 的 mini UART 的中斷處理。 然後，您就不必忙於輪詢 UART 設備。

### Enable mini UART’s Interrupt.

要使能mini UART的中斷，需要設置AUX_MU_IER_REG(0x3f215044)和二級中斷控制器的Enable IRQs1(0x3f00b210)的bit29。

### Determine the Interrupt Source

當UART中斷使能時，CPU就有多個中斷源。 因此，您的內核需要在執行相應的中斷處理程序之前檢查中斷源。 請參考兩個中斷控制器的手冊來確定中斷源。

### Asynchronous Read and Write

在之前的實驗中，您的 shell 在需要讀取或寫入時通過忙於輪詢 UART 來阻止執行。 現在，您可以創建一個讀緩衝區和一個寫緩衝區。 當 shell 打印消息時，它會將字節寫入寫入緩衝區。 數據由 UART 的 TX 中斷處理程序異步發送。 此外，UART 的 RX 中斷處理程序將數據放入讀緩衝區。 shell 從緩衝區讀取字節數組並獲取讀取的字節數。

> Todo
> 
> 通過中斷處理程序實現異步UART 讀/寫。

> NOte
> 
> 您不必將所有打印功能替換為異步版本。

## Advanced Exercises





