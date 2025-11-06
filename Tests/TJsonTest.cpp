/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TJsonTest.cpp
   Author : tao.jing
   Date   : 2024/7/26
   Brief  :
**************************************************************************/
#include "TJsonHandler.h"
#include <gtest/gtest.h>
#include "TSysUtils.h"
#include <iostream>


TEST(TJsonTest, AssertionTrue) {
    bool found = false;
    std::string file_name = R"(Tests\Data\settings.json)";
    std::string file_path = TBase::searchFileInParentDirs(file_name, found);

    TBase::TJsonHandler json_handler;
    json_handler.load(file_path);

    ASSERT_EQ(json_handler.getValue<std::string>({"app_name"}), "视觉AI设备缺陷检测");
    ASSERT_EQ(json_handler.getValue<int>({"lef_menu_size", "minimum"}), 50);
    ASSERT_EQ(json_handler.getArrayValue<int>({"startup_size"}, 1), 720);
}