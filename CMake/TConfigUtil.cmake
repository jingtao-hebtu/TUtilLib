# *  @Copyright (c) tao.jing
# *
# *

set(T_CONFIG_INC_PATH ${TAO_UTIL_LIB_INC_PATH}/Config/)
set(T_CONFIG_SRC_PATH ${TAO_UTIL_LIB_SRC_PATH}/Config/)


include(${TAO_THIRD_PARTY_PATH}/yaml/cmake/yaml.cmake)
include(${TAO_THIRD_PARTY_PATH}/inicpp/cmake/inicpp.cmake)
include(${TAO_THIRD_PARTY_PATH}/json/cmake/json.cmake)


##################
## Include_Dirs ##
##################
include_directories(${T_CONFIG_INC_PATH})


##################
## Source_Dirs  ##
##################
aux_source_directory(${T_CONFIG_SRC_PATH} T_CONFIG_UTIL_SRC)


###########
## Libs  ##
###########
if (YAML_CPP_LIB_TARGET)
    list(APPEND TAO_UTIL_LIBS ${YAML_CPP_LIB_TARGET})
endif ()
