/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TTypeParam.cpp
   Author : tao.jing
   Date   : 2024/5/29
   Brief  :
**************************************************************************/
#include "TTypeParam.h"
#include "TException.h"
#include <sstream>


namespace TBase {

    template<typename T>
    void TTypeParam<T>::setValue(const TTypeParamBase &newValue) {
        if (newValue.getType() == typeid(T)) {
            value = static_cast<const TTypeParam <T> &>(newValue).getValue();
        } else {
            TBASE_LOG_THROW_RUNTIME("Type mismatch when setting parameter value.");
        }
    }

    template<typename T>
    void TTypeParam<T>::setValue(T newValue) {
        value = newValue;
    }

    template class TAO_UTIL_API TTypeParam<int>;
    template class TAO_UTIL_API TTypeParam<double>;
    template class TAO_UTIL_API TTypeParam<std::string>;
    template class TAO_UTIL_API TTypeParam<bool>;

    template<typename T>
    std::string TTypeParam<T>::getValueAsString() const {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

};

