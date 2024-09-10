

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <MBx_api.h>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>

// 定义测试参数结构体
struct CRC16TestParam
{
    std::vector<uint8_t> data;
    uint16_t             expected_crc;
};

// 定义参数化测试类
class CRC16Test : public ::testing::TestWithParam<CRC16TestParam>
{
};

// 生成随机数据并计算CRC16
std::vector<CRC16TestParam> GenerateRandomCRC16TestParams(size_t num_tests, size_t min_size, size_t max_size)
{
    std::vector<CRC16TestParam> test_params;
    std::srand(std::time(0)); // 初始化随机数种子

    for(size_t i = 0; i < num_tests; ++i)
    {
        size_t               size = min_size + (std::rand( ) % (max_size - min_size + 1));
        std::vector<uint8_t> data(size);

        // 生成随机数据
        for(size_t j = 0; j < size; ++j)
        {
            data[j] = static_cast<uint8_t>(std::rand( ) % 256);
        }

        // 计算CRC16
        uint16_t crc = MBx_utility_crc16(data.data( ), data.size( ));

        // 添加调试信息
        // std::cout << "Generated data (size " << size << "): ";
        // for(auto byte : data)
        // {
        //     std::cout << "0x" << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(byte) << " ";
        // }
        // std::cout << "Expected CRC: 0x" << std::setw(4) << std::setfill('0') << std::hex << crc << std::endl;

        // 添加到测试参数集合
        test_params.push_back(CRC16TestParam{data, crc});
    }

    return test_params;
}

// 生成8组1~256字节的随机测试数据
std::vector<CRC16TestParam> random_test_cases = GenerateRandomCRC16TestParams(16, 1, 256);

// 实例化参数化测试的测试套件
INSTANTIATE_TEST_SUITE_P(CRC16TestCases, CRC16Test, ::testing::ValuesIn(random_test_cases));

// 定义参数化测试
TEST_P(CRC16Test, calculate_test)
{
    CRC16TestParam param = GetParam( );
    EXPECT_EQ(param.expected_crc, MBx_utility_crc16(param.data.data( ), param.data.size( )));
}