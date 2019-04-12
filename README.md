# LibsForDev
Some useful libs for developer

## 测试框架googletest
广泛应用于各种C++应用的测试中

```cpp
```
## 压缩库zlib
应用于图像，文本各种格式的数据进行压缩存储。

```cpp
```

## protobuf
用来数据交换的库，当前在深度学习应用十分广泛

```cpp
```

## bearssl
BearSSL is an implementation of the SSL/TLS protocol (RFC 5246) written in C. 
https://www.bearssl.org/index.html

## libcurl - the multiprotocol file transfer library
libcurl is a free and easy-to-use client-side URL transfer library, supporting DICT, FILE, FTP, FTPS, Gopher, HTTP, HTTPS, IMAP, IMAPS, LDAP, LDAPS, POP3, POP3S, RTMP, RTSP, SCP, SFTP, SMTP, SMTPS, Telnet and TFTP. libcurl supports SSL certificates, HTTP POST, HTTP PUT, FTP uploading, HTTP form based upload, proxies, cookies, user+password authentication (Basic, Digest, NTLM, Negotiate, Kerberos), file transfer resume, http proxy tunneling and more!

libcurl is highly portable, it builds and works identically on numerous platforms, including Solaris, NetBSD, FreeBSD, OpenBSD, Darwin, HPUX, IRIX, AIX, Tru64, Linux, UnixWare, HURD, Windows, Amiga, OS/2, BeOs, Mac OS X, Ultrix, QNX, OpenVMS, RISC OS, Novell NetWare, DOS and more...

libcurl is free, thread-safe, IPv6 compatible, feature rich, well supported, fast, thoroughly documented and is already used by many known, big and successful companies.
https://curl.haxx.se/libcurl/

## glog & gflag
google 出的一个C++轻量级日志库、

安装： `sudo apt-get install libgflags-dev libgoogle-glog-dev`

使用参考链接：

http://www.cnblogs.com/tianyajuanke/archive/2013/02/22/2921850.html

## Eigen
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
