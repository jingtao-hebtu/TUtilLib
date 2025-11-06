# *  @Copyright (c) tao.jing
# *
# *

set(T_LOG_INC_PATH ${TAO_UTIL_LIB_INC_PATH}/Log/)
set(T_LOG_SRC_PATH ${TAO_UTIL_LIB_SRC_PATH}/Log/)


##################
## Include_Dirs ##
##################
include_directories(${T_LOG_INC_PATH})


##################
## Source_Dirs  ##
##################
#aux_source_directory(${T_LOG_SRC_PATH} T_LOG_UTIL_SRC)
set(T_LOG_UTIL_SRC ${T_LOG_SRC_PATH}/TLog.cpp)
