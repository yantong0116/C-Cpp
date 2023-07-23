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



- bootloader / 
```
make
```

- create_fs / 
```
./create_cpio.sh
```

- move file
```
from create_fs/initramfs.cpio
to bootloader/initramfs.cpio
```

- kernel /
```
make
```

### Execute
- bootloader / 
```
make run_std
```
