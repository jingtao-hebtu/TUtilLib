# *  @Copyright (c) tao.jing
# *
# *


set(T_BASE_INC_PATH ${TAO_UTIL_LIB_INC_PATH}/Base/)
set(T_BASE_SRC_PATH ${TAO_UTIL_LIB_SRC_PATH}/Base/)


##################
## Include_Dirs ##
##################
include_directories(${T_BASE_INC_PATH})


##################
## Source_Dirs  ##
##################
aux_source_directory(${T_BASE_SRC_PATH} T_BASE_UTIL_SRC)
