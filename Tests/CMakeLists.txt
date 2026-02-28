# *  @Copyright (c) tao.jing
# *
# *
cmake_minimum_required(VERSION 3.26)
project(TaoUtilTests)


if (NOT TAO_UTIL_LIB_PATH)
    message(FATAL_ERROR "No TAO_UTIL_LIB_PATH set.")
else ()
    set(TAO_UTIL_LIB_PATH ${TAO_UTIL_LIB_PATH})
endif ()


set(EXECUTABLE_OUTPUT_PATH ${BASE_OUTPUT_DIR}/${PROJECT_NAME})


if (DEFINED BUILD_SHARED_LIBS)
    set(_tutil_had_build_shared_libs ON)
    set(_tutil_prev_build_shared_libs "${BUILD_SHARED_LIBS}")
else ()
    set(_tutil_had_build_shared_libs OFF)
endif ()
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)

add_subdirectory(gtest)

if (_tutil_had_build_shared_libs)
    set(BUILD_SHARED_LIBS "${_tutil_prev_build_shared_libs}" CACHE BOOL "Build shared libraries" FORCE)
else ()
    unset(BUILD_SHARED_LIBS CACHE)
endif ()


include_directories(${TAO_UTIL_LIB_PATH}/Inc)


link_directories(${CMAKE_BINARY_DIR}/lib)


file(GLOB TUtilTestsSRC *.cpp)


# --- System Util Test ---
add_executable(TUtilTest ${TUtilTestsSRC})


target_link_directories(TUtilTest PUBLIC ${TAO_UTIL_LIB_OUTPUT_PATH})

target_link_libraries(TUtilTest PUBLIC TUtilLib)
target_link_libraries(TUtilTest PUBLIC gtest gtest_main)


set(DEBUG_SUFFIX)
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set(LINK_LIB_TYPE "Debug")
    set(DEBUG_SUFFIX "d")
else ()
    set(LINK_LIB_TYPE "Release")
    set(DEBUG_SUFFIX)
endif ()


if (WIN32)
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/bin/gtest.dll"
            "${BASE_OUTPUT_DIR}/TUtilLib/"
    )
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/bin/gtest.dll"
            "${BASE_OUTPUT_DIR}/TaoUtilTests/"
    )
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${BASE_OUTPUT_DIR}/TUtilLib/TUtilLib${DEBUG_SUFFIX}.dll"
            "${BASE_OUTPUT_DIR}/TaoUtilTests/"
    )
elseif (UNIX)
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/lib/libgtest.so"
            "${BASE_OUTPUT_DIR}/TUtilLib/"
    )
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_BINARY_DIR}/lib/libgtest.so"
            "${BASE_OUTPUT_DIR}/TaoUtilTests/"
    )
    add_custom_command(TARGET TUtilTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${BASE_OUTPUT_DIR}/TUtilLib/libTUtilLib${DEBUG_SUFFIX}.so"
            "${BASE_OUTPUT_DIR}/TaoUtilTests/"
    )
endif ()