#include "detail.hpp"
#include "fields.hpp"
#include "abstract.hpp"


namespace rpc {
    typedef std::pair<std::string, int> Address;//设置网络地址


    class JsonMessage : public BaseMessage {
        public:
            using ptr = std::shared_ptr<JsonMessage>;
            virtual std::string serialize() override {
                std::string body;
                bool ret = JSON::serialize(_body, body);//序列化
                if (ret == false) {
                    return std::string();//失败了返回一个空string
                }
                return body;
            }
            //override，判断是否重写成功
            virtual bool unserialize(const std::string &msg) override {
                return JSON::unserialize(msg, _body);//反序列化
            }
        protected://便于继承之后可以访问
            Json::Value _body;//添加一个JSon对象
    };

    class JsonRequest : public JsonMessage {
        public:
            using ptr = std::shared_ptr<JsonRequest>;
    };

    class JsonResponse : public JsonMessage {
        public:
            using ptr = std::shared_ptr<JsonResponse>;
            virtual bool check() override {
                //设计中大部分的响应都有响应状态码
                //因此只需要判断响应状态码字段是否存在，类型是否正确即可
                if (_body[KEY_RCODE].isNull() == true) {
                    ELOG("响应中没有响应状态码！");
                    return false;
                }
                if (_body[KEY_RCODE].isIntegral() == false) {
                    ELOG("响应状态码类型错误！");
                    return false;
                }
                return true;
            }
            virtual RCode rcode() {
                return (RCode)_body[KEY_RCODE].asInt();
            }
            virtual void setRCode(RCode rcode) {
                _body[KEY_RCODE] = (int)rcode;
            }
    };
    //客户端
    class RpcRequest : public JsonRequest {
        public:
            using ptr = std::shared_ptr<RpcRequest>;
            virtual bool check() override {
                //rpc请求中，包含请求方法名称-字符串，参数字段-对象
                //方法检查
                if (_body[KEY_METHOD].isNull() == true ||
                    _body[KEY_METHOD].isString() == false) {
                    ELOG("RPC请求中没有方法名称或方法名称类型错误！");
                    return false;
                }
                //参数检查
                if (_body[KEY_PARAMS].isNull() == true ||
                    _body[KEY_PARAMS].isObject() == false) {
                    ELOG("RPC请求中没有参数信息或参数信息类型错误！");
                    return false;
                }
                return true;
            }
            //返回JSon当中的请求方法名称。
            std::string method() {
                return _body[KEY_METHOD].asString();
            }
            //设置方法名称
            void setMethod(const std::string &method_name) {
                _body[KEY_METHOD] = method_name;
            }
            //返回参数
            Json::Value params() {
                return _body[KEY_PARAMS];
            }
            //设置参数
            void setParams(const Json::Value &params) {
                _body[KEY_PARAMS] = params;
            }
    };

    //发布操作
    class TopicRequest : public JsonRequest {
        public:
            using ptr = std::shared_ptr<TopicRequest>;

            virtual bool check() override {
                //rpc请求中，包含请求方法名称-字符串，参数字段-对象
                if (_body[KEY_TOPIC_KEY].isNull() == true ||
                    _body[KEY_TOPIC_KEY].isString() == false) {
                    ELOG("主题请求中没有主题名称或主题名称类型错误！");
                    return false;
                }
                if (_body[KEY_OPTYPE].isNull() == true ||
                    _body[KEY_OPTYPE].isIntegral() == false) {
                    ELOG("主题请求中没有操作类型或操作类型的类型错误！");
                    return false;
                }
                //因为需要发布请求消息给提供对应服务的服务器，所以需要进行消息检测
                if (_body[KEY_OPTYPE].asInt() == (int)TopicOptype::TOPIC_PUBLISH &&
                    (_body[KEY_TOPIC_MSG].isNull() == true ||
                    _body[KEY_TOPIC_MSG].isString() == false)) {
                    ELOG("主题消息发布请求中没有消息内容字段或消息内容类型错误！");
                    return false;
                }
                return true;
            }
            //返回主题名称
            std::string topicKey() {
                return _body[KEY_TOPIC_KEY].asString();
            }
            //设置主题名称
            void setTopicKey(const std::string &key) {
                _body[KEY_TOPIC_KEY] = key;
            }
            //获得操作类型
            TopicOptype optype() {
                return (TopicOptype)_body[KEY_OPTYPE].asInt();//需要将整型强转
            }
            //设置操作类型
            void setOptype(TopicOptype optype) {
                _body[KEY_OPTYPE] = (int)optype;
            }
            //获得主题消息
            std::string topicMsg() {
                return _body[KEY_TOPIC_MSG].asString();
            }
            //设置主题消息
            void setTopicMsg(const std::string &msg) {
                _body[KEY_TOPIC_MSG] = msg;
            }

    };
    
    class ServiceRequest : public JsonRequest {
        public:
            using ptr = std::shared_ptr<ServiceRequest>;
            virtual bool check() override {
                //rpc请求中，包含请求方法名称-字符串，参数字段-对象
                if (_body[KEY_METHOD].isNull() == true ||
                    _body[KEY_METHOD].isString() == false) {
                    ELOG("服务请求中没有方法名称或方法名称类型错误！");
                    return false;
                }
                if (_body[KEY_OPTYPE].isNull() == true ||
                    _body[KEY_OPTYPE].isIntegral() == false) {
                    ELOG("服务请求中没有操作类型或操作类型的类型错误！");
                    return false;
                }
                //只有我们“不是进行服务发现”，才需要检查host格式，因为“服务发现”没有host信息————因为服务发现的目的就是找到host
                if (_body[KEY_OPTYPE].asInt() != (int)(ServiceOptype::SERVICE_DISCOVERY) &&
                    (_body[KEY_HOST].isNull() == true ||
                    _body[KEY_HOST].isObject() == false ||
                    _body[KEY_HOST][KEY_HOST_IP].isNull() == true ||
                    _body[KEY_HOST][KEY_HOST_IP].isString() == false ||
                    _body[KEY_HOST][KEY_HOST_PORT].isNull() == true ||
                    _body[KEY_HOST][KEY_HOST_PORT].isIntegral() == false)) {
                    ELOG("服务请求中主机地址信息错误！");
                    return false;
                }
                return true;
            }
            //获得方法
            std::string method() {
                return _body[KEY_METHOD].asString();
            }
            void setMethod(const std::string &name) {
                _body[KEY_METHOD] = name;
            }
            ServiceOptype optype() {
                return (ServiceOptype)_body[KEY_OPTYPE].asInt();//整形到枚举类型强转
            }
            void setOptype(ServiceOptype optype) {
                _body[KEY_OPTYPE] = (int)optype;
            }
            Address host() {
                Address addr;
                addr.first = _body[KEY_HOST][KEY_HOST_IP].asString();
                addr.second = _body[KEY_HOST][KEY_HOST_PORT].asInt();
                return addr;
            }
            //设置网络
            void setHost(const Address &host) {
                Json::Value val;//构造一个JSon网络格式的对象
                val[KEY_HOST_IP] = host.first;//通过host获取ip和port
                val[KEY_HOST_PORT] = host.second;
                _body[KEY_HOST] = val;//根据KEY_HOST将此时的数据插入到JSon对象中
                //即：JSon中的host信息是一个JSON对象数组
            }
    };
    
    class RpcResponse : public JsonResponse {
        public:
            using ptr = std::shared_ptr<RpcResponse>;
            virtual bool check() override {
                if (_body[KEY_RCODE].isNull() == true ||
                    _body[KEY_RCODE].isIntegral() == false) {
                    ELOG("响应中没有响应状态码,或状态码类型错误！");
                    return false;
                }
                if (_body[KEY_RESULT].isNull() == true) {
                    ELOG("响应中没有Rpc调用结果,或结果类型错误！");
                    return false;
                }
                return true;
            }
            Json::Value result() {
                return _body[KEY_RESULT];
            }
            void setResult(const Json::Value &result) {
                _body[KEY_RESULT] = result;
            }
    };

    class TopicResponse : public JsonResponse {//使用父类中的响应就行了
        public:
            using ptr = std::shared_ptr<TopicResponse>;
    };

    class ServiceResponse : public JsonResponse {
        public:
            using ptr = std::shared_ptr<ServiceResponse>;
            virtual bool check() override {
                if (_body[KEY_RCODE].isNull() == true ||
                    _body[KEY_RCODE].isIntegral() == false) {
                    ELOG("响应中没有响应状态码,或状态码类型错误！");
                    return false;
                }
                if (_body[KEY_OPTYPE].isNull() == true ||
                    _body[KEY_OPTYPE].isIntegral() == false) {
                    ELOG("响应中没有操作类型,或操作类型的类型错误！");
                    return false;
                }
                if (_body[KEY_OPTYPE].asInt() == (int)(ServiceOptype::SERVICE_DISCOVERY) &&
                   (_body[KEY_METHOD].isNull() == true ||
                    _body[KEY_METHOD].isString() == false ||
                    _body[KEY_HOST].isNull() == true ||
                    _body[KEY_HOST].isArray() == false)) {
                    ELOG("服务发现响应中响应信息字段错误！");
                    return false;
                }
                return true;
            }
            ServiceOptype optype() {
                return (ServiceOptype)_body[KEY_OPTYPE].asInt();
            }
            void setOptype(ServiceOptype optype) {
                _body[KEY_OPTYPE] = (int)optype;
            }
            std::string method() {
                return _body[KEY_METHOD].asString();
            }
            void setMethod(const std::string &method) {
                _body[KEY_METHOD] = method;
            }
            void setHost(std::vector<Address> addrs) {
                for (auto &addr : addrs) {
                    Json::Value val;
                    val[KEY_HOST_IP] = addr.first;
                    val[KEY_HOST_PORT] = addr.second;
                    _body[KEY_HOST].append(val);
                }
            }
            std::vector<Address> hosts() {
                std::vector<Address> addrs;
                int sz = _body[KEY_HOST].size();
                for (int i = 0; i < sz; i++) {
                    Address addr;
                    addr.first = _body[KEY_HOST][i][KEY_HOST_IP].asString();
                    addr.second = _body[KEY_HOST][i][KEY_HOST_PORT].asInt();
                    addrs.push_back(addr);
                }
                return addrs;
            }
    };

    //实现一个消息对象的生产工厂，将对象的构造整合起来。便于后期维护
    class MessageFactory {
        public:
            static BaseMessage::ptr create(MType mtype) {//返回类型为父类
                switch(mtype) {
                    case MType::REQ_RPC : return std::make_shared<RpcRequest>();
                    case MType::RSP_RPC : return std::make_shared<RpcResponse>();
                    case MType::REQ_TOPIC : return std::make_shared<TopicRequest>();
                    case MType::RSP_TOPIC : return std::make_shared<TopicResponse>();
                    case MType::REQ_SERVICE : return std::make_shared<ServiceRequest>();
                    case MType::RSP_SERVICE : return std::make_shared<ServiceResponse>();
                }
                return BaseMessage::ptr();//如果类型都不对，就返回一个空父类对象
            }
            
            template<typename T, typename ...Args>//通过模板参数，确定我们需要构造对象的类型
            //
            static std::shared_ptr<T> create(Args&& ...args) {//引用折叠，既可以接受左值，又可以接受右值
                return std::make_shared<T>(std::forward(args)...);//完美转发
            }
    };
}


