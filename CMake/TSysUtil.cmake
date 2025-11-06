# *  @Copyright (c) tao.jing
# *
# *

set(T_SYSTEM_INC_PATH ${TAO_UTIL_LIB_INC_PATH}/System/)
set(T_SYSTEM_SRC_PATH ${TAO_UTIL_LIB_SRC_PATH}/System/)


##################
## Include_Dirs ##
##################
include_directories(${T_SYSTEM_INC_PATH})

##################
## Source_Dirs  ##
##################

# Standard sources
set(STD_UTIL_IMPL_PATH ${T_SYSTEM_SRC_PATH}/Impl/)
aux_source_directory(${STD_UTIL_IMPL_PATH} T_SYSTEM_UTIL_SRC)

# OS related sources
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(T_SYSTEM_UTIL_IMPL_PATH ${STD_UTIL_IMPL_PATH}/Linux)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(T_SYSTEM_UTIL_IMPL_PATH ${STD_UTIL_IMPL_PATH}/Windows)
endif ()

aux_source_directory(${T_SYSTEM_UTIL_IMPL_PATH} T_SYSTEM_UTIL_SRC)
