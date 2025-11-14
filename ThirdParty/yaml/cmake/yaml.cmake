# *  @Copyright (c) tao.jing
# *
# *

set(THIRD_PARTY_YAML_CPP_PATH "${THIRD_PARTY_YAML_CPP_PATH}" CACHE PATH "Optional yaml-cpp installation prefix")

set(_yaml_cpp_config_hints)
if (THIRD_PARTY_YAML_CPP_PATH)
    list(APPEND _yaml_cpp_config_hints
            "${THIRD_PARTY_YAML_CPP_PATH}"
            "${THIRD_PARTY_YAML_CPP_PATH}/share/cmake/yaml-cpp"
            "${THIRD_PARTY_YAML_CPP_PATH}/lib/cmake/yaml-cpp")
endif ()

set(YAML_CPP_LIB_TARGET "")

find_package(yaml-cpp CONFIG QUIET HINTS ${_yaml_cpp_config_hints})

if (NOT yaml-cpp_FOUND)
    if (THIRD_PARTY_YAML_CPP_PATH)
        find_package(yaml-cpp QUIET
                PATHS ${THIRD_PARTY_YAML_CPP_PATH}
                NO_DEFAULT_PATH)
    else ()
        find_package(yaml-cpp QUIET)
    endif ()
endif ()

if (yaml-cpp_FOUND)
    if (TARGET yaml-cpp::yaml-cpp)
        set(YAML_CPP_LIB_TARGET yaml-cpp::yaml-cpp)
    elseif (TARGET yaml-cpp)
        set(YAML_CPP_LIB_TARGET yaml-cpp)
    elseif (DEFINED YAML_CPP_LIBRARIES)
        set(YAML_CPP_LIB_TARGET ${YAML_CPP_LIBRARIES})
        if (DEFINED YAML_CPP_INCLUDE_DIR)
            include_directories(${YAML_CPP_INCLUDE_DIR})
        elseif (DEFINED YAML_CPP_INCLUDE_DIRS)
            include_directories(${YAML_CPP_INCLUDE_DIRS})
        endif ()
    else ()
        message(FATAL_ERROR "yaml-cpp was found but no CMake target was provided.")
    endif ()
else ()
    if (NOT THIRD_PARTY_YAML_CPP_PATH)
        message(FATAL_ERROR "yaml-cpp was not found. Set THIRD_PARTY_YAML_CPP_PATH to specify a custom installation prefix.")
    endif ()

    set(_yaml_cpp_include_dir ${THIRD_PARTY_YAML_CPP_PATH}/include)
    if (EXISTS ${_yaml_cpp_include_dir})
        include_directories(${_yaml_cpp_include_dir})
    endif ()

    set(_yaml_cpp_library_dir ${THIRD_PARTY_YAML_CPP_PATH}/lib)

    find_library(YAML_CPP_LIBRARY_RELEASE NAMES yaml-cpp PATHS ${_yaml_cpp_library_dir})
    find_library(YAML_CPP_LIBRARY_DEBUG NAMES yaml-cppd yaml-cpp PATHS ${_yaml_cpp_library_dir})

    if (NOT YAML_CPP_LIBRARY_RELEASE AND NOT YAML_CPP_LIBRARY_DEBUG)
        message(FATAL_ERROR "yaml-cpp was not found in ${THIRD_PARTY_YAML_CPP_PATH}.")
    endif ()

    add_library(TaoYamlCpp INTERFACE)

    if (EXISTS ${_yaml_cpp_include_dir})
        target_include_directories(TaoYamlCpp INTERFACE ${_yaml_cpp_include_dir})
    endif ()

    if (YAML_CPP_LIBRARY_RELEASE AND YAML_CPP_LIBRARY_DEBUG AND NOT "${YAML_CPP_LIBRARY_RELEASE}" STREQUAL "${YAML_CPP_LIBRARY_DEBUG}")
        target_link_libraries(TaoYamlCpp INTERFACE
                optimized ${YAML_CPP_LIBRARY_RELEASE}
                debug ${YAML_CPP_LIBRARY_DEBUG})
    elseif (YAML_CPP_LIBRARY_RELEASE)
        target_link_libraries(TaoYamlCpp INTERFACE ${YAML_CPP_LIBRARY_RELEASE})
    else ()
        target_link_libraries(TaoYamlCpp INTERFACE ${YAML_CPP_LIBRARY_DEBUG})
    endif ()

    set(YAML_CPP_LIB_TARGET TaoYamlCpp)
endif ()
