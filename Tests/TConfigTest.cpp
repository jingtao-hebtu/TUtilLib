/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TConfigTest.cpp
   Author : tao.jing
   Date   : 2024/10/3
   Brief  :
**************************************************************************/
#include <gtest/gtest.h>
#include "TConfig.h"
#include "TSysUtils.h"


TEST(TConfigTest, AssertionTrue) {
    // Yaml ---
    bool found = false;
    std::string yaml_file_name = R"(Tests\Data\YamlSample.yml)";
    std::string yaml_file_path = TBase::searchFileInParentDirs(yaml_file_name, found);
    TBase::TConfig::instance().addYamlConfigFile(yaml_file_path);

    auto app_name = GET_STR_CONFIG("AppName");
    ASSERT_EQ(app_name, "BreakerAppName");

    auto key = GET_STR_CONFIG("NestedKey1", "NestedKey12", "NestedKey123");
    ASSERT_EQ(key, "123");

    auto array_value = TBase::TConfig::instance().getArrayConfig<int>({"IntArray"}, 1);
    ASSERT_EQ(array_value, 2);
    auto array_value2 = GET_ARR_INT_CONFIG(1, "IntArray");
    ASSERT_EQ(array_value2, 2);

    std::size_t seq_len = 0;
    TBase::TConfig::instance().getSeqNodeLen({"IntArray"}, seq_len);
    ASSERT_EQ(seq_len, 3);

    // Ini ---
    std::string ini_file_name = R"(Tests\Data\DLQ.ini)";
    std::string ini_file_path = TBase::searchFileInParentDirs(ini_file_name, found);
    TBase::TConfig::instance().addIniConfigFile(ini_file_path);

    auto name1 = GET_STR_CONFIG("DLQName", "Name1");
    ASSERT_EQ(name1, "进线1动作类型");

    auto cnt = GET_INT_CONFIG("DLQCount", "Count");
    ASSERT_EQ(cnt, 30);

    // Json ---
    std::string json_file_name = R"(Tests\Data\settings.json)";
    std::string json_file_path = TBase::searchFileInParentDirs(json_file_name, found);
    TBase::TConfig::instance().addJsonConfigFile(json_file_path);

    auto app_name_json = GET_STR_CONFIG("app_name");
    ASSERT_EQ(app_name_json, "视觉AI设备缺陷检测");

    auto lef_menu_size_minimum = GET_INT_CONFIG("lef_menu_size", "minimum");
    ASSERT_EQ(lef_menu_size_minimum, 50);

    auto minimum_size_1 = GET_ARR_INT_CONFIG(1, "minimum_size");
    ASSERT_EQ(minimum_size_1, 540);
}