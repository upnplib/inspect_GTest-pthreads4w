# Copyright (C) 2021 GPL 3 and higher by Ingo Höft,  <Ingo@Hoeft-online.de>
# Redistribution only with this Copyright remark. Last modified: 2021-09-23

# Test builds with:
# cmake -S . -B build -D gtest_disable_pthreads=ON [-G "NMake Makefiles"] [-D BUILD_SHARED_LIBS=ON] -D CMAKE_BUILD_TYPE=[MinSizeRel|Debug] [--log-level=DEBUG]
# cmake --build build --config [MinSizeRel|Debug]

cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GTEST_WITH_PT4W VERSION 0004)


#################################
# General checks and settings   #
#################################

option(PT4W_BUILD_TESTING "Build the time and space consuming test suite of pthreads4w" OFF)

if(NOT CMAKE_BUILD_TYPE)
    list(GET CMAKE_CONFIGURATION_TYPES 0 cmakeBuildType)
    set(CMAKE_BUILD_TYPE ${cmakeBuildType} CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}." FORCE)
    message(STATUS "No build type specified, defaulting to ${CMAKE_BUILD_TYPE}.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # Suffix on libraries having built with Debug information
    set(UPNP_DEBUG_SUFFIX "d")
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

include(FetchContent)


#################################
# Install pthreads4w            #
#################################
message(CHECK_START "Download and configuring POSIX Threads for Windows")

FetchContent_Declare(
    pthreads4w
    GIT_REPOSITORY  https://github.com/jwinarske/pthreads4w.git
    GIT_TAG         origin/cmake
    GIT_SHALLOW     ON
)
# Check if population has already been performed
FetchContent_GetProperties(pthreads4w)
if(NOT pthreads4w_POPULATED)
    # Fetch the content using previously declared details.
    # This caches pthreads4w_SOURCE_DIR, pthreads4w_BINARY_DIR and pthreads4w_POPULATED.
    FetchContent_Populate(pthreads4w)

    # Output with cmake option --log-level=DEBUG
    message(DEBUG "  DEBUG: pthreads4w_SOURCE_DIR is: ${pthreads4w_SOURCE_DIR}")
    message(DEBUG "  DEBUG: pthreads4w_BINARY_DIR is: ${pthreads4w_BINARY_DIR}")

    execute_process(COMMAND cmake -S ${pthreads4w_SOURCE_DIR} -B ${pthreads4w_BINARY_DIR} -G "NMake Makefiles" -D CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -D BUILD_TESTING=${PT4W_BUILD_TESTING} #-D CMAKE_COLOR_MAKEFILE=OFF
                    WORKING_DIRECTORY ${pthreads4w_SOURCE_DIR}
                    RESULT_VARIABLE PT4W_RETURN_CODE
                    ERROR_VARIABLE PT4W_ERROR_MESSAGE
                    COMMAND_ECHO STDOUT)
    if(NOT ${PT4W_RETURN_CODE} EQUAL 0)
        # This will stop the installation
        message(FATAL_ERROR "${PT4W_RETURN_CODE}: ${PT4W_ERROR_MESSAGE}")
    endif()

    message(CHECK_PASS "done")
    message(CHECK_START "Building POSIX Threads for Windows libraries")

    execute_process(COMMAND cmake --build ${pthreads4w_BINARY_DIR} --config ${CMAKE_BUILD_TYPE} #--target install
                    WORKING_DIRECTORY ${pthreads4w_SOURCE_DIR}
                    RESULT_VARIABLE PT4W_RETURN_CODE
                    ERROR_VARIABLE PT4W_ERROR_MESSAGE
                    OUTPUT_QUIET)
    if(NOT ${PT4W_RETURN_CODE} EQUAL 0)
        # This will stop the installation
        message(FATAL_ERROR "${PT4W_RETURN_CODE}: ${PT4W_ERROR_MESSAGE}")
    endif()

    set(UPNP_PT4W_STATIC_LIBRARY "${pthreads4w_BINARY_DIR}/libpthreadVC3${UPNP_DEBUG_SUFFIX}.lib"
        CACHE INTERNAL "Name of the linkable static library for POSIX threads")  # Implies FORCE

    message(CHECK_PASS "done")

endif(NOT pthreads4w_POPULATED)


#################################
# Install GoogleTest            #
#################################

# pthread is always disabled on GoogleTest due to bugs with pthreads4w.
# Default Generator is "Visual Studio *" on MS Windows.
# Tested following combinations:
# Generator    build     build type    verified
# ---------------------------------------------
# default      static    MinSizeRel    yes
# default      static    Debug         yes
# default      shared    MinSizeRel    yes
# default      shared    Debug         yes
# NMake        static    MinSizeRel    yes
# NMake        static    Debug         yes
# NMake        shared    MinSizeRel    yes
# NMake        shared    Debug         yes

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

# Cache the different GoogleTest library names for later use
if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
    set(UPNP_GTEST_LIBRARY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GMOCK_LIBRARY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GTEST_MAIN_LIBRARY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gtest_main${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GMOCK_MAIN_LIBRARY "${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}/gmock_main${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
else()
    set(UPNP_GTEST_LIBRARY "${PROJECT_BINARY_DIR}/lib/gtest${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GMOCK_LIBRARY "${PROJECT_BINARY_DIR}/lib/gmock${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GTEST_MAIN_LIBRARY "${PROJECT_BINARY_DIR}/lib/gtest_main${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
    set(UPNP_GMOCK_MAIN_LIBRARY "${PROJECT_BINARY_DIR}/lib/gmock_main${UPNP_DEBUG_SUFFIX}.lib" CACHE INTERNAL "")
endif()

message(CHECK_PASS "done")


#################################
# Build Unit Tests              #
#################################

message(CHECK_START "configuring Unit Tests")

# This caches INSPECT_GTESTS_SOURCE_DIR and INSPECT_GTESTS_BINARY_DIR
add_subdirectory(${PROJECT_SOURCE_DIR}/gtests ${PROJECT_SOURCE_DIR}/gtests/build)

# Create symlinks for shared gtest .dlls so the Unit Tests can find them.
if(WIN32 AND BUILD_SHARED_LIBS)
    if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
        # Make Directory is necessary because sub directory ${CMAKE_BUILD_TYPE} isn't available yet.
        file(MAKE_DIRECTORY ${INSPECT_GTESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE})
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/gtest${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/gmock${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gmock${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/gtest_main${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gtest_main${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/gmock_main${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/gmock_main${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
    else()
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/gtest${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/gtest${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/gmock${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/gmock${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/gtest_main${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/gtest_main${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
        file(CREATE_LINK ${PROJECT_BINARY_DIR}/bin/gmock_main${UPNP_DEBUG_SUFFIX}.dll
                         ${INSPECT_GTESTS_BINARY_DIR}/gmock_main${UPNP_DEBUG_SUFFIX}.dll
                         SYMBOLIC)
    endif()

endif(WIN32 AND BUILD_SHARED_LIBS)

message(CHECK_PASS "done")
