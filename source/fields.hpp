#pragma once
#include <string>
#include <unordered_map>

namespace rpc {
    //定义字段宏，便于我们对JSON字段名称进行修改
    #define KEY_METHOD      "method"//方法名称
    #define KEY_PARAMS      "parameters"//方法参数
    #define KEY_TOPIC_KEY   "topic_key"//主题名称
    #define KEY_TOPIC_MSG   "topic_msg"//主题消息
    #define KEY_OPTYPE      "optype"//操作类型
    #define KEY_HOST        "host"
    #define KEY_HOST_IP     "ip"
    #define KEY_HOST_PORT   "port"
    #define KEY_RCODE       "rcode"//rcp响应码
    #define KEY_RESULT      "result"//rpc响应结构

    enum class MType {//消息类型
        REQ_RPC = 0,
        RSP_RPC,
        REQ_TOPIC,
        RSP_TOPIC,
        REQ_SERVICE,
        RSP_SERVICE
    };

    enum class RCode {//响应码
        RCODE_OK = 0,
        RCODE_PARSE_FAILED,
        RCODE_ERROR_MSGTYPE,
        RCODE_INVALID_MSG,
        RCODE_DISCONNECTED,
        RCODE_INVALID_PARAMS,
        RCODE_NOT_FOUND_SERVICE,
        RCODE_INVALID_OPTYPE,
        RCODE_NOT_FOUND_TOPIC,
        RCODE_INTERNAL_ERROR
    };
    static std::string errReason(RCode code) { //错误信息，JSON类型
        static std::unordered_map<RCode, std::string> err_map = {//使用map快速匹配到
            {RCode::RCODE_OK, "成功处理！"},
            {RCode::RCODE_PARSE_FAILED, "消息解析失败！"},
            {RCode::RCODE_ERROR_MSGTYPE, "消息类型错误！"},
            {RCode::RCODE_INVALID_MSG, "无效消息"},
            {RCode::RCODE_DISCONNECTED, "连接已断开！"},
            {RCode::RCODE_INVALID_PARAMS, "无效的Rpc参数！"},
            {RCode::RCODE_NOT_FOUND_SERVICE, "没有找到对应的服务！"},
            {RCode::RCODE_INVALID_OPTYPE, "无效的操作类型"},
            {RCode::RCODE_NOT_FOUND_TOPIC, "没有找到对应的主题！"},
            {RCode::RCODE_INTERNAL_ERROR, "内部错误！"}
        };
        auto it = err_map.find(code);
        if (it == err_map.end()) {
            return "未知错误！";
        }
        return it->second;
    }

    enum class RType {//RPC请求类型
        REQ_ASYNC = 0,
        REQ_CALLBACK
    };

    enum class TopicOptype {//主题操作类型
        TOPIC_CREATE = 0,
        TOPIC_REMOVE,
        TOPIC_SUBSCRIBE,
        TOPIC_CANCEL,
        TOPIC_PUBLISH
    };

    enum class ServiceOptype {//服务操作类型
        SERVICE_REGISTRY = 0,
        SERVICE_DISCOVERY,
        SERVICE_ONLINE,
        SERVICE_OFFLINE,
        SERVICE_UNKNOW
    };
}