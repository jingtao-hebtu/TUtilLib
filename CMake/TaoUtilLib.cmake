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


set(TAO_UTIL_LIB_INC_PATH "${TAO_UTIL_LIB_PATH}/Src/Inc")
set(TAO_UTIL_LIB_SRC_PATH "${TAO_UTIL_LIB_PATH}/Src/Src")


set(TAO_THIRD_PARTY_PATH ${TAO_UTIL_LIB_PATH}/ThirdParty)


set(TAO_UTIL_LIBS)


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

add_library(${PROJECT_NAME}_objects OBJECT ${TAO_UTIL_LIB_SRC})
set_target_properties(${PROJECT_NAME}_objects PROPERTIES POSITION_INDEPENDENT_CODE ON)

function(tutil_configure_library target)
    set_target_properties(${target} PROPERTIES
            DEBUG_POSTFIX "d"
            ARCHIVE_OUTPUT_DIRECTORY ${TAO_UTIL_LIB_OUTPUT_PATH}
            LIBRARY_OUTPUT_DIRECTORY ${TAO_UTIL_LIB_OUTPUT_PATH}
            RUNTIME_OUTPUT_DIRECTORY ${TAO_UTIL_LIB_OUTPUT_PATH}
    )

    if(WIN32)
        set_target_properties(${target} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS OFF)
        target_link_libraries(${target} PRIVATE Ws2_32)
    endif()

    target_link_libraries(${target} PRIVATE ${TAO_UTIL_LIBS})
endfunction()

if (TUTIL_BUILD_SHARED)
    add_library(${PROJECT_NAME}_shared SHARED $<TARGET_OBJECTS:${PROJECT_NAME}_objects>)
    target_compile_definitions(${PROJECT_NAME}_shared PRIVATE BUILDING_DLL)
    set_target_properties(${PROJECT_NAME}_shared PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    tutil_configure_library(${PROJECT_NAME}_shared)
    add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}_shared)
    add_library(${PROJECT_NAME}::shared ALIAS ${PROJECT_NAME}_shared)
endif ()

if (TUTIL_BUILD_STATIC)
    add_library(${PROJECT_NAME}_static STATIC $<TARGET_OBJECTS:${PROJECT_NAME}_objects>)
    set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
    tutil_configure_library(${PROJECT_NAME}_static)
    add_library(${PROJECT_NAME}::static ALIAS ${PROJECT_NAME}_static)
    if (NOT TUTIL_BUILD_SHARED)
        add_library(${PROJECT_NAME} ALIAS ${PROJECT_NAME}_static)
    endif ()
endif ()