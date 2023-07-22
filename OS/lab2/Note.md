### Make File
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
