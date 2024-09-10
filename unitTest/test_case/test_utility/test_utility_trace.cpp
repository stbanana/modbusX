#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <MBx_api.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>

extern uint8_t MBxErrTraceTop;

class MBxErrTraceTest : public ::testing::Test
{
protected:
    void SetUp( ) override
    {
        // 初始化错误追踪栈
        MBxErrTraceTop = 0;
    }

    void TearDown( ) override
    {
        // 清理错误追踪栈
        MBxErrTraceTop = 0;
    }
};

TEST_F(MBxErrTraceTest, AddTraceTest)
{
    MBxErrTraceAdd(1, 0, 1, 0x12345678);

    ASSERT_EQ(MBxErrTraceTop, 1);
    EXPECT_EQ(MBxErrTraceStack[0].SlaveID, 1);
    EXPECT_EQ(MBxErrTraceStack[0].mode, 0);
    EXPECT_EQ(MBxErrTraceStack[0].State, 1);
    EXPECT_EQ(MBxErrTraceStack[0].ErrCode, 0x12345678);
}

TEST_F(MBxErrTraceTest, GetTraceTest)
{
    // 先添加一个错误记录
    MBxErrTraceAdd(1, 0, 1, 0x12345678);

    uint8_t  SlaveID = 0;
    uint8_t  mode    = 0;
    uint8_t  State   = 0;
    uint32_t ErrCode = 0;

    uint32_t result  = MBxErrTraceGet(&SlaveID, &mode, &State, &ErrCode);

    ASSERT_EQ(result, MBX_API_RETURN_DEFAULT);
    EXPECT_EQ(SlaveID, 1);
    EXPECT_EQ(mode, 0);
    EXPECT_EQ(State, 1);
    EXPECT_EQ(ErrCode, 0x12345678);
    ASSERT_EQ(MBxErrTraceTop, 0); // 确保栈已被弹出
}

TEST_F(MBxErrTraceTest, GetEmptyTraceTest)
{
    uint8_t  SlaveID = 0;
    uint8_t  mode    = 0;
    uint8_t  State   = 0;
    uint32_t ErrCode = 0;

    uint32_t result  = MBxErrTraceGet(&SlaveID, &mode, &State, &ErrCode);

    ASSERT_EQ(result, MBX_API_RETURN_BUFFER_EMPTY);
}

TEST_F(MBxErrTraceTest, AddTraceWhenStackFullTest)
{
    // 假设MBX_ERR_TRACE_MAX_NUM是栈的最大容量
    for(int i = 0; i < MBX_ERR_TRACE_MAX_NUM; ++i)
    {
        MBxErrTraceAdd(i, i % 2, i % 3, i);
    }

    // 尝试添加多于最大容量的错误记录
    MBxErrTraceAdd(99, 1, 1, 0x99999999);

    ASSERT_EQ(MBxErrTraceTop, MBX_ERR_TRACE_MAX_NUM); // 确保没有添加超出栈容量的记录

    // 检查最后一个记录是否未被覆盖
    EXPECT_EQ(MBxErrTraceStack[MBX_ERR_TRACE_MAX_NUM - 1].SlaveID, MBX_ERR_TRACE_MAX_NUM - 1);
    EXPECT_EQ(MBxErrTraceStack[MBX_ERR_TRACE_MAX_NUM - 1].mode, (MBX_ERR_TRACE_MAX_NUM - 1) % 2);
    EXPECT_EQ(MBxErrTraceStack[MBX_ERR_TRACE_MAX_NUM - 1].State, (MBX_ERR_TRACE_MAX_NUM - 1) % 3);
    EXPECT_EQ(MBxErrTraceStack[MBX_ERR_TRACE_MAX_NUM - 1].ErrCode, MBX_ERR_TRACE_MAX_NUM - 1);
}
