/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TParamTest.cpp
   Author : tao.jing
   Date   : 2024/10/1
   Brief  :
**************************************************************************/
#include <gtest/gtest.h>
#include "TParamHandler.h"


TEST(TParamTest, AssertionTrue) {
    TBase::TParamHandler cfg;
    cfg["mybool"] = false;

    cfg["myint"] = 42;
    cfg["mydouble"] = 42.42;
    cfg["player"]["health"]["current"] = 42;
    cfg["player"]["health"]["max"] = 100.05;

    ASSERT_EQ(static_cast<int>(cfg["myint"]), 42);
    ASSERT_EQ(static_cast<double>(cfg["mydouble"]), 42.42);
    ASSERT_EQ(static_cast<int>(cfg["player"]["health"]["current"]), 42);
    ASSERT_EQ(static_cast<double>(cfg["player"]["health"]["max"]), 100.05);
}