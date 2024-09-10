#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <MBx_api.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <mutex>
#include <thread>

// // 空的测试类
// class ModbusXTest : public ::testing::Test
// {
// protected:
//     virtual void SetUp( )
//     {
//         // 测试前的初始化代码可以放在这里
//     }

//     virtual void TearDown( )
//     {
//         // 测试后的清理代码可以放在这里
//     }
// };

// // 空的测试用例
// TEST_F(ModbusXTest, EmptyTestCase)
// {
//     // 测试内容，当前为空
//     ASSERT_TRUE(true); // 一个简单的断言，永远成功
// }

TEST(EmptyTestCase, AlwaysPasses)
{
    // 测试内容，当前为空
    ASSERT_TRUE(true); // 一个简单的断言，永远成功
}