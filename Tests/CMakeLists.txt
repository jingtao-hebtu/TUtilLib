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


add_subdirectory(gtest)


include_directories(${TAO_UTIL_LIB_PATH}/Inc)


link_directories(${CMAKE_BINARY_DIR}/lib)


file(GLOB TUtilTestsSRC *.cpp)


# --- System Util Test ---
add_executable(TUtilTest ${TUtilTestsSRC})


target_link_directories(TUtilTest PUBLIC ${TAO_UTIL_LIB_OUTPUT_PATH})

target_link_libraries(TUtilTest PUBLIC TUtilLib)
target_link_libraries(TUtilTest PUBLIC gtest gtest_main)


add_custom_command(TARGET TUtilTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/bin/gtest.dll"
        "${BASE_OUTPUT_DIR}/TUtilLib/"
)