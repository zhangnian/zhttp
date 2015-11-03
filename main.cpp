#include <stdio.h>

#include <pthread.h>

#include <mysql++.h>

#include "zhttp.h"
#include "zk_helper.h"
#include "json_utils.h"

using namespace zhttp;

#define RETURN_ERR(status, info) \
    res.writeJson(JsonUtils::MakeResponse(status, info)); \
    return;


void handle_login(Request& req, Response& res)
{
    printf("[TID: %lld]handle requests\n", pthread_self());

    const std::string& username = req.GetQueryStr("username");
    const std::string& password = req.GetQueryStr("password");
    if( username.size() == 0 || password.size() == 0 )
    {
        RETURN_ERR(10003, "参数错误");
    }

    mysqlpp::Connection conn(false);
    if( !conn.connect("vvshop", "120.24.65.221", "admin", "admin123") )
    {
        RETURN_ERR(10004, "连接数据库失败");
    }

    char buf[256] = { 0 };
    snprintf(buf, sizeof buf, "select id, cs_token from cs_login where realname='%s' and password = '%s';",
            username.c_str(), password.c_str());

    mysqlpp::Query query = conn.query(buf);
    std::string uid = "";
    std::string token = "";
    if (mysqlpp::StoreQueryResult result = query.store())
    {
		mysqlpp::StoreQueryResult::const_iterator it;
		for (it = result.begin(); it != result.end(); ++it) {
			mysqlpp::Row row = *it;
            uid = (char*)row["id"].c_str();
            token = (char*)row["cs_token"].c_str();
			//printf("uid: %s, cs_token: %s\n", row[0].c_str(), row[1].c_str());
		}
	}
	else
    {
		RETURN_ERR(10005, "查询数据库失败");
		return;
	}

    if( uid.size() > 0 && token.size() > 0 )
    {
        std::map<std::string, std::string> args;
        args["id"] = uid;
        args["token"] = token;

        res.writeJson(JsonUtils::MakeResponse(0, "Success", args));
        return;
    }

    RETURN_ERR(10006, "找不到用户数据");
}



int main(int argc, char* argv[])
{
    if( argc != 2 )
    {
        printf("usage: %s port\n", argv[0]);
        return -1;
    }

    printf("[TID: %lld]zhttp start...\n", pthread_self());

    ZHttpApp app;
    app.route("/login", boost::bind(&handle_login, _1, _2))
       .listen("0.0.0.0", atoi(argv[1]))
       .run();



    return 0;
}
