# Lab 2 : Booting

## Introduction

**啟動 (Booting)**是**計算機重置後設置運行各種用戶程序的環境的過程**。 它包括由引導加載程序加載的內核、子系統初始化、設備驅動程序匹配以及加載 init 用戶程序以在用戶空間中啟動其餘服務。

在實驗 2 中，您將學習加載內核和用戶程序的方法之一。 此外，您還將了解如何將設備與 rpi3 上的驅動程序相匹配。 其餘子系統的初始化將在後面的實驗中介紹。

## Goals of this lab
- 實現一個通過 UART 加載內核映像的引導加載程序。
- 實現一個簡單的分配器。
- 了解什麼是初始 ramdisk。
- 了解什麼是 devicetree。

## Background
### How a Kernel is Loaded on Rpi3
在內核開始執行之前有 4 個步驟。

1. GPU 從 SoC 上的 ROM 執行第一階段引導加載程序。
2. 第一階段引導加載程序識別 FAT16/32 文件系統，並將第二階段引導加載程序 bootcode.bin 從 SD 卡加載到二級緩存。
3. bootcode.bin初始化SDRAM並加載start.elf
4. start.elf 讀取配置以將內核和其他數據加載到內存中，然後喚醒 CPU 開始執行。

步驟 4 加載的內核也可以是另一個具有更強大功能的引導加載程序，例如網絡引導或 ELF 加載。

在實驗 2 中，您將**實現一個通過 UART 加載實際內核的引導加載程序，並由前一階段的引導加載程序加載**。

## Exercises
### Basic Exercise 1 - UART Bootloader

在實驗 1 中，您可能會在調試過程中經常經歷在主機和 rpi3 之間移動 SD 卡的過程。 **您可以通過引入另一個引導加載程序來加載正在調試的內核來消除此問題。**

要通過 UART 發送二進製文件，您應該設計一個協議來讀取原始數據。 它在傳輸過程中很少丟失數據，因此您可以保持協議簡單。

在Linux中可以通過串口設備的設備文件有效地將數據從主機寫入rpi3。

```
with open('/dev/ttyUSB0', "wb", buffering = 0) as tty:
  tty.write(...)
```

> __Hint__
> 
> You can use <font color="red">qemu-system-aarch64 -serial null -serial pty -M raspi3b</font> to create a pseudo TTY device and test your bootloader through it.

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/ca5506ce-134f-41e8-898c-82d0202be701)

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/124f2f9b-5e3b-4be6-97c2-3bf6b6b4a2d8)

![image](https://github.com/yantong0116/C-Cpp/assets/51469882/55d41cb6-3609-4c6f-9a46-a31f26ee6fb1)

#### Config Kernel Loading Setting

您可能仍然想在 0x80000 處加載實際的內核映像，但它會與引導加載程序重疊。 您可以首先通過**重寫鏈接器腳本**來指定另一個起始地址。 然後，在SD卡的啟動分區添加**config.txt**文件，通過**kernel_address=**指定加載地址。

為了進一步使您的引導加載程序與實際內核不那麼模糊，您可以通過 **kernel=** 和 **arm_64bit=1** 添加加載映像名稱

```
kernel_address=0x60000
kernel=bootloader.img
arm_64bit=1
```

> **Todo**
> 
> Implement a UART bootloader that loads kernel images through UART.

> **Important**
> 
> UART 是一種低速接口。 發送你的 kernel image 是可以的，因為它很小。 不要用它來發送大型二進製文件。

```
send_image_to_bootloader.py
```

### Basic Exercise 2 - Initial Ramdisk

初始化內核後，它會掛載一個根文件系統並運行一個init用戶程序。 init程序可以是一個腳本或可執行二進製文件，用於在稍後啟動其他服務或加載其他驅動程序。

然而，由於您尚未實現任何文件系統和存儲驅動程序代碼，所以無法使用您的內核從SD卡加載任何內容。另一種方法是通過初始ramdisk加載用戶程序。

初始ramdisk是由引導加載程序加載或嵌入在內核中的文件。它通常是一個歸檔文件，可以被提取出來以構建一個根文件系統。

#### New ASCII Format Cpio Archive

cpio 是一種非常簡單的壓縮目錄和文件的歸檔格式。 每個目錄和文件都記錄為**標頭，後跟其路徑名和內容**。

在實驗 2 中，您將使用新 ASCII 格式 Cpio 格式創建 cpio 存檔。 您可以先創建一個 **rootfs** 目錄並將所需的所有文件放入其中。 然後，使用以下命令將其存檔。

```
cd rootfs
find . | cpio -o -H newc > ../initramfs.cpio
cd ..
```

<a href="https://man.freebsd.org/cgi/man.cgi?query=cpio&sektion=5">Freebsd’s man page</a> has a detailed definition of how New ASCII Format Cpio Archive is structured. You should read it and implement a parser to read files in the archive.

#### Loading Cpio Archive

**QEMU**
將參數 **-initrd <cpio archive>** 添加到 QEMU。 默認情況下，QEMU 將 cpio 存檔文件加載到 0x8000000。

> **Todo**
> 
> Parse New ASCII Format Cpio archive, and read file’s content given file’s pathname.

> **Important**
> 
> In Lab 2, you only need to **put some plain text files inside your archive** to test the functionality. In the later labs, you will also put script files and executables inside to automate the testing.

```
create_cpio.sh
makefile
```

### Basic Exercise 3 - Simple Allocator

內核在子系統初始化過程中需要一個分配器。 然而，動態分配器也是一個需要初始化的子系統。 所以我們在啟動初期需要一個簡單的分配器。

> **Todo**
> 
> 實現一個 alloc 函數，該函數返回指向所請求大小的連續空間的指針。

> **Hint**
> 
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

## Advanced Exercise

### Advanced Exercise 1 - Devicetree

在啟動過程中，內核應該知道當前連接了哪些設備，並使用相應的驅動程序來初始化和訪問它。 對於 PCIe 和 USB 等功能強大的總線，內核可以通過查詢總線的寄存器來檢測連接了哪些設備。 然後，它將設備的名稱與所有驅動程序相匹配，並使用兼容的驅動程序來初始化和訪問設備。

然而，對於具有簡單總線的計算機系統，內核無法檢測連接了哪些設備。 驅動這些設備的一種方法就像您在實驗 1 中所做的那樣； 開發人員知道要運行的目標機器是什麼，並在其內核中硬編碼 io 內存地址。 事實證明，驅動程序代碼變得不可移植。

更簡潔的方法是使用一個文件來描述計算機系統上的設備。 此外，它還記錄每個設備之間的屬性和關係。 然後，內核可以像查詢強大的總線系統一樣查詢該文件以加載正確的驅動程序。 該文件名為 deivcetree。

#### Format

Devicetree 有兩種格式 devicetree source (dts) 和 flattened devicetree (dtb)。 Devicetree 源代碼以人類可讀的形式描述了設備樹。 然後它被編譯成扁平化的設備樹，因此在慢速嵌入式系統中解析可以更簡單、更快。

您可以從樹莓派的linux存儲庫中讀取rpi3的dts

你可以通過手動編譯或者下載現成的來獲取rpi3的dtb。

#### Parsing

在這個高級部分中，您應該實現一個解析器來解析扁平化的設備樹。 此外，您的內核應該提供一個帶有回調函數參數的接口。 因此，驅動程序代碼可以遍歷整個設備樹來查詢每個設備節點，並通過檢查節點的名稱和屬性來匹配自身。

您可以從devicetree的官方網站獲取最新的規格。 然後按照第5、2、3章的順序閱讀rpi3的dts來實現你的解析器。

#### Dtb Loading

引導加載程序將 dtb 加載到內存中，並將寄存器 x0 中指定的加載地址傳遞給內核。 此外，它還修改了原始 dtb 內容以匹配實際機器設置。 例如，如果您要求引導加載程序加載初始 ramdisk，它會在 dtb 中添加初始 ramdisk 的加載地址。

**QEMU**

Add the argument -dtb bcm2710-rpi-3-b-plus.dtb to QEMU.

**Rpi3**

Move bcm2710-rpi-3-b-plus.dtb into SD card.

> **Todo**
> 
> 實現一個可以迭代設備樹的解析器。 另外，提供一個帶有回調函數的 API，以便驅動程序代碼可以在設備樹迭代期間訪問設備節點的內容。

The folloing code is a breif example of the API. You can design it in your own way.

```
void initramfs_callback(...) {
  ...
}

int main() {
  fdt_traverse(initramfs_callback);
}
```

> **Todo**
> 
> Use the API to get the address of initramfs instead of hardcoding it.

> **Todo**
> 
> Modify your bootloader for passing the device tree loading address.

---

## Implement
### Generate initramfs.cpio

```console
cd create_fs
bash create_cpio.sh
```

### Copy create_fs/initramfs.cpio to kernel/initramfs.cpio
### Generate kernel/build
```console
cd kernel
make
```

```console
make run
```
