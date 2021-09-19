# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-18

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(INSPECT_GTESTS VERSION 0002)


message(CHECK_START "Configuring Unit Tests")

# Sets variables googletest_POPULATED, googletest_SOURCE_DIR, googletest_BINARY_DIR.
FetchContent_GetProperties(googletest)

include_directories(
    ${googletest_SOURCE_DIR}/googletest/include/
    ${googletest_SOURCE_DIR}/googlemock/include/
    ${pthreads4w_SOURCE_DIR}/pthreads4w/include/
)

# If linking with shared build gtest libs we need to tell it the compiler.
# I have found a vague hint about the flag GTEST_LINKED_AS_SHARED_LIBRARY at
# build/_deps/googletest-src/googletest/README.md
add_compile_definitions(
        PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:GTEST_LINKED_AS_SHARED_LIBRARY>
)

# test template
add_executable(test_template ${PROJECT_SOURCE_DIR}/test_template.cpp)

set(targ_suffix "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(targ_suffix "d")
endif()

target_link_libraries(test_template
               ${CMAKE_BINARY_DIR}/lib/gtest${targ_suffix}.lib
               ${CMAKE_BINARY_DIR}/lib/gmock${targ_suffix}.lib
               #${CMAKE_BINARY_DIR}/lib/gtest_main${targ_suffix}.lib
               #${CMAKE_BINARY_DIR}/lib/gmock_main${targ_suffix}.lib
               #-pthread
               #${pthreads4w_BINARY_DIR}/libpthreadVC3.lib
               #${pthreads4w_BINARY_DIR}/pthread.lib
)

add_executable(simple_pthread ${PROJECT_SOURCE_DIR}/simple_pthread.cpp)

target_link_libraries(simple_pthread
               #${pthreads4w_BINARY_DIR}/libpthreadVC3.lib
               ${pthreads4w_BINARY_DIR}/pthread.lib
)

message(CHECK_PASS "done")
