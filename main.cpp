#include <stdio.h>

#include <pthread.h>


#include "zhttp.h"
#include "zk_helper.h"


using namespace zhttp;


void handler2(Request& req, Response& res)
{
    //printf("[handler2 tid: %d]处理请求的业务逻辑\n", pthread_self());
    //printf("你请求的URL为: %s\n", req.GetUrl().c_str());
    res.set_content_type(Response::kJSON);
    //res.set_keepalive();
    //res.add_header("sign", "this is the signed string...");
    res.write("{\"name\":\"张念\", \"age\":27}");
}


void handler1(Request& req, Response& res)
{
    //printf("[ handler1tid: %d]处理请求的业务逻辑\n", pthread_self());

    //printf("你请求的URL为: %s\n", req.GetUrl().c_str());
    res.write("handler1 response!\n");
}

int main(int argc, char* argv[])
{
    printf("zhttp start...\n");

    ZK_INS->registe_service("/test", "http://58.67.219.143:12321/test");

    ZHttpApp app;
    app.route("/", boost::bind(&handler1, _1, _2))
       .route("/test", boost::bind(&handler2, _1, _2))
       .listen("0.0.0.0", 12321)
       .run(3);



    return 0;
}
