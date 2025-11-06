/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TParam.cpp
   Author : tao.jing
   Date   : 2023/12/11
   Brief  :
**************************************************************************/
#include "TParam.h"
#include <sstream>


namespace TBase {

    // --- TParam ---
    TParam::TParam(std::string value)
            : mStatus(TParam::VALUE), mValue(std::move(value)) {
    }

    TParam::TParam(const std::map<std::string, TParam> &map)
            : mStatus(TParam::MAP), mParamMap(map) {
    }

    template<class T>
    TParam &TParam::operator=(T value) {
        std::ostringstream sstr;
        sstr << value;

        mStatus = TParam::VALUE;
        mValue = sstr.str();
        return (*this);
    }

    template TAO_UTIL_API TParam &TParam::operator=<bool>(bool);

    template TAO_UTIL_API TParam &TParam::operator=<int>(int);

    template TAO_UTIL_API TParam &TParam::operator=<double>(double);

    template TAO_UTIL_API TParam &TParam::operator=<std::string>(std::string);

    TParam &TParam::operator[](const std::string &key) {
        if (mParamMap.find(key) == mParamMap.end()) {
            mParamMap.insert(std::map<std::string, TParam>::value_type(key, TParam("")));
        }
        return (mParamMap.find(key)->second);
    }

    TParam &TParam::operator[](const char key[]) {
        return (this->operator[](std::string(key)));
    }

};
