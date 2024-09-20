#include "message.hpp"
#include <memory>
#include "dispatcher.hpp"
#include "abstract.hpp"


//以下回调函数都匹配这个类型 using MessageCallback = std::function<void(const BaseConnection::ptr&, BaseMessage::ptr&)>;

void onRpcRequest(const rpc::BaseConnection::ptr& conn, rpc::RpcRequest::ptr& msg)
{
    std::cout << "收到了Rpc请求";
    std::string body = msg->serialize();
    std::cout << body << std::endl;
    auto rpc_req = rpc::MessageFactory::create<rpc::RpcResponse>();
    rpc_req->setId("1111");
    rpc_req->setMType(rpc::MType::RSP_RPC);
    rpc_req->setRCode(rpc::RCode::RCODE_OK);
    rpc_req->setResult(33);
    conn->send(rpc_req);
}
void onTopicRequest(const rpc::BaseConnection::ptr& conn, rpc::TopicRequest::ptr& msg)
{
    std::cout << "收到了Topic请求";
    std::string body = msg->serialize();
    std::cout << body << std::endl;
    auto rpc_req = rpc::MessageFactory::create<rpc::TopicResponse>();
    rpc_req->setId("1112");
    rpc_req->setMType(rpc::MType::RSP_TOPIC);
    rpc_req->setRCode(rpc::RCode::RCODE_OK);
    conn->send(rpc_req);
}
int main()
{
    auto dispather_1 = std::make_shared<rpc::Dispatcher>();
    dispather_1->registerHandler<rpc::RpcRequest>(rpc::MType::REQ_RPC, onRpcRequest);  
    dispather_1->registerHandler<rpc::TopicRequest>(rpc::MType::REQ_TOPIC, onTopicRequest);
    // client->setMessagCallback(onMessage);//不适用onMessage接口
    // 将onMessage和对象绑定起来
    auto server = rpc::ServerFactory::create(9999);

    // auto message_cb = std::bind(&rpc::Dispatcher::onMessage, dispather_1.get(),
    //                             std::placeholders::_1, std::placeholers::_2);
    //.get（）获取原始指针
    auto message_cb = std::bind(&rpc::Dispatcher::onMessage, dispather_1.get(),
                                std::placeholders::_1, std::placeholders::_2);

    server->setMessageCallback(message_cb); // 将回调函数设置到服务器中

    server->start();
    return 0;
}
