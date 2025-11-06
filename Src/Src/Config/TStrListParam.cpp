/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TStrListParam.cpp
   Author : tao.jing
   Date   : 2024/8/9
   Brief  :
**************************************************************************/
#include "TStrListParam.h"
#include "TException.h"


namespace TBase {

    TStrListParam::TStrListParam(const std::map<std::string, std::string> &params) : mStrParamMap(params) {

    }

    TStrListParam::TStrListParam(std::initializer_list<std::pair<std::string, std::string>> params) {
        for (const auto& param : params) {
            mStrParamMap[param.first] = param.second;
        }
    }

    std::string &TStrListParam::operator[](const std::string &key) {
        if (mStrParamMap.find(key) == mStrParamMap.end()) {
            TBASE_LOG_THROW_RUNTIME("Key %s not found.", key.c_str());
        }
        return mStrParamMap[key];
    }


};
