# inspect_GTest-pthreads4w
Inspect how GoogleTest works with POSIX Threads for Windows.

## Preparation
Here I use the PowerShell. These are examples from my environment. You have to use your own correct paths to your installation.

To find the header files of pthreads4w we have to set the environment varilable CL:

    PS> $env:CL = "/I C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-src"

To find the .dll files we have to append its directories to the Path environment variable. To easy restore the original setting I backup it:

    PS> $env:Path_orig = $env:Path
    PS> $env:Path += ";C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\bin;C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-build"

To find the linkable libraries we have to append its directories to the LIB environment variable. To easy restore the original setting I backup it:

    PS> $env:LIB_orig = $env:LIB
    PS> $env:LIB += "";C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\lib;C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-build"

## Configuration and build

    PS> cmake -S . -B build -G "NMake Makefiles" --log-level=DEBUG -D BUILD_TESTING=OFF -D CMAKE_BUILD_TYPE=MinSizeRel [ -D BUILD_SHARED_LIBS=ON ]
    PS> cmake --build build --config MinSizeRe
    PS> .\gtests\build\test_template.exe
    PS> .\gtests\build\simple_pthread.exe
