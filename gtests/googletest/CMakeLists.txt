# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-19

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GOOGLETEST VERSION 0002)


message(CHECK_START "Download and configuring GoogleTest")

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
