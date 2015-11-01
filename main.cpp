#include <stdio.h>

#include <pthread.h>


#include "zhttp.h"
#include "zk_helper.h"
#include "json_utils.h"

using namespace zhttp;

#define RETURN_ERR(code, msg) \
    res.writeJson(JsonUtils::MakeResponse(code, msg)); \
    return;


void handle_query(Request& req, Response& res)
{
    std::string sign = req.GetHeader("sign");
    printf("sign: %s\n", sign.c_str());

    const std::string& uri = req.GetUrl();
    if( uri != "/query")
    {
        RETURN_ERR(10002, "请求URI错误");
    }

    const std::string& s_name = req.GetQueryStr("s_name");
    if( s_name.size() == 0 )
    {
        RETURN_ERR(10003, "参数错误");
    }

    // 去注册中心查询服务地址
    std::map<std::string, std::string> dict_args;
    dict_args["s_name"] = s_name;
    dict_args["s_addr"] = "http://api.example.com/v1/chat";
    res.writeJson(JsonUtils::MakeResponse(0, "", dict_args));

    return;
}



int main(int argc, char* argv[])
{
    printf("zhttp start...\n");

    //ZK_INS->registe_service("/test", "http://58.67.219.143:12321/test");

    ZHttpApp app;
    app.route("/query", boost::bind(&handle_query, _1, _2))
       .listen("0.0.0.0", 12345)
       .run(4);



    return 0;
}
