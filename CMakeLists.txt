# *  @Copyright (c) tao.jing
# *
# *

cmake_minimum_required(VERSION 3.26)
project(TUtilLib)


# ---- Compilations ----
set(CMAKE_CXX_STANDARD 20)
set(BUILD_SHARED_LIBS ON)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

if (MSVC)
    # MultiThreadedDLL MultiThreaded
    #set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Werror -Wall -Wextra -pedantic")
endif ()


# --- Project Management ---
IF (NOT BASE_OUTPUT_DIR)
    if (CMAKE_BUILD_TYPE MATCHES "Debug")
        set(BASE_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bin_debug)
    else ()
        set(BASE_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/bin_release)
    endif ()
ELSE ()
    set(BASE_OUTPUT_DIR ${BASE_OUTPUT_DIR})
ENDIF ()

set(LIBRARY_OUTPUT_PATH ${BASE_OUTPUT_DIR}/${PROJECT_NAME})
set(TAO_UTIL_LIB_OUTPUT_PATH ${BASE_OUTPUT_DIR}/${PROJECT_NAME})
set(EXECUTABLE_OUTPUT_PATH ${BASE_OUTPUT_DIR}/${PROJECT_NAME})


set(TAO_UTIL_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Tests)

include(${TAO_UTIL_LIB_PATH}/CMake/TaoUtilLib.cmake)