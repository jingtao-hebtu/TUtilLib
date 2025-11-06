/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TLogTest.cpp
   Author : tao.jing
   Date   : 2024/10/3
   Brief  :
**************************************************************************/
#include <gtest/gtest.h>
#include "TLog.h"


TEST(TLogTest, AssertionTrue) {
    /*
    int argc = 1;
    const char* filename = "TLogTest";
    char *argv[2] = {const_cast<char *>(filename), nullptr};
    loguru::init(argc, argv);
    */

    ASSERT_EQ(loguru::add_file("info.log", loguru::Append, loguru::Verbosity_INFO), true);
}