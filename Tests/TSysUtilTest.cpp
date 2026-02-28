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

#ifdef _WIN32
    std::string dir_full_path = "C:\\a\\b\\c";
    std::string path_a = "C:/a/";
    std::string path_b = "b";
    std::string path_c = "c";
    std::string path_joined = TBase::joinPath({path_a, path_b, path_c});
    EXPECT_EQ(path_joined, dir_full_path);

    std::string file_full_path = "C:\\a\\b\\c";
    path_joined = TBase::joinPath({path_a, path_b});
    path_joined = TBase::joinPath(path_joined, path_c);
    EXPECT_EQ(path_joined, file_full_path);
#elif __linux__
    std::string dir_full_path = "/a/b/c/";
    std::string path_a = "/a/";
    std::string path_b = "b";
    std::string path_c = "c";
    std::string path_joined = TBase::joinPath({path_a, path_b, path_c});
    EXPECT_EQ(path_joined, dir_full_path);

    std::string file_full_path = "/a/b/c";
    path_joined = TBase::joinPath({path_a, path_b});
    path_joined = TBase::joinPath(path_joined, path_c);
    EXPECT_EQ(path_joined, file_full_path);
#endif
}


