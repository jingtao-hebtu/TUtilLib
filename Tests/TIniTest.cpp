/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TIniTest.cpp
   Author : tao.jing
   Date   : 2024/10/3
   Brief  :
**************************************************************************/
#include <gtest/gtest.h>
#include "TSysUtils.h"
#include "TIniHandler.h"


TEST(TIniTest, AssertionTrue) {
    bool found = false;
    std::string file_name = R"(Tests\Data\DLQ.ini)";
    std::string file_path = TBase::searchFileInParentDirs(file_name, found);

    TBase::TIniHandler ini_handler;
    ini_handler.load(file_path);

    ASSERT_EQ(ini_handler.getValue<std::string>({"DLQName", "Name2"}), "进线1动作时间");
    ASSERT_EQ(ini_handler.getValue<int>({"DLQCount", "Count"}), 30);
}