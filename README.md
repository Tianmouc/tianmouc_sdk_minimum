# Tianmouc SDK miniomum version

## usages

### If you have the access to the source code of tianmouc-sdk full version

1. install cyusb, opencv, lib-usb-1.0-dev
2. cp libcyusb.so to ./lib
3. run 'sh compile.sh'
4. run the somple application for tianmouc-v1 by ./build/tianmoucv_app
5. you can also use the ./build/libtianmouc.so for further development

### If you dont have the access

- only x86 supported because the shared .so files are compiled under x86, ubuntu24.04

1. You can find another cmake file in ./lib, use it to replace the defualt one
2. compile this project, you can still get the app

### Function

The program ./build/tianmoucv_app executes two parallel visualization branches with different frame rates, displaying RGB and TSD respectively, as shown in the following effect:
<img width="1854" height="960" alt="79d80186d8edcf133e7581bfe0ededa4" src="https://github.com/user-attachments/assets/57271f6b-4989-4e32-b509-dac42c458261" />

This program can serve as a foundation for developing more low-latency, high-speed inference algorithms.

## development log

- init version 20241104
-- bug: memory leak, cannot find where is the bug
-- bug2: bad cone decoded result??
-- bug3: cannot use disableSync api, repost "free( ) invalid xxx"
-- bug4: very dirty stop
- updated version 20240906
- fixed all bugs
- minimum develop kit
