/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TJsonHandler.cpp
   Author : tao.jing
   Date   : 2024/7/25
   Brief  :
**************************************************************************/
#include "TJsonHandler.h"
#include <fstream>
#include <iostream>
#include "json.hpp"


using json = nlohmann::json;


namespace TBase {

    void reverseKeys(const json& j, std::vector<std::string>& keys, const std::string& prefix = "") {
        if (j.is_object()) {
            for (auto it = j.begin(); it != j.end(); ++it) {
                std::string new_prefix = prefix.empty() ? it.key() : prefix + it.key();
                new_prefix += ":";
                keys.push_back(new_prefix);
                reverseKeys(it.value(), keys, new_prefix);
            }
        }
    }

    class TJsonHandler::Impl {
    public:
        std::shared_ptr<json> mJson;

        Impl() {
            mJson = std::make_shared<json>();
        };

        ~Impl() = default;
    };

    TJsonHandler::TJsonHandler() : mImpl(std::make_unique<Impl>()) {

    }

    TJsonHandler::~TJsonHandler() = default;

    TJsonHandler::TJsonHandler(const char *file_name) {
        load(file_name);
    }

    TJsonHandler::TJsonHandler(const std::string &file_name) {
        load(file_name);
    }

    void TJsonHandler::load(const std::string &file_name) {
        *(mImpl->mJson) = json::parse(std::ifstream(file_name));
    }

    void TJsonHandler::fetchKeys(std::vector<std::string> &keys) {
        if (mImpl->mJson == nullptr) {
            std::cout << "[TJsonHandler::fetchKeys] No Json file loaded." << std::endl;
            return;
        }
        reverseKeys(*(mImpl->mJson), keys, "");
    }

    void TJsonHandler::showContents() {
        if (mImpl->mJson == nullptr) {
            std::cout << "[TJsonHandler::showContents] No Json file loaded." << std::endl;
            return;
        }
        for (auto it = mImpl->mJson->begin(); it != mImpl->mJson->end(); ++it) {
            if (it.value().is_object()) {
                std::cout << "Object: " << it.key() << ": " << it.value() << std::endl;
            } else if (it.value().is_array()) {
                std::cout << "Array: " << it.key() << ": " << it.value() << std::endl;
            } else {
                std::cout << "Leaf: " << it.key() << ": " << it.value() << std::endl;
            }
        }
    }

    template<typename T>
    T TJsonHandler::getValue(std::initializer_list<const std::string> keys) {
        const json* item = mImpl->mJson.get();
        for (const auto& key : keys) {
            if (item->contains(key)) {
                item = &(*item)[key];
            } else {
                throw std::runtime_error("Key not found: " + key);
            }
        }
        if (item->is_object()) {
            throw std::runtime_error("[TJsonHandler::getValue] Is object.");
        } else if (item->is_array()) {
            throw std::runtime_error("[TJsonHandler::getValue] Is array.");
        }
        return item->get<T>();
    }

    template TAO_UTIL_API int TJsonHandler::getValue<int>(std::initializer_list<const std::string>);

    template TAO_UTIL_API bool TJsonHandler::getValue<bool>(std::initializer_list<const std::string>);

    template TAO_UTIL_API float TJsonHandler::getValue<float>(std::initializer_list<const std::string>);

    template TAO_UTIL_API std::string TJsonHandler::getValue<std::string>(std::initializer_list<const std::string>);

    template<typename T>
    T TJsonHandler::getArrayValue(std::initializer_list<const std::string> keys, std::size_t index) {
        const json* item = mImpl->mJson.get();
        for (const auto& key : keys) {
            if (item->contains(key)) {
                item = &(*item)[key];
            } else {
                throw std::runtime_error("Key not found: " + key);
            }
        }
        if (!item->is_array()) {
            throw std::runtime_error("[TJsonHandler::getArrayValue] Is not array.");
        }
        if (index >= item->size()) {
            throw std::runtime_error("[TJsonHandler::getArrayValue] Index out of bounds.");
        }
        return (*item)[index].get<T>();
    }

    template TAO_UTIL_API int TJsonHandler::getArrayValue<int>(std::initializer_list<const std::string>, std::size_t);

    template TAO_UTIL_API bool TJsonHandler::getArrayValue<bool>(std::initializer_list<const std::string>, std::size_t);

    template TAO_UTIL_API float TJsonHandler::getArrayValue<float>(std::initializer_list<const std::string>, std::size_t);

    template TAO_UTIL_API std::string TJsonHandler::getArrayValue<std::string>(std::initializer_list<const std::string>, std::size_t);

};


