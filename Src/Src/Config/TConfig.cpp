/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TConfig.cpp
   Author : tao.jing
   Date   : 2023/11/26
   Brief  : 
**************************************************************************/
#include "TConfig.h"
#include "TYamlHelper.h"
#include "TException.h"


namespace TBase {

    class TConfig::Impl {
    public:
        // ---- Yaml ----
        // Key : ConfigFile
        std::map<std::string, std::string> mYamlConfigFileMap;
        // ConfigFile : YamlNode
        std::map<std::string, YAML::Node> mYamlConfigNodeMap;

        // ---- Ini ----
        // Key : ConfigFile
        std::map<std::string, std::string> mIniConfigFileMap;
        // ConfigFile : Ini Handler
        std::map<std::string, TBase::TIniHandler*> mIniConfigNodeMap;

        // ---- Json ----
        // Key : ConfigFile
        std::map<std::string, std::string> mJsonConfigFileMap;
        // ConfigFile : Json Handler
        std::map<std::string, TBase::TJsonHandler*> mJsonConfigNodeMap;

        Impl() = default;

        ~Impl() = default;
    };

    TConfig::TConfig() : mImpl(std::make_unique<Impl>()) {

    }

    TConfig::~TConfig() {

        for (const auto &iter: mImpl->mIniConfigNodeMap) {
            delete iter.second;
        }
        mImpl->mIniConfigNodeMap.clear();

        for (const auto &iter: mImpl->mJsonConfigNodeMap) {
            delete iter.second;
        }
        mImpl->mJsonConfigNodeMap.clear();

    }

    std::string TConfig::concatKeys(std::initializer_list<const std::string> keys) {
        std::string concat_key;
        for (const auto &key: keys) {
            concat_key += key;
            concat_key += ":";
        }
        return concat_key;
    }

    YAML::Node TConfig::getYamlNode(std::initializer_list<const std::string> keys) {
        std::string concat_key = concatKeys(keys);
        if (mImpl->mYamlConfigFileMap.find(concat_key) == mImpl->mYamlConfigFileMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::getYamlNode] Key %s not exists.", concat_key.c_str());
        }
        std::string file_path = mImpl->mYamlConfigFileMap[concat_key];
        YAML::Node node = mImpl->mYamlConfigNodeMap[file_path];
        YAML::Node sub_node = YAML::Clone(node);
        for (const auto &key: keys) {
            sub_node = sub_node[key.c_str()];
        }
        return sub_node;
    }

    TBase::TIniHandler *TConfig::getIniHandler(std::initializer_list<const std::string> keys) {
        std::string concat_key = concatKeys(keys);
        if (mImpl->mIniConfigFileMap.find(concat_key) == mImpl->mIniConfigFileMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::getIniHandler] Key %s not exists.", concat_key.c_str());
        }
        std::string file_path = mImpl->mIniConfigFileMap[concat_key];
        return mImpl->mIniConfigNodeMap[file_path];
    }

    TBase::TJsonHandler *TConfig::getJsonHandler(std::initializer_list<const std::string> keys) {
        std::string concat_key = concatKeys(keys);
        if (mImpl->mJsonConfigFileMap.find(concat_key) == mImpl->mJsonConfigFileMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::getJsonHandler] Key %s not exists.", concat_key.c_str());
        }
        std::string file_path = mImpl->mJsonConfigFileMap[concat_key];
        return mImpl->mJsonConfigNodeMap[file_path];
    }

    template<typename T>
    inline T TConfig::getConfig(std::initializer_list<const std::string> keys) {
        std::string concat_key = concatKeys(keys);
        if (mImpl->mYamlConfigFileMap.find(concat_key) != mImpl->mYamlConfigFileMap.end()) {
            YAML::Node node = getYamlNode(keys);
            return node.as<T>();
        } else if (mImpl->mIniConfigFileMap.find(concat_key) != mImpl->mIniConfigFileMap.end()) {
            return getIniHandler(keys)->getValue<T>(keys);
        } else if (mImpl->mJsonConfigFileMap.find(concat_key) != mImpl->mJsonConfigFileMap.end()) {
            return getJsonHandler(keys)->getValue<T>(keys);
        }
        TBASE_LOG_THROW_RUNTIME("[TConfig::getConfig] Invalid Key %s.", concat_key.c_str());
    }

    template TAO_UTIL_API std::string TConfig::getConfig<std::string>(std::initializer_list<const std::string>);

    template TAO_UTIL_API bool TConfig::getConfig<bool>(std::initializer_list<const std::string>);

    template TAO_UTIL_API int TConfig::getConfig<int>(std::initializer_list<const std::string>);

    template TAO_UTIL_API float TConfig::getConfig<float>(std::initializer_list<const std::string>);

    template<typename T>
    T TConfig::getArrayConfig(std::initializer_list<const std::string> keys, std::size_t index) {
        std::string concat_key = concatKeys(keys);
        if (mImpl->mYamlConfigFileMap.find(concat_key) != mImpl->mYamlConfigFileMap.end()) {
            YAML::Node node = getYamlNode(keys);
            if (!node.IsSequence()) {
                TBASE_LOG_THROW_RUNTIME("[TConfig::getArrayConfig] Key %s is not array.", concat_key.c_str());
            } else if (index >= node.size()) {
                TBASE_LOG_THROW_RUNTIME("[TConfig::getArrayConfig] Array out of bounds. "
                                        "Array %s length %zu, but indexing %zu.", concat_key.c_str(), node.size(), index);
            }
            return node[index].as<T>();
        } else if (mImpl->mIniConfigFileMap.find(concat_key) != mImpl->mIniConfigFileMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::getArrayConfig] Invalid array access for ini config %s.",
                                    concat_key.c_str());
        } else if (mImpl->mJsonConfigFileMap.find(concat_key) != mImpl->mJsonConfigFileMap.end()) {
            return getJsonHandler(keys)->getArrayValue<T>(keys, index);
        }
        TBASE_LOG_THROW_RUNTIME("[TConfig::getArrayConfig] Invalid Key %s.", concat_key.c_str());
    }

    template TAO_UTIL_API std::string
    TConfig::getArrayConfig<std::string>(std::initializer_list<const std::string>, std::size_t index);

    template TAO_UTIL_API bool
    TConfig::getArrayConfig<bool>(std::initializer_list<const std::string>, std::size_t index);

    template TAO_UTIL_API int TConfig::getArrayConfig<int>(std::initializer_list<const std::string>, std::size_t index);

    template TAO_UTIL_API float
    TConfig::getArrayConfig<float>(std::initializer_list<const std::string>, std::size_t index);


    TBaseError TConfig::getSeqNodeLen(std::initializer_list<const std::string> keys,
                                      std::size_t &seq_len) {
        auto node = getYamlNode(keys);
        if (!node.IsSequence()) {
            seq_len = 0;
            return E_T_YAML_NODE_NOT_SEQ;
        } else {
            seq_len = node.size();
            return E_T_SUCCESS;
        }
    }

    void TConfig::addYamlConfigFile(const std::string &yaml_file_path) {
        if (mImpl->mYamlConfigNodeMap.find(yaml_file_path) != mImpl->mYamlConfigNodeMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::addYamlConfigFile] "
                                    "%s has already been added.", yaml_file_path.c_str());
        }

        // Load Yaml file
        auto node = YAML::LoadFile(yaml_file_path);

        // Fetch all keys from the file
        std::vector<std::string> keys;
        TBase::TYamlHelper::recursiveFetchKey(node, keys);

        // Store keys and node
        for (const auto &it: keys) {
            if (mImpl->mYamlConfigFileMap.find(it) != mImpl->mYamlConfigFileMap.end()) {
                TBASE_LOG_THROW_RUNTIME("[TConfig::addYamlConfigFile] Key %s already exists (In %s file).",
                                        it.c_str(), mImpl->mYamlConfigFileMap[it].c_str());
            }
            mImpl->mYamlConfigFileMap[it] = yaml_file_path;
        }
        mImpl->mYamlConfigNodeMap[yaml_file_path] = node;
    }

    void TConfig::addIniConfigFile(const std::string &ini_file_path) {
        if (mImpl->mIniConfigNodeMap.find(ini_file_path) != mImpl->mIniConfigNodeMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::addIniConfigFile] "
                                    "%s has already been added.", ini_file_path.c_str());
        }
        // Load ini file
        auto ini_handler = new TBase::TIniHandler();
        ini_handler->load(ini_file_path);

        // Fetch all keys from the file
        std::vector<std::string> keys;
        ini_handler->fetchKeys(keys);

        // Store keys and node
        for (const auto &it: keys) {
            if (mImpl->mIniConfigFileMap.find(it) != mImpl->mIniConfigFileMap.end()) {
                TBASE_LOG_THROW_RUNTIME("[TConfig::addIniConfigFile] Key %s already exists (In %s file).",
                                        it.c_str(), mImpl->mIniConfigFileMap[it].c_str());
            }
            mImpl->mIniConfigFileMap[it] = ini_file_path;
        }
        mImpl->mIniConfigNodeMap[ini_file_path] = ini_handler;
    }

    void TConfig::addJsonConfigFile(const std::string &json_file_path) {
        if (mImpl->mJsonConfigNodeMap.find(json_file_path) != mImpl->mJsonConfigNodeMap.end()) {
            TBASE_LOG_THROW_RUNTIME("[TConfig::addJsonConfigFile] "
                                    "%s has already been added.", json_file_path.c_str());
        }
        // Load ini file
        auto json_handler = new TBase::TJsonHandler();
        json_handler->load(json_file_path);

        // Fetch all keys from the file
        std::vector<std::string> keys;
        json_handler->fetchKeys(keys);

        // Store keys and node
        for (const auto &it: keys) {
            if (mImpl->mJsonConfigFileMap.find(it) != mImpl->mJsonConfigFileMap.end()) {
                TBASE_LOG_THROW_RUNTIME("[TConfig::addJsonConfigFile] Key %s already exists (In %s file).",
                                        it.c_str(), mImpl->mJsonConfigFileMap[it].c_str());
            }
            mImpl->mJsonConfigFileMap[it] = json_file_path;
        }
        mImpl->mJsonConfigNodeMap[json_file_path] = json_handler;
    }

};

