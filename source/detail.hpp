#pragma once  
#include <iostream>
#include <ctime>  
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>
#include <cstdio>
#include <random>
#include <chrono>
#include <iomanip>
#include <atomic>




namespace rpc{
    
#define LDBG 0
#define LINF 1
#define LERR 2

#define LDEFAULT LINF
  
// 末尾的换行\是转义，让其视为同一行。设置不定参数，获取当前时间并打印日志
#define LOG(level,format, ...) {\  
    if(level >= LDEFAULT){\
    time_t t = time(NULL);\  
    struct tm *lt = localtime(&t);\  
    char time_tmp[32] = { 0 };\  
    strftime(time_tmp, sizeof(time_tmp), "%m-%d %H:%M:%S", lt);\  
    printf("[%s]-[%s:%d] " format "\n", time_tmp, __FILE__, __LINE__, ##__VA_ARGS__);\ 
    }\
}
/*输出等级控制 */
#define DLOG(format, ...) LOG(LDBG, format, ##__VA_ARGS__);
#define ILOG(format, ...) LOG(LINF, format, ##__VA_ARGS__);
#define ELOG(format, ...) LOG(LERR, format, ##__VA_ARGS__);

    class JSON
    {
    public:
        static bool serialize(const Json::Value &val, std::string &body) // body为返回类型,bool无实义
        {
            std::stringstream ss;
            Json::StreamWriterBuilder swb; // 工厂(模式类对象swb)进行生产StreamWriter类
            std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
            int ret = sw->write(val, &ss);
            if (ret != 0)
            {
                 ELOG("Serialize failed!");
                return -1;
            }
            body = ss.str();
            return true;
        }

        static bool unserialize(const std::string &body, Json::Value &val)
        {
            // 实例化工厂对象
            Json::CharReaderBuilder crb;
            // 生产CharReader对象
            std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
            std::string errs;
            bool ret1 = cr->parse(body.c_str(), body.c_str() + body.size(), &val, &errs);
            if (ret1 == false)
            {
                ELOG("UnSerialize failed!\n:%s",errs.c_str());
                return false;
            }
            return true;
        }
    };
    class UUID
    {
        static std::string generateUUID()
        {
            std::stringstream ss; // 用于存放最终转换成的16进制UUID字符串

            // 1. 创建一个随机数设备对象，用于生成高质量的随机种子
            std::random_device rd;

            // 2. 使用随机设备初始化一个Mersenne Twister伪随机数生成器
            std::mt19937 generator(rd());

            // 3. 创建一个均匀分布的整数生成器，范围在0到255之间
            std::uniform_int_distribution<int> distribution(0, 255);

            // 4. 生成UUID的前8个字节（128位中的前64位，也就是前16位十六进制）
            for (int i = 0; i < 8; i++)
            {
                // 在第5个和第7个字节位置插入"-"
                if (i == 4 || i == 6)
                {              // 分别对应16进制的"8-4-4"
                    ss << "-"; // 流插入
                }
                // 生成一个随机数，转换为16进制，宽度设为2，不足补0
                ss << std::setw(2) << std::setfill('0') << std::hex << distribution(generator);
            }
            ss << "-"; //"8-4-4-"

            // 5. 生成UUID的后8个字节（128位中的后64位），作为序列号逐字节加
            //    使用静态原子变量保证线程安全地递增序列号
            static std::atomic<size_t> seq(1);
            size_t cur = seq.fetch_add(1); // 获取当前序列号并递增

            // 从高位开始存放，保持和原本的二进制格式一样,逐字节处理序列号，并转换为16进制格式加入字符串
            for (int i = 7; i >= 0; i--)
            {
                if (i == 5)
                {
                    ss << "-"; //"8-4-4-4-"
                }
                // 提取当前字节，转换为16进制，宽度设为2，不足补0
                ss << std::setw(2) << std::setfill('0') << std::hex << ((cur >> (i * 8)) & 0xFF);
            }

            // 返回生成的UUID字符串
            return ss.str();
        }
    };
}
