# inspect_GTest-pthreads4w
Inspect how GoogleTest works with POSIX Threads for Windows.

## preparation
Here I use the PowerShell. These are examples from my environment. You have to use your own correct paths to your installation.

To find the header files of pthreads4w we have to set the environment varilable CL:
PS> $env:CL = "/I C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\_deps\pthreads4w-src"

To find the .dll files we have to append its directories to the Path environment variable. To easy restore the original setting I backup it:
PS> $env:Path-orig = $env:Path
PS> $env:Path += "$env:Path;C:\Users\ingo\devel\inspect_GTest-pthreads4w\build\bin"
