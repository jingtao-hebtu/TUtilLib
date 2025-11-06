/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TYamlHelper.cpp
   Author : tao.jing
   Date   : 2023/11/27
   Brief  : 
**************************************************************************/
#include "TYamlHelper.h"


namespace TBase::TYamlHelper {

    template<typename T>
    TBaseError getScalarValue(const std::string &yaml_file_path,
                              const std::string &key,
                              T &value) {
        auto node = YAML::LoadFile(yaml_file_path)[key];
        if (!node.IsDefined()) {
            return E_T_YAML_KEY_NOT_EXISTS;
        }

        if (node.IsScalar()) {
            value = node.as<T>();
            return E_T_SUCCESS;
        } else {
            return E_T_YAML_NODE_NOT_SCALAR;
        }
    }

    template TAO_UTIL_API TBaseError getScalarValue<std::string>(const std::string &yaml_file_path,
                                                                 const std::string &key, std::string &values);

    template<typename T>
    TBaseError getSeqValues(const std::string &yaml_file_path,
                            const std::string &key,
                            std::vector<T> &values) {
        auto node = YAML::LoadFile(yaml_file_path)[key];
        if (!node.IsDefined()) {
            return E_T_YAML_KEY_NOT_EXISTS;
        }

        if (node.IsSequence()) {
            for (auto &&it: node) {
                values.emplace_back(it.as<T>());
            }
            return E_T_SUCCESS;
        } else {
            return E_T_YAML_NODE_NOT_SEQ;
        }
    }

    template TAO_UTIL_API TBaseError getSeqValues<std::string>(const std::string &yaml_file_path,
                                                               const std::string &key,
                                                               std::vector<std::string> &values);


    void recursiveFetchKey(const YAML::Node &node,
                           std::map<void *, bool> &visited,
                           const std::string &prefix,
                           std::vector<std::string> &keys) {
        if (node.IsMap()) {
            for (auto it = node.begin(); it != node.end(); ++it) {
                const YAML::Node &key = it->first;
                const YAML::Node &value = it->second;

                std::string addi_prefix;
                if (key.IsScalar()) {
                    addi_prefix += key.as<std::string>();
                    addi_prefix += ":";
                }

                recursiveFetchKey(key, visited, prefix + addi_prefix, keys);
                if (value.IsMap()) {
                    recursiveFetchKey(value, visited, prefix + addi_prefix, keys);
                }
            }
        } else if (node.IsSequence()) {
            for (auto it = node.begin(); it != node.end(); ++it) {
                recursiveFetchKey(*it, visited, prefix, keys);
            }
        } else if (node.IsScalar()) {
            keys.emplace_back(prefix);
        } else {
            // do nothing
        }
    }

    void recursiveFetchKey(const YAML::Node &node, std::vector<std::string> &keys) {
        std::map<void *, bool> visited;
        recursiveFetchKey(node, visited, "", keys);
    }

};


