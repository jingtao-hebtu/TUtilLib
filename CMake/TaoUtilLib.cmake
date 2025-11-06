# *  @Copyright (c) tao.jing
# *
# *
cmake_minimum_required(VERSION 3.26)
project(TUtilLib)


if (NOT TAO_UTIL_LIB_PATH)
    message(FATAL_ERROR "No TAO_UTIL_LIB_PATH set.")
else ()
    set(TAO_UTIL_LIB_PATH ${TAO_UTIL_LIB_PATH})
endif ()

add_definitions(-DBUILDING_DLL)


set(TAO_UTIL_LIB_INC_PATH "${TAO_UTIL_LIB_PATH}/Src/Inc")
set(TAO_UTIL_LIB_SRC_PATH "${TAO_UTIL_LIB_PATH}/Src/Src")


set(TAO_THIRD_PARTY_PATH ${TAO_UTIL_LIB_PATH}/ThirdParty)


set(TAO_UTIL_LIBS "")


include(${TAO_UTIL_LIB_PATH}/CMake/TBaseUtil.cmake)
include(${TAO_UTIL_LIB_PATH}/CMake/TLogUtil.cmake)
include(${TAO_UTIL_LIB_PATH}/CMake/TSysUtil.cmake)
include(${TAO_UTIL_LIB_PATH}/CMake/TConfigUtil.cmake)


set(TAO_UTIL_LIB_SRC
        ${T_BASE_UTIL_SRC}
        ${T_LOG_UTIL_SRC}
        ${T_SYSTEM_UTIL_SRC}
        ${T_CONFIG_UTIL_SRC}
)

add_library(${PROJECT_NAME} ${TAO_UTIL_LIB_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF)
    target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${TAO_UTIL_LIBS})