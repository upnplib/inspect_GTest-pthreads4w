# inspect_GTest-pthreads4w
Inspect how GoogleTest works with POSIX Threads for Windows.

GoogleTest finds the pthread header and library but unfortunately does not compile successfully. I haven't found why not yet. I will keep looking for a solution. **UPnPlib** and other programs can compile and use pthreads but testing and mocking pthread functions does not work. All other tests are working. To make this repository running just don't modify environment variable LIB but CL is needed as shown below. GoogleTest will not find the pthreads libraries then and compiles successful without pthread support. Then you can check the simple program with pthreads.

## Preparation
Here I use the PowerShell. These are examples from my environment. You have to use your own correct paths to your installation.

We have to set some environment variables to find the libraries. We cannot do it with cmake because there they are only local to the current process. They are not propagated to sub- and calling processes. We have to set them before executing cmake.

To find the header files of pthreads4w we must set the environment varilable CL:

    PS> $env:CL = "/I C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-src"

To find the linkable libraries we have to append its directories to the LIB environment variable. To easy restore the original setting I backup it:

    PS> $env:LIB_orig = $env:LIB
    PS> $env:LIB += ";C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-build"

To find the .dll files we have to append its directory to the Path environment variable. To easy restore the original setting I backup it. These settings are not needed if you link with the static pthreads4w library.

    PS> $env:Path_orig = $env:Path
    # for googletest executable (.dll)
    PS> $env:Path += ";C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-build"

## Configuration and build

    PS> cmake -S . -B build -G "NMake Makefiles" --log-level=DEBUG -D BUILD_TESTING=OFF -D CMAKE_BUILD_TYPE=MinSizeRel [-D BUILD_SHARED_LIBS=ON] [-D GTEST_HAS_PTHREAD=1]
    PS> cmake --build build --config MinSizeRel
    PS> .\gtests\build\test_template.exe
    PS> .\gtests\build\simple_pthread.exe

Option -D GTEST_HAS_PTHREAD=1 is usually not needed if all environment variables are set.
