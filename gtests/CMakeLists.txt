# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-18

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(INSPECT_GTESTS VERSION 0002)


message(CHECK_START "Configuring Unit Tests")

include_directories(
    ${googletest_SOURCE_DIR}/googletest/include/
    ${googletest_SOURCE_DIR}/googlemock/include/
    ${pthreads4w_SOURCE_DIR}/pthreads4w/include/
)

set(targ_suffix "")
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    set(targ_suffix "d")
endif()

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    link_libraries(${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest_main${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock_main${targ_suffix}.lib)
else()
    link_libraries(${CMAKE_BINARY_DIR}/lib/gtest${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/gmock${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/gtest_main${targ_suffix}.lib
                   ${CMAKE_BINARY_DIR}/lib/gmock_main${targ_suffix}.lib)
endif()

# If linking with shared build gtest libs we need to tell it the compiler.
# I have found a vague hint about the flag GTEST_LINKED_AS_SHARED_LIBRARY at
# build/_deps/googletest-src/googletest/README.md
add_compile_definitions(
        PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:GTEST_LINKED_AS_SHARED_LIBRARY>
)

# test template
add_executable(test_template ${PROJECT_SOURCE_DIR}/test_template.cpp)

message(CHECK_PASS "done")
