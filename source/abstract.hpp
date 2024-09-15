#pragma once
#include <memory>
#include <functional>
#include "fields.hpp"

//抽象层实现
namespace rpc {
    class BaseMessage { 
        public:
            using ptr = std::shared_ptr<BaseMessage>; //起别名，便于使用，并且使用智能指针管理对象

            virtual ~BaseMessage(){}
            virtual void setId(const std::string &id) {//定义为虚函数以供多态
                _rid = id;
            }

            virtual std::string rid() { return _rid; }
            virtual void setMType(MType mtype) {
                _mtype = mtype;
            }
            virtual MType mtype() { return _mtype; }

            virtual std::string serialize() = 0;//序列化，因为每个对象的序列化和反序列化方式不同，故使用虚函数
            virtual bool unserialize(const std::string &msg) = 0;//反序列化
            virtual bool check() = 0;//消息校验，看消息是否是齐备的

        private:
            MType _mtype;//消息类型
            std::string _rid;//消息Id
    };

    class BaseBuffer {//对缓冲区进行四字节操作，纯虚函数
        public:
            using ptr = std::shared_ptr<BaseBuffer>;
            virtual size_t readableSize() = 0;
            virtual int32_t peekInt32() = 0;
            virtual void retrieveInt32() = 0;
            virtual int32_t readInt32() = 0;
            virtual std::string retrieveAsString(size_t len) = 0;
    };

    class BaseProtocol {//将传递的Buff的数据，转化为消息
        public:
            using ptr = std::shared_ptr<BaseProtocol>;
            virtual bool canProcessed(const BaseBuffer::ptr &buf) = 0;//是否能够处理buff数据
            virtual bool onMessage(const BaseBuffer::ptr &buf, BaseMessage::ptr &msg) = 0;//将buff转化为数据 `BaseMessage`对象，使用别名并用智能指针管理起来
        public:
            virtual std::string serialize(const BaseMessage::ptr &msg) = 0;//序列化消息
    };

    class BaseConnection {//需要有BaseProtocol对象，用来先处理消息，便于发送。
        public:
            using ptr = std::shared_ptr<BaseConnection>;
            virtual void send(const BaseMessage::ptr &msg) = 0;//发送消息
            virtual void shutdown() = 0;//关闭连接
            virtual bool connected() = 0;//建立连接
    };
    /*-------设置回调函数类型----------*/
    using ConnectionCallback = std::function<void(const BaseConnection::ptr&)>;
    using CloseCallback = std::function<void(const BaseConnection::ptr&)>;
    using MessageCallback = std::function<void(const BaseConnection::ptr&, BaseMessage::ptr&)>;
    /*-------设置回调函数类型----------*/
  
  
    //服务器抽象
    class BaseServer {
        public:
            using ptr = std::shared_ptr<BaseServer>;
            virtual void setConnectionCallback(const ConnectionCallback& cb) {
                _cb_connection = cb;
            }
            virtual void setCloseCallback(const CloseCallback& cb) {
                _cb_close = cb;
            }
            virtual void setMessageCallback(const MessageCallback& cb) {
                _cb_message = cb;
            }
            virtual void start() = 0;//服务器启动，不过具体方法由子类实现。
        protected:
            ConnectionCallback _cb_connection;
            CloseCallback _cb_close;
            MessageCallback _cb_message;
    };
    //客户端抽象
    class BaseClient {
        public:
            using ptr = std::shared_ptr<BaseClient>;
            virtual void setConnectionCallback(const ConnectionCallback& cb) {
                _cb_connection = cb;
            }
            virtual void setCloseCallback(const CloseCallback& cb) {
                _cb_close = cb;
            }
            virtual void setMessageCallback(const MessageCallback& cb) {
                _cb_message = cb;
            }
            /*操作比较多，因为客户端需要提供给用户使用*/
            virtual void connect() = 0;
            virtual void shutdown() = 0;
            virtual bool send(const BaseMessage::ptr&) = 0;
            virtual BaseConnection::ptr connection() = 0;
            virtual bool connected() = 0;
        protected:
            ConnectionCallback _cb_connection;
            CloseCallback _cb_close;
            MessageCallback _cb_message;
    };
}
