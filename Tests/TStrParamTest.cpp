/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TStrParamTest.cpp
   Author : tao.jing
   Date   : 2024/6/2
   Brief  :
**************************************************************************/
#include "TStrParamHandler.h"
#include <iostream>
#include <gtest/gtest.h>


TEST(TStrParamTest, AssertionTrue) {

    TBase::TStrParamHandler param ({
            {"param1", "123"}, {"param2", "abc"}, {"param3", "4.56"}
    });

    ASSERT_EQ(param.get<int>("param1"), 123);
    ASSERT_EQ(param.get<std::string>("param2"), "abc");
    ASSERT_EQ(param.get<double>("param3"), 4.56);

    TBase::TStrParamHandler pm1 {
            {"param1", "123"},
            {"param2", "45.67"},
            {"param3", "true"}
    };

    TBase::TStrParamHandler pm2 {
            {"param2", "100.01"},
            {"param3", "false"},
            {"param4", "new_value"}
    };

    pm1.merge(pm2);

    ASSERT_EQ(pm1.get<int>("param1"), 123);
    ASSERT_EQ(pm1.get<double>("param2"), 100.01);
    ASSERT_EQ(pm1.get<bool>("param3"), false);
    ASSERT_EQ(pm1.get<std::string>("param4"), "new_value");
}


