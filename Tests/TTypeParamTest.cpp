/**************************************************************************

           Copyright(C), tao.jing All rights reserved

 **************************************************************************
   File   : TTypeParamTest.cpp
   Author : tao.jing
   Date   : 2024/10/3
   Brief  :
**************************************************************************/
#include <gtest/gtest.h>
#include "TTypeParamHandler.h"


TEST(TTypeParamTest, AssertionTrue) {
    TBase::TTypeParamHandler pm("param1", 10,
                                "param2", 3.14,
                                "param3", std::string("hello"),
                                "param4", true);
    TBase::TTypeParamHandler pm2{
        {"param3", std::string("123")}, {"param5", "New param."}};
    try {
        int param1 = pm["param1"];
        double param2 = pm["param2"];
        std::string param3 = pm["param3"];
        bool param4 = pm["param4"];

        ASSERT_EQ(param1, 10);
        ASSERT_EQ(param2, 3.14);
        ASSERT_EQ(param3, "hello");
        ASSERT_EQ(param4, true);

        pm.merge(pm2);

        std::string param3_2 = pm["param3"];
        std::string param5_2 = pm["param5"];
        ASSERT_EQ(param3_2, "123");
        ASSERT_EQ(param5_2, "New param.");

    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

