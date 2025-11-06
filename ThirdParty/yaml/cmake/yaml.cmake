# *  @Copyright (c) tao.jing
# *
# *

if (CMAKE_BUILD_TYPE MATCHES DEBUG OR
        CMAKE_BUILD_TYPE MATCHES Debug OR
        CMAKE_BUILD_TYPE MATCHES debug)
    set(THIRD_PARTY_YAML_CPP_PATH "E:/Software/Library/YamlCpp/install/0_7_0/debug")
    set(YAML_CPP_LIB_NAME yaml-cppd)
else ()
    set(THIRD_PARTY_YAML_CPP_PATH "E:/Software/Library/YamlCpp/install/0_7_0/release")
    set(YAML_CPP_LIB_NAME yaml-cpp)
endif ()

set(YAML_CPP_CMAKE_CONFIG_PATH ${THIRD_PARTY_YAML_CPP_PATH}/share/cmake/yaml-cpp/)


UNSET(yaml-cpp)
include_directories(${THIRD_PARTY_YAML_CPP_PATH}/include)
find_package(yaml-cpp PATHS ${YAML_CPP_CMAKE_CONFIG_PATH} NO_CACHE REQUIRED
        NO_DEFAULT_PATH
        NO_CMAKE_PATH
        NO_CMAKE_BUILDS_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_CMAKE_SYSTEM_PATH
        NO_CMAKE_ENVIRONMENT_PATH
        NO_SYSTEM_ENVIRONMENT_PATH
        NO_PACKAGE_ROOT_PATH
)

link_directories(${THIRD_PARTY_YAML_CPP_PATH}/lib)
