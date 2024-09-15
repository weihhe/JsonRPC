/*
    实现一个翻译服务器，客户端发送过来一个英语单词，返回一个汉语词语
*/
#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <iostream>
#include <string>
#include <unordered_map>

class DictServer {
    public:
        DictServer(int port): _server(&_baseloop, //构造Server对象
            muduo::net::InetAddress("0.0.0.0", port), 
            "DictServer", muduo::net::TcpServer::kReusePort)//kReusePort地址重用
        {
            //设置连接事件（连接建立/管理）的回调
            _server.setConnectionCallback(std::bind(&DictServer::onConnection, this, std::placeholders::_1));//因为是类成员，需要加上this指针，所以要使用bind
            //设置连接消息的回调
            _server.setMessageCallback(std::bind(&DictServer::onMessage, this, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        }
        void start() {
            _server.start();//先开始监听
            _baseloop.loop();//然后开始死循环事件监控，start和loop顺序不能改变
        }
    private:
        void onConnection(const muduo::net::TcpConnectionPtr &conn) { //回调函数
            if (conn->connected()) {
                std::cout << "连接建立！\n";
            }else {
                std::cout << "连接断开！\n";
            }
        }
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp){//回调函数
            static std::unordered_map<std::string, std::string> dict_map = {//建立词典
                {"hello",  "你好"},
                {"world",  "世界"},
                {"gua",  "瓜"}
            };
            std::string msg = buf->retrieveAllAsString();//全部取出
            std::string res;
            auto it = dict_map.find(msg);
            if (it != dict_map.end()) {
                res = it->second;
            }else {
                res = "未知单词！";
            }
            conn->send(res);//对客户端进行响应
        }
    private:
        muduo::net::EventLoop _baseloop;//类似于监听套接字，先创建EventLoop，因为Server需要它来构造
        muduo::net::TcpServer _server;//服务器对象
};

int main()
{
    DictServer server(9999);
    server.start();
    return 0;
}
/*
    实现一个翻译服务器，客户端发送过来一个英语单词，返回一个汉语词语
*/
