## 有用的参考
- [OpenCV过渡指南-W3Cschool](https://www.w3cschool.cn/opencv/opencv-cig92c96.html)

## 自定义编译链编译opencv

### 设置编译链
认清自己的的编译链是什么系统什么架构的，就可以在opencv目录下找到对应的文件`opencv-3.3.0/platforms/linux/arm-gnueabi.toolchain.cmake`，添加自己的编译链。如下代码中前三行是自己添加的

```cmake
SET(CMAKE_TOOLCHAIN_DIR /path/to/mytoolchains/arm-himix100-linux/bin)
SET(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_DIR}/arm-himix100-linux-gcc)
SET(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_DIR}/arm-himix100-linux-g++)
set(GCC_COMPILER_VERSION "" CACHE STRING "GCC Compiler version")
set(GNU_MACHINE "arm-linux-gnueabi" CACHE STRING "GNU compiler triple")
include("${CMAKE_CURRENT_LIST_DIR}/arm.toolchain.cmake")
```

### cmake & make 

创建一个构建目录，使其成为当前并运行以下命令：
`cmake [<some optional parameters>] -DCMAKE_TOOLCHAIN_FILE=<path to the OpenCV source directory>/platforms/linux/arm-gnueabi.toolchain.cmake <path to the OpenCV source directory>`

工具链默认使用gnueabihf EABI约定。添加`-DSOFTFP=ON` cmake参数以打开softfp编译器。

`cmake [<some optional parameters>] -DSOFTFP=ON -DCMAKE_TOOLCHAIN_FILE=<path to the OpenCV source directory>/platforms/linux/arm-gnueabi.toolchain.cmake <path to the OpenCV source directory>`

## 列表

- [Mat的基本操作](samples/mat_the_basic_image_contrainer.cpp)
- [DFT](samples/discrete_fourier_transform.cpp)
- [视频PSNR & SSIM算法](samples/video_input_psnr_ssim.cpp)
- [OpenCV使用背景减法]()