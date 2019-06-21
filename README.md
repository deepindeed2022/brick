# Brick

## Introduction
As a developer, we will use a lot of 3rdparty libs for the open source community. 
We have use a lot of 3rdparty libs from github, google developer community or official website.

## libraries list

### gtest

For C++ application unit test which is designed and maintained by google.

### zlib 

Data compression used for save storage in application. The zlib could work with multi-format 
data, such as digital image, raw text.

### protobuf

Protocol buffers are Google's language-neutral, platform-neutral, extensible mechanism for serializing structured data – think XML, but smaller, faster, and simpler. You define how you want your data to be structured once, then you can use special generated source code to easily write and read your structured data to and from a variety of data streams and using a variety of languages.

### bearssl
BearSSL is an implementation of the SSL/TLS protocol (RFC 5246) written in C. 
https://www.bearssl.org/index.html

### libcurl - the multiprotocol file transfer library
libcurl is a free and easy-to-use client-side URL transfer library, supporting DICT, FILE, FTP, FTPS, Gopher, HTTP, HTTPS, IMAP, IMAPS, LDAP, LDAPS, POP3, POP3S, RTMP, RTSP, SCP, SFTP, SMTP, SMTPS, Telnet and TFTP. libcurl supports SSL certificates, HTTP POST, HTTP PUT, FTP uploading, HTTP form based upload, proxies, cookies, user+password authentication (Basic, Digest, NTLM, Negotiate, Kerberos), file transfer resume, http proxy tunneling and more!

libcurl is highly portable, it builds and works identically on numerous platforms, including Solaris, NetBSD, FreeBSD, OpenBSD, Darwin, HPUX, IRIX, AIX, Tru64, Linux, UnixWare, HURD, Windows, Amiga, OS/2, BeOs, Mac OS X, Ultrix, QNX, OpenVMS, RISC OS, Novell NetWare, DOS and more...

libcurl is free, thread-safe, IPv6 compatible, feature rich, well supported, fast, thoroughly documented and is already used by many known, big and successful companies.
https://curl.haxx.se/libcurl/

### glog & gflag
C++ lite log lib `glog`

install directly on os system:`sudo apt-get install libgflags-dev libgoogle-glog-dev`

[cnblog glog](http://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html)

### Eigen
Eigen is a high-level C++ library of template headers for linear algebra, matrix and vector operations, geometrical transformations, numerical solvers and related algorithms. Eigen is an open source library licensed under MPL2 starting from version 3.1.1. Earlier versions were licensed under LGPL3+.

Eigen is implemented using the expression templates metaprogramming technique, meaning it builds expression trees at compile time and generates custom code to evaluate these. Using expression templates and a cost model of floating point operations, the library performs its own loop unrolling and vectorization.

http://eigen.tuxfamily.org/index.php?title=Main_Page

#### Eigen is versatile.

It supports all matrix sizes, from small fixed-size matrices to arbitrarily large dense matrices, and even sparse matrices.
It supports all standard numeric types, including std::complex, integers, and is easily extensible to custom numeric types.
It supports various matrix decompositions and geometry features.
Its ecosystem of unsupported modules provides many specialized features such as non-linear optimization, matrix functions, a polynomial solver, FFT, and much more.

#### Eigen is fast.

Expression templates allow to intelligently remove temporaries and enable lazy evaluation, when that is appropriate.
Explicit vectorization is performed for SSE 2/3/4, AVX, AVX2, FMA, AVX512, ARM NEON (32-bit and 64-bit), PowerPC AltiVec/VSX (32-bit and 64-bit), ZVector (s390x/zEC13) SIMD instruction sets, and since 3.4 MIPS MSA with graceful fallback to non-vectorized code.
Fixed-size matrices are fully optimized: dynamic memory allocation is avoided, and the loops are unrolled when that makes sense.
For large matrices, special attention is paid to cache-friendliness.

#### Eigen is reliable.

Algorithms are carefully selected for reliability. Reliability trade-offs are clearly documented and extremely safe decompositions are available.
Eigen is thoroughly tested through its own test suite (over 500 executables), the standard BLAS test suite, and parts of the LAPACK test suite.

#### Eigen is elegant.

The API is extremely clean and expressive while feeling natural to C++ programmers, thanks to expression templates.
Implementing an algorithm on top of Eigen feels like just copying pseudocode.
Eigen has good compiler support as we run our test suite against many compilers to guarantee reliability and work around any compiler bugs. Eigen also is standard C++98 and maintains very reasonable compilation times.

### opencv

`https://github.com/cwlseu/recipes/blob/master/script/install-opencv.sh`

### caffe

`https://github.com/cwlseu/recipes/blob/master/script/install_caffe.sh`

### Vulkan
https://www.khronos.org/files/vulkan11-reference-guide.pdf

Vulkan® is a graphics and compute API consisting of procedures and functions to specify shader programs, compute kernels, objects, and operations involved in producing high-quality graphical images, specifically color images of three-dimensional objects. Vulkan is also a pipeline with programmable and state-driven fixed-function stages that are invoked by a set of specific drawing operations.

Vulkan®是一种图形和计算API，由程序和函数组成，用于指定着色器程序、计算内核、对象和用于生成高质量图形图像(特别是三维对象的彩色图像)的操作。Vulkan也是由一组特定的绘图操作调用组成的，具有可编程和状态驱动的固定功能级的流水线。

在Alibaba开源的MNN项目中就封装了该库。

## 中文介绍
作为一名开发者，我们常常应用来自开源社区的第三方库来构建应用， 而这些开源应用就相当于构建应用的砖头，
无论多么宏伟的建筑物都是从这些砖头一点垒起来的。本项目中主要将日常开发中的使用的第三方库收集，整理，编写sample等等，方便用户快速入门。

### 特点
- 依赖的库自动下载
- 基于cmake开发，可跨平台使用
- 提供sample，快速上手

### 编译

`mkdir build && cd build && cmake .. && make -j4`

## contact

`echo Y2Fvd2VubG9uZzkyQGdtYWlsLmNvbQo= | base64 --decode`