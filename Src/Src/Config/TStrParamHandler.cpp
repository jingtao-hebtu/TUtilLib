/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TStrParamHandler.cpp
   Author : tao.jing
   Date   : 2024/6/2
   Brief  :
**************************************************************************/
#include "TStrParamHandler.h"
#include <sstream>
#include <algorithm>
#include "TException.h"


namespace TBase {

    class TStrParamHandler::Impl {
    public:
        std::map<std::string, std::string> mParams;

        Impl() = default;

        ~Impl() = default;
    };

    TStrParamHandler::TStrParamHandler() : mImpl(std::make_unique<Impl>()) {

    }

    TStrParamHandler::~TStrParamHandler() = default;

    TStrParamHandler::TStrParamHandler(std::initializer_list<std::pair<std::string, std::string>> args)
            : mImpl(std::make_unique<Impl>()) {
        for (const auto &arg: args) {
            mImpl->mParams[arg.first] = arg.second;
        }
    }

    TStrParamHandler::TStrParamHandler(const TStrListParam &args) : mImpl(std::make_unique<Impl>()) {
        for (const auto &arg: args) {
            mImpl->mParams[arg.first] = arg.second;
        }
    }

    template<typename T>
    T TStrParamHandler::get(const std::string &name) const {
        return convertTo<T>(name);
    }

    template TAO_UTIL_API bool TStrParamHandler::get(const std::string &name) const;

    template TAO_UTIL_API int TStrParamHandler::get(const std::string &name) const;

    template TAO_UTIL_API double TStrParamHandler::get(const std::string &name) const;

    template TAO_UTIL_API std::string TStrParamHandler::get(const std::string &name) const;


    template<typename T>
    void TStrParamHandler::set(const std::string &name, const T &value) {
        std::ostringstream oss;
        oss << value;
        mImpl->mParams[name] = oss.str();
    }

    const std::map<std::string, std::string>& TStrParamHandler::getParams() const {
        return mImpl->mParams;
    }

    template TAO_UTIL_API void TStrParamHandler::set(const std::string &name, const bool &value);

    template TAO_UTIL_API void TStrParamHandler::set(const std::string &name, const int &value);

    template TAO_UTIL_API void TStrParamHandler::set(const std::string &name, const double &value);

    template TAO_UTIL_API void TStrParamHandler::set(const std::string &name, const std::string &value);

    template<typename T>
    T TStrParamHandler::convertTo(const std::string &name) const {
        auto it = mImpl->mParams.find(name);
        if (it != mImpl->mParams.end()) {
            std::istringstream iss(it->second);
            T value;
            if constexpr (std::is_same<T, std::string>::value) {
                if (iss.str().size() == 0) {
                    return std::string("");
                }

                if (!std::getline(iss, value)) {
                    TBASE_LOG_THROW_RUNTIME("Failed to convert parameter %s.", name.c_str());
                }
                return value;
            }
            if (!(iss >> value)) {
                TBASE_LOG_THROW_RUNTIME("Failed to convert parameter %s.", name.c_str());
            }
            return value;
        }
        TBASE_LOG_THROW_RUNTIME("Parameter %s not found.", name.c_str());
    }

    template TAO_UTIL_API int TStrParamHandler::convertTo(const std::string &name) const;

    template TAO_UTIL_API double TStrParamHandler::convertTo(const std::string &name) const;

    template TAO_UTIL_API std::string TStrParamHandler::convertTo(const std::string &name) const;

    template<>
    bool TStrParamHandler::convertTo<bool>(const std::string &name) const {
        auto it = mImpl->mParams.find(name);
        if (it != mImpl->mParams.end()) {
            std::string val = it->second;
            std::transform(val.begin(), val.end(), val.begin(), ::tolower);
            return (val == "true" || val == "1");
        }
        return false;
    }

    void TStrParamHandler::merge(const TStrParamHandler &other) {
        for (const auto &param: other.getParams()) {
            mImpl->mParams[param.first] = param.second;
        }
    }

    void TStrParamHandler::merge(const std::map<std::string, std::string> &params) {
        for (const auto &param: params) {
            mImpl->mParams[param.first] = param.second;
        }
    }



};

