#include <muduo/net/TcpClient.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>
#include <muduo/net/TcpConnection.h>
#include <muduo/net/Buffer.h>
#include <muduo/base/CountDownLatch.h>
#include <iostream>
#include <string>

class DictClient {
    public:
        DictClient(const std::string &sip, int sport):
            _baseloop(_loopthread.startLoop()),//初始化客户端，新的线程启动，进行事件循环
            _downlatch(1),//初始化计数器为1，因为为0时才会唤醒
            _client(_baseloop, muduo::net::InetAddress(sip, sport), "DictClient")
        {
            //设置连接事件（连接建立/管理）的回调
            _client.setConnectionCallback(std::bind(&DictClient::onConnection, this, std::placeholders::_1));
            //设置连接消息的回调
            _client.setMessageCallback(std::bind(&DictClient::onMessage, this, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
            
            //连接服务器
            _client.connect();//connet是一个非阻塞的接口，此时不一定有数据，因此需要进行同步
            _downlatch.wait();//经过连接的回调函数处理后，_downlatch.countDown()经过--，此时为0阻塞
        }

        bool send(const std::string &msg) {
            if (_conn->connected() == false) {
                std::cout << "连接已经断开，发送数据失败！\n";
                return false;
            }
            _conn->send(msg);
            return true;
        }
    
    private:
        void onConnection(const muduo::net::TcpConnectionPtr &conn) {
            if (conn->connected()) {
                std::cout << "连接建立！\n";
                _downlatch.countDown();//计数--，为0时唤醒阻塞
                _conn = conn;
            }else {
                std::cout << "连接断开！\n";
                _conn.reset();
            }
        }
        void onMessage(const muduo::net::TcpConnectionPtr &conn, muduo::net::Buffer *buf, muduo::Timestamp){
            std::string res = buf->retrieveAllAsString();
            std::cout << res << std::endl;
        }
    private:
        muduo::net::TcpConnectionPtr _conn;
        muduo::CountDownLatch _downlatch;
        muduo::net::EventLoopThread _loopthread;//一旦实例化之后，就会自动的循环监控
        muduo::net::EventLoop *_baseloop;//EventLoop *的指针指向_loopthread
        muduo::net::TcpClient _client;
};

int main()
{
    DictClient client("127.0.0.1",9999);
    while(1) {
        std::string msg;
        std::cin >> msg;
        client.send(msg);
    }
    return 0;
}