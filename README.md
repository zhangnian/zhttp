# zhttp
现代C++风格的Web API开发框架

## 代码示例
```
void handler1(Request& req, Response& res)
{
    printf("你请求的URL为: %s\n", req.GetUrl().c_str());
    res.write("handler1 response!\n");
}

int main(int argc, char* argv[])
{
    ZHttpApp app;
    app.route("/", boost::bind(&handler1, _1, _2))
       .route("/test", boost::bind(&handler1, _1, _2))
       .listen("0.0.0.0", 12321)
       .run();

    return 0;
}
```