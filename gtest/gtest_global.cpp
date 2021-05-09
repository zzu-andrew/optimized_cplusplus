//
// Created by andrew on 2021/5/9.
//
#include <iostream>
#include <climits>
#include <gtest/gtest.h>


class GlobalTestSmpl : public testing::Environment
{
protected:
    void SetUp() override {
        // 准备工作
    }

    void TearDown() override {
        // 清理工作
    }

    int a; // 全局有效
};






int main(int argc, char* argv[]) {

    // 注册 全局事件 GlobalTestSmpl
    testing::AddGlobalTestEnvironment(new GlobalTestSmpl());
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
