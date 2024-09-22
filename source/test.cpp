// #include "message.hpp"
// #include <memory>
// #include <thread>
// #include "net.hpp"
// #include "dispatcher.hpp"
// #include "abstract.hpp"


// // int main()
// // {
// // rpc::RpcRequest::ptr rrp = rpc::MessageFactory::create<rpc::RpcRequest>();//调用完美转发构造消息
// //  Json::Value param;
// //  param["num_1"] = 11;
// //  param["num_2"] = 22;
// //  rrp->setParams(param);
// //  rrp->setMethod("ADD");
// //  std::string str = rrp->serialize();
// //  std::cout<<str<<std::endl;

// // rpc::BaseMessage::ptr rep = rpc::MessageFactory::create(rpc::MType::REQ_RPC);
// // bool ret = rep ->unserialize(str);
// // if(ret == false)
// // {
// //     return -1;
// // }
// // rpc::RpcRequest::ptr rep_s = std::dynamic_pointer_cast<rpc::RpcRequest>(rep);
// // std::cout<< rep_s->method()<<std::endl;
// // std::cout<<rep_s->params()["num_2"].asInt()<<std::endl;

// // /*查看主题服务*/
// // rpc::TopicRequest::ptr trp = rpc::MessageFactory::create<rpc::TopicRequest>();
// // trp->setTopicKey("news");//设置主题
// // trp->setTopicMsg("yuanshen");//设置主题消息
// // trp->setOptype(rpc::TopicOptype::TOPIC_PUBLISH);
// // std::string str = trp->serialize();
// // std::cout<<str<<std::endl;

// // /*检查消息主题发布是否正常报错——如果设置了消息主题则不报错*/
// // rpc::BaseMessage::ptr bmp = rpc::MessageFactory::create(rpc::MType::REQ_TOPIC);
// // bool ret = bmp -> unserialize(str);
// // if(ret ==false)
// // {
// //     return -1;
// // }
// // ret = bmp ->check();
// // if(ret == false)
// // {
// //     return -1;
// // }
// // rpc::RpcRequest::ptr rep_s = std::dynamic_pointer_cast<rpc::RpcRequest>(bmp);
// // std::cout<< rep_s->method()<<std::endl;

// // /*订阅服务测试*/
// // rpc::ServiceRequest::ptr tmp = rpc::MessageFactory::create<rpc::ServiceRequest>();
// // tmp->setMethod("ADD");                                 // 设置服务
// // tmp->setOptype(rpc::ServiceOptype::SERVICE_DISCOVERY); // 设置服务发现
// // std::string str = tmp->serialize();
// // std::cout << str << std::endl;

// // /*发现服务并打印*/
// // rpc::BaseMessage::ptr svc_s = rpc::MessageFactory::create(rpc::MType::REQ_SERVICE);
// // bool ret = svc_s->unserialize(str);
// // if (ret == false)
// // {
// //     return -1;
// // }
// // ret = svc_s->check();
// // if (ret == false)
// // {
// //     return -1;
// // }

// // rpc::ServiceRequest::ptr tmp_s = std::dynamic_pointer_cast<rpc::ServiceRequest>(tmp);
// // std::cout << tmp_s->method() << std::endl;
// // std::cout << (int)tmp_s->optype()<< std::endl;

// // /*订阅服务测试*/
// // rpc::ServiceRequest::ptr tmp = rpc::MessageFactory::create<rpc::ServiceRequest>();
// // tmp->setMethod("ADD");                                 // 设置服务
// // tmp->setOptype(rpc::ServiceOptype::SERVICE_DISCOVERY); // 设置服务发现
// // tmp->setHost(rpc::Address("127.0.0.1", 9999));
// // std::string str = tmp->serialize();
// // std::cout << str << std::endl;

// // /*发现服务并打印*/
// // rpc::BaseMessage::ptr svc_s = rpc::MessageFactory::create(rpc::MType::REQ_SERVICE);
// // bool ret = svc_s->unserialize(str);
// // if (ret == false)
// // {
// //     return -1;
// // }
// // ret = svc_s->check();
// // if (ret == false)
// // {
// //     return -1;
// // }

// // rpc::ServiceRequest::ptr tmp_s = std::dynamic_pointer_cast<rpc::ServiceRequest>(tmp);
// // std::cout << tmp_s->method() << std::endl;
// // std::cout << (int)tmp_s->optype() << std::endl;
// // std::cout << tmp_s->host().first << std::endl;  // 打印IP地址
// // std::cout << tmp_s->host().second << std::endl; // 打印端口号

// // /*响应测试 */
// // rpc::RpcResponse::ptr tmp = rpc::MessageFactory::create<rpc::RpcResponse>();
// // tmp->setRCode(rpc::RCode::RCODE_OK);//设置响应码
// // tmp->setResult(33);        // 设置响应值
// // std::string str = tmp->serialize();
// // std::cout << str << std::endl;

// // /*发现服务并打印*/
// // rpc::BaseMessage::ptr svc_s = rpc::MessageFactory::create(rpc::MType::RSP_RPC);
// // bool ret = svc_s->unserialize(str);
// // if (ret == false)
// // {
// //     return -1;
// // }
// // ret = svc_s->check();
// // if (ret == false)
// // {
// //     return -1;
// // }
// // rpc::RpcResponse::ptr tmp_s = std::dynamic_pointer_cast<rpc::RpcResponse>(tmp);
// // std::cout << (int)tmp_s->rcode() << std::endl;
// // std::cout <<  tmp_s->result().asInt() << std::endl;

// // /*测试主题响应 */
// // rpc::TopicResponse::ptr tmp = rpc::MessageFactory::create<rpc::TopicResponse>();
// // tmp->setRCode(rpc::RCode::RCODE_OK);//设置响应码
// // std::string str = tmp->serialize();
// // std::cout << str << std::endl;

// // /*发现服务并打印*/
// // rpc::BaseMessage::ptr svc_s = rpc::MessageFactory::create(rpc::MType::RSP_RPC);
// // bool ret = svc_s->unserialize(str);
// // if (ret == false)
// // {
// //     return -1;
// // }
// // ret = svc_s->check();
// // if (ret == false)
// // {
// //     return -1;
// // }
// // rpc::TopicResponse::ptr tmp_s = std::dynamic_pointer_cast<rpc::TopicResponse>(tmp);
// // std::cout << (int)tmp_s->rcode() << std::endl;

// // void onMessage(const rpc::BaseConnection::ptr &conn,rpc::BaseMessage::ptr & msg)
// // {
// //     std::string body = msg->serialize();
// //     std::cout<<body<<endl;
// // }
// void onRpcResponse(const rpc::BaseConnection::ptr& conn, rpc::BaseMessage::ptr& msg)
// {
//     std::cout << "收到Rpc响应";
//     std::string body = msg->serialize();
//     std::cout << body << std::endl;
// }

// void onTopicResponse(const rpc::BaseConnection::ptr& conn, rpc::BaseMessage::ptr& msg)
// {
//     std::cout << "收到Topic响应";
//     std::string body = msg->serialize();
//     std::cout << body << std::endl;
// }

// int main()
// {
//     auto dispather_1 = std::make_shared<rpc::Dispatcher>();//构造一个智能指针对象
//     //auto dispather_2 = std::make_shared<rpc::Dispatcher>();
//     dispather_1->registerHandler<rpc::RpcResponse>(rpc::MType::REQ_RPC, onRpcResponse);         // 注册映射关系
//     dispather_1->registerHandler<rpc::TopicResponse>(rpc::MType::REQ_TOPIC, onTopicResponse); // 注册映射关系

//     auto client = rpc::ClientFactory::create("127.0.0.1", 9090);
//     // client->setMessagCallback(onMessage);//不适用onMessage接口

//     // 将onMessage和对象绑定起来
//     auto message_cb = std::bind(&rpc::Dispatcher::onMessage, dispather_1.get(),
//                                 std::placeholders::_1, std::placeholders::_2);
//     client->setMessageCallback(message_cb); // 将onMessage接口设置给回调函数
//     client->connect();

//     auto rpc_req = rpc::MessageFactory::create<rpc::RpcRequest>();
//     rpc_req->setId("1111");
//     rpc_req->setMType(rpc::MType::REQ_RPC);
//     rpc_req->setMethod("Add");
//     Json::Value param;
//     param["num1"] = 11;
//     param["num2"] = 22;
//     rpc_req->setParams(param);
//     client->send(rpc_req);

//     auto topic_req = rpc::MessageFactory::create<rpc::TopicRequest>();
//     topic_req->setId("1111");
//     topic_req->setMType(rpc::MType::REQ_TOPIC);
//     topic_req->setOptype(rpc::TopicOptype::TOPIC_CREATE);
//     topic_req->setTopicKey("News");
//     client->send(topic_req);
//     std::this_thread::sleep_for(std::chrono::seconds(10));
//     client->shutdown();
//     return 0;
// }
// #include "message.hpp"
// #include <memory>
// #include <thread>
// #include "net.hpp"
// #include "dispatcher.hpp"
// #include "abstract.hpp"

// void onRpcResponse(const rpc::BaseConnection::ptr& conn, rpc::RpcResponse::ptr& msg)
// {
//     std::cout << "收到Rpc响应";
//     std::string body = msg->serialize();
//     std::cout << body << std::endl;
// }

// void onTopicResponse(const rpc::BaseConnection::ptr& conn, rpc::TopicResponse::ptr& msg)
// {
//     std::cout << "收到Topic响应";
//     std::string body = msg->serialize();
//     std::cout << body << std::endl;
// }

// int main()
// {
//     auto dispather_1 = std::make_shared<rpc::Dispatcher>();//构造一个智能指针对象
//     dispather_1->registerHandler<rpc::RpcResponse>(rpc::MType::RSP_RPC, onRpcResponse);         // 注册映射关系
//     dispather_1->registerHandler<rpc::TopicResponse>(rpc::MType::RSP_TOPIC, onTopicResponse); // 注册映射关系

//     auto client = rpc::ClientFactory::create("127.0.0.1", 9999);

//     // 将onMessage和对象绑定起来
//     auto message_cb = std::bind(&rpc::Dispatcher::onMessage, dispather_1.get(),
//                                 std::placeholders::_1, std::placeholders::_2);
//     client->setMessageCallback(message_cb); // 将onMessage接口设置给回调函数
//     client->connect();

//     auto rpc_req = rpc::MessageFactory::create<rpc::RpcRequest>();
//     rpc_req->setId("1111");
//     rpc_req->setMType(rpc::MType::REQ_RPC);
//     rpc_req->setMethod("Add");
//     Json::Value param;
//     param["num1"] = 11;
//     param["num2"] = 22;
//     rpc_req->setParams(param);
//     client->send(rpc_req);

//     auto topic_req = rpc::MessageFactory::create<rpc::TopicRequest>();
//     topic_req->setId("1111");
//     topic_req->setMType(rpc::MType::REQ_TOPIC);
//     topic_req->setOptype(rpc::TopicOptype::TOPIC_CREATE);
//     topic_req->setTopicKey("News");
//     client->send(topic_req);
    
//     std::this_thread::sleep_for(std::chrono::seconds(100));
//     client->shutdown();
//     return 0;
// }
/*Test*/
/*Test*/