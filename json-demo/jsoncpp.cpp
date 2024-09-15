#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <jsoncpp/json/json.h>


bool serilize(const Json::Value &val,std::string &body) //body为返回类型,bool无实义
{
    std::stringstream ss;
    Json::StreamWriterBuilder swb; // 工厂(模式类对象swb)进行生产StreamWriter类
    std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
    int ret = sw->write(val,&ss);
    if (ret != 0)
    {
        std::cout << "Serialize failed!\n";
        return -1;
    }
    body = ss.str();
    return true;    
}  
bool unserilize(const std::string& body,Json::Value& val)
{
    //实例化工厂对象
    Json::CharReaderBuilder crb;
    //生产CharReader对象
    std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
    std::string errs;
    bool ret1 = cr->parse(body.c_str(), body.c_str() + body.size(), &val, &errs);
    if (ret1 == false)
    {
        std::cout << "UnSerialize failed!" << std::endl;
        return false;
    }
    return true;
}
int main()
{
    // 序列化
    const char *name = "小刚";
    int age = 18;
    const char *sex = "男";
    float score[3] = {88, 77.5, 66};
    Json::Value stu;
    stu["姓名"] = name;
    stu["性别"] = sex;
    stu["年龄"] = age;
    stu["成绩"].append(score[0]);
    stu["成绩"].append(score[1]);
    stu["成绩"].append(score[2]);

    Json::Value sport;
    sport["yuanshen"] = "qidong";
    sport["jiane"] = "geile";

    std::string body;//创建对象
    serilize(stu,body);//将我们的对象给序列化
    std::cout<<body<<std::endl;

    //反序列化
    Json::Value stu2;
    std::string str = R"({"姓名":"小黑","年龄":19,"成绩":[32,45,56]})";
    bool ret = unserilize(str,stu2);//解析json对象
    if(ret ==false)
    {
        return -1;
    }
  if (ret == false) 
        return -1;
    std::cout << "姓名: " <<  stu2["姓名"].asString() << std::endl;
    std::cout << "年龄: " <<  stu2["年龄"].asInt() << std::endl;
    int sz = stu2["成绩"].size();
    for (int i = 0; i < sz; i++) {
        std::cout << "成绩: " <<  stu2["成绩"][i].asFloat() << std::endl;
    }
    return 0;
}
