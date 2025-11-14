/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TIniHelper.cpp
   Author : tao.jing
   Date   : 2023/12/12
   Brief  :
**************************************************************************/
#include "TIniHandler.h"
#include "TException.h"
#include "inicpp.h"
#include <iostream>


namespace TBase {

    class TIniHandler::Impl {
    public:
        std::shared_ptr<ini::IniFile> mIni{nullptr};

        Impl() {
            mIni = std::make_shared<ini::IniFile>();
        };

        ~Impl() = default;
    };

    TIniHandler::TIniHandler() : mImpl(std::make_unique<Impl>()) {

    }

    TIniHandler::TIniHandler(const std::string &file_name) {
        mImpl->mIni = std::make_shared<ini::IniFile>();
        load(file_name);
    }

    TIniHandler::TIniHandler(const char *file_name) {
        mImpl->mIni = std::make_shared<ini::IniFile>();
        load(file_name);
    }

    void TIniHandler::load(const std::string &file_name) {
        mImpl->mIni->load(file_name);
    }

    void TIniHandler::showContents() {
        if (mImpl->mIni == nullptr) {
            std::cout << "[TIniHelper::showContents] No Ini file loaded." << std::endl;
            return;
        }
        std::cout << "Parsed ini contents" << std::endl;
        std::cout << "Has " << mImpl->mIni->size() << " sections" << std::endl;
        for (const auto &sectionPair: *mImpl->mIni) {
            const std::string &sectionName = sectionPair.first;
            const ini::IniSection &section = sectionPair.second;
            std::cout << "Section '" << sectionName << "' has " << section.size() << " fields" << std::endl;

            for (const auto &fieldPair: sectionPair.second) {
                const std::string &fieldName = fieldPair.first;
                const ini::IniField &field = fieldPair.second;
                std::cout << "  Field '" << fieldName << "' Value '" << field.as<std::string>() << "'" << std::endl;
            }
        }
    }

    void TIniHandler::fetchKeys(std::vector<std::string> &keys) {
        if (mImpl->mIni == nullptr) {
            std::cout << "[TIniHandler::fetchKeys] No Ini file loaded." << std::endl;
            return;
        }
        for (const auto &section_pair: *(mImpl->mIni)) {
            const std::string &section_name = section_pair.first;
            for (const auto &field_pair: section_pair.second) {
                const std::string &field_name = field_pair.first;
                keys.emplace_back(section_name + ":" + field_name + ":");
            }
        }
    }

    TIniHandler::~TIniHandler() = default;

    template<typename T>
    T TIniHandler::getValue(std::initializer_list<const std::string> keys) {
        if (keys.size() != 2) {
            TBASE_LOG_THROW_RUNTIME("[TIniHandler::getValue] Invalid keys size %zu.", keys.size());
        }
        auto it = keys.begin();
        std::string section_name = *it++;
        std::string field_name = *it;
        return (*(mImpl->mIni))[section_name][field_name].as<T>();
    }

    template TAO_UTIL_API int TIniHandler::getValue<int>(std::initializer_list<const std::string>);

    template TAO_UTIL_API bool TIniHandler::getValue<bool>(std::initializer_list<const std::string>);

    template TAO_UTIL_API float TIniHandler::getValue<float>(std::initializer_list<const std::string>);

    template TAO_UTIL_API std::string TIniHandler::getValue<std::string>(std::initializer_list<const std::string>);

};


