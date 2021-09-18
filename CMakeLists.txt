# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-18

# Test build with:
# cmake -S . -B build -G "NMake Makefiles" --log-level=DEBUG -D BUILD_TESTING=OFF -D CMAKE_BUILD_TYPE=MinSizeRel -D BUILD_SHARED_LIBS=ON
# cmake --build build --config MinSizeRel --target install

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GTEST_WITH_PT4W VERSION 0001)


if(NOT CMAKE_GENERATOR STREQUAL "NMake Makefiles")
    message(FATAL_ERROR "\nBuilding POSIX Threads for Windows is only supported using Generator \"NMake Makefiles\". To configure use option, for example:\ncmake -S . -B build -G \"NMake Makefiles\" -D BUILD_TESTING=OFF")
endif()

if(NOT CMAKE_BUILD_TYPE)
    list(GET CMAKE_CONFIGURATION_TYPES 0 cmakeBuildType)
    set(CMAKE_BUILD_TYPE ${cmakeBuildType} CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}." FORCE)
    message(STATUS "No build type specified, defaulting to ${CMAKE_BUILD_TYPE}.")
endif()


find_package(Git)

# pthreads4w is essential to run UPnPlib on MS Windows.
if(NOT Git_FOUND)
    message(FATAL_ERROR "A git client isn't available. We need it to download POSIX Threads for Windows.")
else()
    # This we need to FetchContent from github.com
    if(GIT_VERSION_STRING VERSION_LESS "1.6.5")
        message(FATAL_ERROR "Git version ${GIT_VERSION_STRING} found but needed at least 1.6.5.")
    endif()
endif()

message(CHECK_START "Download and configuring POSIX Threads for Windows")

include(FetchContent)

FetchContent_Declare(
    pthreads4w
    GIT_REPOSITORY  https://github.com/jwinarske/pthreads4w.git
    GIT_TAG         origin/cmake
    GIT_SHALLOW     ON
)
# This caches pthreads4w_SOURCE_DIR, pthreads4w_BINARY_DIR and pthreads4w_POPULATED.
FetchContent_MakeAvailable(pthreads4w)

# Output with cmake option --log-level=DEBUG
message(DEBUG "  DEBUG: pthreads4w_SOURCE_DIR is: ${pthreads4w_SOURCE_DIR}")
message(DEBUG "  DEBUG: pthreads4w_BINARY_DIR is: ${pthreads4w_BINARY_DIR}")

message(CHECK_PASS "done")


message(CHECK_START "Download and configuring Googletest")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           origin/master
    GIT_SHALLOW       ON
)
if(WIN32)
    # reference: build/_deps/googletest-src/googletest/README.md
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

# This caches googletest_SOURCE_DIR, googletest_BINARY_DIR and googletest_POPULATED.
FetchContent_MakeAvailable(googletest)

# Output with cmake option --log-level=DEBUG
message(DEBUG "  DEBUG: googletest_SOURCE_DIR is: ${googletest_SOURCE_DIR}")
message(DEBUG "  DEBUG: googletest_BINARY_DIR is: ${googletest_BINARY_DIR}")

message(CHECK_PASS "done")

# The fetched content is made available by building the libraries in a
# subproject using 'add_subdirectory()' (encapsulated in
# 'FetchContent_MakeAvailable()'). This is the reason why we cannot build the
# gtests direct here. These builds are executed before the build in the
# subproject. Build of the gtests will fail because the libraries are not
# available yet. We have to use also a subproject for the gtests so we can
# control the order of the builds.
#
add_subdirectory(${PROJECT_SOURCE_DIR}/gtests ${PROJECT_SOURCE_DIR}/gtests/build)
