/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TParamHandler.cpp
   Author : tao.jing
   Date   : 2023/12/10
   Brief  :
**************************************************************************/
#include "TParamHandler.h"
#include <sstream>
#include <utility>


namespace TBase {

    class TParamHandler::Impl {
    public:
        std::map<std::string, TParam> mParams;

        Impl() = default;

        ~Impl() = default;
    };

    TParamHandler::TParamHandler() : mImpl(std::make_unique<Impl>()) {

    }

    TParamHandler::~TParamHandler() = default;

    // --- TParamHandler ---
    TParam &TParamHandler::operator[](const std::string &key) {
        if (mImpl->mParams.find(key) == mImpl->mParams.end()) {
            mImpl->mParams.insert(std::map<std::string, TParam>::value_type(key, TParam("")));
        }
        return (mImpl->mParams.find(key)->second);
    }

    TParam &TParamHandler::operator[](const char key[]) {
        return (this->operator[](std::string(key)));
    }

};

