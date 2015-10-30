#include <stdio.h>

#include "zhttp.h"
#include <pthread.h>

using namespace zhttp;


void handler2(Request& req, Response& res)
{
    //printf("[handler2 tid: %d]处理请求的业务逻辑\n", pthread_self());
    //printf("你请求的URL为: %s\n", req.GetUrl().c_str());
    res.set_content_type("application/json");
    res.add_header("sign", "this is the signed string...");
    res.write("{\"name\":\"zn\", \"age\":27}");
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

    ZHttpApp app;
    app.route("/", boost::bind(&handler1, _1, _2))
       .route("/test", boost::bind(&handler2, _1, _2))
       .listen("0.0.0.0", 12321)
       .run(4);

    return 0;
}
