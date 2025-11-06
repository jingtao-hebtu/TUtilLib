/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TTypeParamHandler.cpp
   Author : tao.jing
   Date   : 2024/5/29
   Brief  :
**************************************************************************/
#include "TTypeParamHandler.h"
#include <sstream>
#include "TException.h"


namespace TBase {

    template<typename T>
    TTypeParamHandler::ParamAccessor &TTypeParamHandler::ParamAccessor::operator=(T newValue) {
        if (param->getType() == typeid(T)) {
            std::static_pointer_cast<TTypeParam<T>>(param)->setValue(newValue);
        } else {
            TBASE_LOG_THROW_RUNTIME("Type mismatch when setting parameter value");
        }
        return *this;
    }

    template TAO_UTIL_API TTypeParamHandler::ParamAccessor &TTypeParamHandler::ParamAccessor::operator=<bool>(bool);

    template TAO_UTIL_API TTypeParamHandler::ParamAccessor &TTypeParamHandler::ParamAccessor::operator=<int>(int);

    template TAO_UTIL_API TTypeParamHandler::ParamAccessor &TTypeParamHandler::ParamAccessor::operator=<double>(double);

    template TAO_UTIL_API TTypeParamHandler::ParamAccessor &TTypeParamHandler::ParamAccessor::operator=<std::string>(std::string);


    TTypeParamHandler::TTypeParamHandler(std::initializer_list<std::pair<std::string, TInitParamTypes>> args) {
        for (const auto &arg: args) {
            addParameter(arg.first, arg.second);
        }
    }

    void TTypeParamHandler::addParameter(const std::string &name, const TInitParamTypes &value) {
        std::visit([this, &name](auto &&val) {
            using T = std::decay_t<decltype(val)>;
            mParameters[name] = std::make_shared<TTypeParam<T>>(val);
        }, value);
    }

    template<typename T>
    void TTypeParamHandler::setParameter(const std::string &name, T value) {
        auto it = mParameters.find(name);
        if (it != mParameters.end()) {
            if (it->second->getType() == typeid(T)) {
                std::static_pointer_cast<TTypeParam<T>>(it->second)->setValue(value);
            } else {
                TBASE_LOG_THROW_RUNTIME("Type mismatch when setting parameter value, param name %s.", name.c_str());
            }
        } else {
            TBASE_LOG_THROW_RUNTIME("Parameter not found, param name %s.", name.c_str());
        }
    }

    template TAO_UTIL_API void TTypeParamHandler::setParameter(const std::string &name, bool value);

    template TAO_UTIL_API void TTypeParamHandler::setParameter(const std::string &name, int value);

    template TAO_UTIL_API void TTypeParamHandler::setParameter(const std::string &name, double value);

    template TAO_UTIL_API void TTypeParamHandler::setParameter(const std::string &name, std::string value);


    TTypeParamHandler::ParamAccessor TTypeParamHandler::operator[](const std::string &name) {
        auto it = mParameters.find(name);
        if (it != mParameters.end()) {
            return {it->second};
        }
        TBASE_LOG_THROW_RUNTIME("Parameter not found, param name %s.", name.c_str());
    }

    void TTypeParamHandler::merge(const TTypeParamHandler &other) {
        for (const auto &[name, param]: other.mParameters) {
            if (mParameters.find(name) != mParameters.end()) {
                if (mParameters[name]->getType() == param->getType()) {
                    mParameters[name]->setValue(*param);
                } else if (mParameters[name]->getType() == typeid(std::string) && param->getType() == typeid(int)) {
                    std::string value = intToHexString(std::static_pointer_cast<TTypeParam<int>>(param)->getValue());
                    std::static_pointer_cast<TTypeParam<std::string>>(mParameters[name])->setValue(value);
                } else if (mParameters[name]->getType() == typeid(int) && param->getType() == typeid(std::string)) {
                    int value = stringToInt(std::static_pointer_cast<TTypeParam<std::string>>(param)->getValue());
                    std::static_pointer_cast<TTypeParam<int>>(mParameters[name])->setValue(value);
                } else {
                    TBASE_LOG_THROW_RUNTIME("Type mismatch when setting parameter value, param name %s.", name.c_str());
                }
            } else {
                mParameters[name] = param;
            }
        }
    }

    void TTypeParamHandler::merge(std::initializer_list<std::pair<std::string, TInitParamTypes>> args) {
        merge(TTypeParamHandler(args));
    }

    std::map<std::string, std::string>
    TTypeParamHandler::getParamValues(std::initializer_list<std::string> paramNames) const {
        std::map<std::string, std::string> param_map;
        for (const auto &name: paramNames) {
            auto it = mParameters.find(name);
            if (it != mParameters.end()) {
                param_map[name] = it->second->getValueAsString();
            } else {
                TBASE_LOG_THROW_RUNTIME("Parameter not found: %s.", name.c_str());
            }
        }
        return param_map;
    }

    void TTypeParamHandler::printParameters() const {
        for (const auto &[name, param]: mParameters) {
            std::cout << name << ": ";
            if (param->getType() == typeid(int)) {
                std::cout << std::static_pointer_cast<TTypeParam<int>>(param)->getValue() << std::endl;
            } else if (param->getType() == typeid(double)) {
                std::cout << std::static_pointer_cast<TTypeParam<double>>(param)->getValue() << std::endl;
            } else if (param->getType() == typeid(std::string)) {
                std::cout << std::static_pointer_cast<TTypeParam<std::string>>(param)->getValue() << std::endl;
            } else if (param->getType() == typeid(bool)) {
                std::cout << std::static_pointer_cast<TTypeParam<bool>>(param)->getValue() << std::endl;
            }
        }
    }

    int TTypeParamHandler::stringToInt(const std::string &str) {
        int base = 10;
        size_t pos = 0;

        // Determine whether hexadecimal
        if (str.size() > 2 && str[0] == '0' && str[1] == 'x') {
            base = 16;
            pos = 2;
        } else if (str.size() > 1 && str[0] == '#') {
            base = 16;
            pos = 1;
        }

        try {
            return std::stoi(str.substr(pos), nullptr, base);
        } catch (const std::invalid_argument &e) {
            TBASE_LOG_THROW_RUNTIME("Invalid argument %s: not a valid integer or hex string, %s.", e.what(), str.c_str());
        } catch (const std::out_of_range &e) {
            TBASE_LOG_THROW_RUNTIME("Out of range %s: integer value is too large, %s.", e.what(), str.c_str());
        }
    }

    std::string TTypeParamHandler::intToHexString(int value) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << value;
        return oss.str();
    }


};