#pragma once
#include "../net.hpp"
#include "../message.hpp"
#include <future>
#include <functional>

namespace rpc {
    namespace client {
        class Requestor {
            public:
                using ptr = std::shared_ptr<Requestor>;
                using RequestCallback = std::function<void(const BaseMessage::ptr&)>;
                using AsyncResponse = std::future<BaseMessage::ptr>;

                //描述请求信息
                struct RequestDescribe {
                    using ptr = std::shared_ptr<RequestDescribe>;
                    BaseMessage::ptr request;
                    RType rtype;//请求类型
                    std::promise<BaseMessage::ptr> response;
                    RequestCallback callback;//回调函数
                };


                //设置回调函数，和dispatcher保持一致
                void onResponse(const BaseConnection::ptr &conn, BaseMessage::ptr &msg){
                    std::string rid = msg->rid();//拿到请求方式
                    RequestDescribe::ptr rdp = getDescribe(rid);//异步的请求方式
                    if (rdp.get() == nullptr) {
                        ELOG("收到响应 - %s，但是未找到对应的请求描述！", rid.c_str());
                        return;
                    }
                    // 根据不同的请求方式，进行不同的处理
                    if (rdp->rtype == RType::REQ_ASYNC) {
                        rdp->response.set_value(msg);
                        //回调函数处理
                    }else if (rdp->rtype == RType::REQ_CALLBACK){
                        if (rdp->callback) rdp->callback(msg);
                    }else {
                        ELOG("请求类型未知！！");
                    }
                    delDescribe(rid);
                }
                //使用异步请求send
                bool send(const BaseConnection::ptr &conn, const BaseMessage::ptr &req, AsyncResponse &async_rsp) {
                    RequestDescribe::ptr rdp = newDescribe(req, RType::REQ_ASYNC);
                    if (rdp.get() == nullptr) {
                        ELOG("构造请求描述对象失败！");
                        return false;
                    }
                    conn->send(req);
                    async_rsp = rdp->response.get_future();
                    return true;
                }
                //使用同步请求send
                bool send(const BaseConnection::ptr &conn, const BaseMessage::ptr &req, BaseMessage::ptr &rsp) {
                    AsyncResponse rsp_future;
                    bool ret = send(conn, req, rsp_future);
                    if (ret == false) {
                        return false;
                    }
                    rsp = rsp_future.get();//没有响应则会一直阻塞，达到同步的目的
                    return true;
                }
                //回调处理
                bool send(const BaseConnection::ptr &conn, const BaseMessage::ptr &req, const RequestCallback &cb) {
                    RequestDescribe::ptr rdp = newDescribe(req, RType::REQ_CALLBACK, cb);
                    if (rdp.get() == nullptr) {
                        ELOG("构造请求描述对象失败！");
                        return false;
                    }
                    conn->send(req);
                    return true;
                }
            private:
            //新增一个请求信息，包含请求信息和请求类型
                RequestDescribe::ptr newDescribe(const BaseMessage::ptr &req, RType rtype, 
                    const RequestCallback &cb = RequestCallback()) {
                    std::unique_lock<std::mutex> lock(_mutex);
                    RequestDescribe::ptr rd = std::make_shared<RequestDescribe>();
                    //构造请求描述
                    rd->request = req;
                    rd->rtype = rtype;
                    if (rtype == RType::REQ_CALLBACK && cb) {
                        rd->callback = cb;
                    }
                    _request_desc.insert(std::make_pair(req->rid(), rd));
                    return rd;
                }
                //寻找请求描述信息
                RequestDescribe::ptr getDescribe(const std::string &rid) {
                    std::unique_lock<std::mutex> lock(_mutex);
                    auto it = _request_desc.find(rid);
                    if (it == _request_desc.end()) {
                        return RequestDescribe::ptr();
                    }
                    return it->second;
                }

                //删除请求描述信息
                void delDescribe(const std::string &rid) {
                    std::unique_lock<std::mutex> lock(_mutex);
                    _request_desc.erase(rid);
                }
            private:
                std::mutex _mutex;
                std::unordered_map<std::string, RequestDescribe::ptr> _request_desc;
        };
    }
}