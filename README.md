# inspect_GTest-pthreads4w
Inspect how GoogleTest works with POSIX Threads for Windows.

GoogleTest finds the pthread header and library but unfortunately does not compile successfully. What I have found with this repository I have reported to a bug at [Googletest Issue #3577](https://github.com/google/googletest/issues/3577). **UPnPlib** and other programs can compile and use pthreads but mocking pthread functions does not work. All other tests are working. You can check the `test_simple` Unit Test with pthreads.

The configuration with attempts to compile GoogleTest with pthreads4w can be found at branch `pthread`.

## Preparation
These are examples from my environment. You have to use your own correct paths to your installation.

This repository uses pthreads4w internal only for this project as also intended to be used with **UPnPlib**. To use pthreads4w system wide you can look at [Install third party POSIX Threads for Windows](https://stackoverflow.com/q/69275376/5014688).

## Configuration and build
Here I use the PowerShell.

    PS> cmake -S . -B build -D gtest_disable_pthreads=ON [-G "NMake Makefiles"] [-D BUILD_SHARED_LIBS=ON] -D CMAKE_BUILD_TYPE=[MinSizeRel|Debug] [--log-level=DEBUG]
    PS> cmake --build build --config [MinSizeRel|Debug]

You should always disable pthreads for GoogleTest. Default using is:

    PS> cmake -S . -B build -D gtest_disable_pthreads=ON -D BUILD_SHARED_LIBS=ON -D CMAKE_BUILD_TYPE=MinSizeRel
    PS> cmake --build build --config MinSizeRel
    PS> .\gtests\build\test_simple.exe

## Conclusion
As shown by the bug report it seems GoogleTest with pthreads is quite buggy in particular on Microsoft Windows. Because I want cross platform compatibillity of the Unit Tests I decided to use GoogleTest without pthreads on all platforms. I will always build with option '-D gtest_disable_pthreads=ON'. There is also a flag `-D GTEST_HAS_PTHREAD=0` but it doesn't work as expected. Lack of pthread support with GoogleTest does not affect pthreads on built programs. On the contrary, it enables mocking of pthread functions, which otherwise would crash with segfault. In particular I have found:

- GoogleTest does not compile successful pthreads4w with Generator "NMake Makefiles"
- GoogleTest does not find library of pthreads4w with Generator "Visual Studio *"
- option GTEST_HAS_PTHREAD does not work as expected
- Preprocessor macro 'GTEST_IS_THREADSAFE' does not work

<pre><sup>
// Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  &#60;Ingo&#64;Hoeft-online.de&#62;
// Redistribution only with this Copyright remark. Last modified: 2021-09-23
</sup></sup>
