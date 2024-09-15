#include "message.hpp"
int main()
{
    rpc::RpcRequest::ptr rrp = rpc::MessageFactory::create<rpc::RpcRequest>();//调用完美转发构造消息
    Json::Value param;
    param["num_1"] = 11;
    param["num_2"] = 22;
    rrp->setParams(param);
    rrp->setMethod("ADD");
    std::string str = rrp->serialize();
    std::cout<<str<<std::endl;
    
    rpc::BaseMessage::ptr rep = rpc::MessageFactory::create(rpc::MType::REQ_RPC);
    bool ret = rep ->unserialize(str);
    if(ret == false)
    {
        return -1;
    }
    rpc::RpcRequest::ptr rep_s = std::dynamic_pointer_cast<rpc::RpcRequest>(rep);
    std::cout<< rep_s->method()<<std::endl;
    std::cout<<rep_s->params()["num_2"].asInt()<<std::endl;

    return 0;
}