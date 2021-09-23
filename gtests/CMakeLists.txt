# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-23

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(INSPECT_GTESTS VERSION 0005)


include_directories(
    ${googletest_SOURCE_DIR}/googletest/include/
    ${googletest_SOURCE_DIR}/googlemock/include/
    ${pthreads4w_SOURCE_DIR}/
)

# If linking with shared build gtest libs we need to tell it the compiler.
# I have found a vague hint about the flag GTEST_LINKED_AS_SHARED_LIBRARY at
# build/_deps/googletest-src/googletest/README.md
add_compile_definitions(
        PRIVATE $<$<BOOL:${BUILD_SHARED_LIBS}>:GTEST_LINKED_AS_SHARED_LIBRARY>
)

link_libraries(
        ${UPNP_GTEST_LIBRARY}
        ${UPNP_GMOCK_LIBRARY}
        ${UPNP_GTEST_MAIN_LIBRARY}
        ${UPNP_GMOCK_MAIN_LIBRARY}
        ${UPNP_PT4W_STATIC_LIBRARY}
)

# test simple
add_executable(test_simple ${PROJECT_SOURCE_DIR}/test_simple.cpp)
