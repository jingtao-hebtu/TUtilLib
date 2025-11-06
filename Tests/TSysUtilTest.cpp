/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TbSysUtilTest.cpp
   Author : tao.jing
   Date   : 2023/11/12
   Brief  :
**************************************************************************/
#include "TSysUtils.h"
#include <gtest/gtest.h>


TEST(SysUtilTest, AssertionTrue) {
    std::string user_name = TBase::getUserName();

    std::string dir_name = "..\\bin_for_gtest\\";
    EXPECT_FALSE(TBase::dirExists(dir_name));

    EXPECT_TRUE(TBase::createDir(dir_name));
    EXPECT_TRUE(TBase::dirExists(dir_name));
    EXPECT_TRUE(TBase::delDir(dir_name));

    EXPECT_TRUE(TBase::fileExists(__FILE__));
}


